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

function get_weight(depth)
  -- the larger the return value the more likely the selection of the request
  return 1/depth
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
      if i ~= j then 
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
  shuffled = table.shuffle(opt)

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
  
  -- 7) create edges
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
  tdepth = 6
  if status == "SETUP" then
    setup_anm_tree(tdepth)
    status = "ANM"
  else
    local request = chose_request(math.random(), requests)
    -- switch for first node
    local pf = posmap[request.first]
    local ps = posmap[request.second]
    local pnext = get_next(pf, ps, shuffled)
    local next = shuffled[pnext]    
    log("info", "Next: " .. next )
    
    local fmarker = WorldInformation.get_robot_information(request.first)
    local nmarker = WorldInformation.get_robot_information(next)
    log("info", "marker")
    
    -- switch heap entries and mapping
    shuffled[pf], shuffled[pnext] = shuffled[pnext], shuffled[pf]
    posmap[request.first], posmap[next] = posmap[next], posmap[request.first]
    -- get markes
    log("info", "shuffled")
    WorldInformation.remove_robot(request.first)
    WorldInformation.remove_robot(next)
    log("info", "remove")
    local d = math.floor(math.log(posmap[request.first]) / math.log(2))
    local fpos = Vector3d((position(posmap[request.first])-0.5)*tdepth*1.5, -d, 0)
    WorldInformation.add_robot(request.first, fpos, "SimpleRobot", fmarker)
    log("info", "add first")
    local d = math.floor(math.log(posmap[next]) / math.log(2))
    local npos = Vector3d((position(posmap[next])-0.5)*tdepth*1.5, -d, 0)
    WorldInformation.add_robot(next, npos, "SimpleRobot", nmarker)
    log("info", "add next")
  end
end

