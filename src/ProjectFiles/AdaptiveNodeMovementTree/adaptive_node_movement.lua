status = "SETUP"
opt = nil
shuffled = nil
requests = nil
posmap = nil

-------------------------------------------------------------------------------
-- Extension of standard library (Knuth shuffle for tables) -------------------
-------------------------------------------------------------------------------

function table.shuffle(t)
  local n = #t
  while n > 1 do
    local k = math.random(n)
    t[n], t[k] = t[k], t[n]
    n = n - 1
  end
  return t
end

-------------------------------------------------------------------------------
-- Utility functions for heap handling ----------------------------------------
-------------------------------------------------------------------------------

function parent(i, heap) 
  local p = math.floor(i / 2)
  if p < 1 then
    -- node i has no parent
    p = nil
  end
  return p
end

function lc(i, heap)
  local c = 2*i
  if c > #heap then
    -- node i has no left child
    c = nil
  end
  return c
end

function rc(i, heap)
  local c = 2*i + 1
  if c > #heap then
    -- node i has no right child
    c = nil
  end
  return c
end

function position(i)
  local base = math.floor(math.log(i) / math.log(2))
  return (2* (i - 2^base) + 1) / (2^(base + 1))
end

function range(i)
  local base = math.floor(math.log(i) / math.log(2))
  local min = (i - 2^base) / (2^base)
  local max = (i - 2^base + 1) / (2^base)
  return min, max
end

function get_next(s, t, heap) 
  -- self loop
  if (s == t) then
    return s
  end
  
  -- get 'sorted' positions and range of start node
  local pt = position(t)
  local ps = position(s)
  local smin, smax = range(s)

  -- is target node a child of start node?
  if pt > smin and pt < ps then
    return lc(s, heap)
  elseif pt > ps and pt < smax then
    return rc(s, heap)
  else
    return parent(s, heap)
  end
end

function hop_distance(s, t, heap)
  if s == t then
    return 0
  else
    return 1 + hop_distance(get_next(s, t, heap), t, heap)
  end
end

-------------------------------------------------------------------------------
-- Functions for request generation and handling ------------------------------
-------------------------------------------------------------------------------

function get_weight(distance)
  -- the larger the return value the more likely the selection of the request
  return 1/distance
end

function chose_request(value, requests)
  -- TODO: replace this with binary search
  i = 1
  while requests[i].probability_interval_position < value do
    i = i+1
  end
  return requests[i]
end

-------------------------------------------------------------------------------
-- Helper functions for ANM experiments ---------------------------------------
-------------------------------------------------------------------------------

function switch_robots(a, b)
  -- get neighbors for both robots and delete old edges
  local na = {}
  local nb = {}
  for i, edge in ipairs(WorldInformation.get_edges(a, "all")) do
    local marker = WorldInformation.get_edge_information(edge)
    if not marker:has_key("optimal") then 
      local tail = WorldInformation.get_tail(edge)
      local head = WorldInformation.get_head(edge)
      if tail ~= a then
	na[#na+1] = tail
      else
	na[#na+1] = head
      end
      WorldInformation.remove_edge(edge)
    end
  end
  for i, edge in ipairs(WorldInformation.get_edges(b, "all")) do
    local marker = WorldInformation.get_edge_information(edge)
    if not marker:has_key("optimal") then
      local tail = WorldInformation.get_tail(edge)
      local head = WorldInformation.get_head(edge)
      if tail ~= b then
	nb[#nb+1] = tail
      else
	nb[#nb+1] = head
      end
      WorldInformation.remove_edge(edge)
    end
  end
  -- switch positions
  local pa = WorldInformation.get_robot_position(a)
  local pb = WorldInformation.get_robot_position(b)
  WorldInformation.set_robot_position(a, pb)
  WorldInformation.set_robot_position(b, pa)
  -- create edges
  for i, id in ipairs(nb) do
    if id ~= a then
      WorldInformation.add_edge(a, id, "undirected")
    end
  end
  for i, id in ipairs(na) do
    if id ~= b then
      WorldInformation.add_edge(b, id, "undirected")
    end
  end
  WorldInformation.add_edge(a, b, "undirected")
end

function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second
  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  -- are a and b neighors in the current tree
  if hop_distance(pa, pb, shuffled) == 1 then
    return
  end
  -- get neighbor position (heap indices) on path
  local pna = get_next(pa, pb, heap)
  -- get neighbors (robot IDs) on path
  local na = heap[pna]
  -- switch a and neighbor of a
  heap[pa], heap[pna] = heap[pna], heap[pa]
  posmap[a], posmap[na] = posmap[na], posmap[a]
  switch_robots(a, na)
end

-------------------------------------------------------------------------------
-- Setup of a new ANM tree experiment -----------------------------------------
-------------------------------------------------------------------------------

function setup_anm_tree(depth)
  -- 1) create the 'optimal' solution
  local opt = {}
  for i = 1, 2^depth-1 do
    opt[i] = i
  end
  
  -- 2) create the request set
  requests = {}
  weight = 0
  for i = 1, #opt do
    for j = 1, #opt do
      if hop_distance(opt[i], opt[j], opt) == 1 then 
	local k = #requests+1
	requests[k] = {}
	requests[k].first = opt[i]
	requests[k].second = opt[j]
	requests[k].weight = get_weight(hop_distance(requests[k].first, requests[k].second, opt))
	weight = weight + requests[k].weight
      end
    end
  end
  
  probability = 0
  for i = 1, #requests do
    requests[i].probability = requests[i].weight / weight
    probability = probability + requests[i].probability
    requests[i].probability_interval_position = probability
  end
  
  
  -- 4) shuffle heap
  shuffled = {}
  for i, value in ipairs(opt) do
    shuffled[i] = value
  end
  table.shuffle(shuffled)

  -- 5) create mapping id -> index (in shuffled array)
  posmap = {}
  for i = 1, #shuffled do
    posmap[shuffled[i]] = i
  end
  
  -- 6) create the actual robots
  for i = 1, #shuffled do
    local d = math.floor(math.log(i) / math.log(2))
    local color = math.floor(math.log(shuffled[i]) / math.log(2))
    local marker = MarkerInformation()
    local pos = Vector3d((position(i)-0.5)*depth*1.5, -d, 0)
    marker:add_data(":color", color)
    WorldInformation.add_robot(shuffled[i], pos, "SimpleRobot", marker)
  end
  
  -- 7) create optimal edges
  local marker = MarkerInformation()
  marker:add_data("optimal", true)
  marker:add_data(":color", 6)
  for i = 1, #opt do
    if lc(i, shuffled) then
      WorldInformation.add_edge(opt[i], opt[lc(i, opt)], marker, "undirected")
    end
    if rc(i, shuffled) then
      WorldInformation.add_edge(opt[i], opt[rc(i, opt)], marker, "undirected")
    end
  end
  
  -- 8) create shuffled edges
  for i = 1, #shuffled do
    if lc(i, shuffled) then
      WorldInformation.add_edge(shuffled[i], shuffled[lc(i, shuffled)], "undirected")
    end
    if rc(i, shuffled) then
      WorldInformation.add_edge(shuffled[i], shuffled[rc(i, shuffled)], "undirected")
    end
  end
end


function main()
  if status == "SETUP" then
    setup_anm_tree(7)
    status = "ANM"
  else
    local request = chose_request(math.random(), requests)
    handle_request(request, shuffled)
  end
end

