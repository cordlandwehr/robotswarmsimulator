status = "SETUP"
opt = nil
shuffled = nil
requests = nil
posmap = nil
projectname = "undefined"
levels = nil
cost = 0

-- IO settings
file_prefix = WorldInformation.get_project_path() .. "/output/"
file_name = "anm_%s_%sphi"
file_path = file_name .. ".dat"
file_path_plt = file_name .. ".plt"
file_name_cost = "anm_%s_%scost"
file_path_cost = file_name_cost .. ".dat"
file_path_plt_cost = file_name_cost .. ".plt"
file_header = "Time\tPhi\n"
file_header_cost = "Time\tCost\n"
file_data = "%d\t%d\n"
gnuplot_terminal = "pdf"
timestamp = nil


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

function chose_request(value, requests)
  -- TODO: review if this binary search is ok
  local min = 1
  local max = #requests
  local mid = min + math.floor((max-min)/2)
  local found = false

  while (found == false) do
    if (min == max) then
      return requests[min]
    end

    if requests[mid].probability_interval_position < value then
      min = mid + 1
      mid = min + math.floor((max-min)/2)
    else 
      max = mid
      mid = min + math.floor((max-min)/2)
    end
  end
end


-------------------------------------------------------------------------------
-- The potential function ------------------------------
-------------------------------------------------------------------------------
function phi()
  local potential = 0
  for i = 1, #opt do
    if lc(i, opt) then
      potential = potential + hop_distance(shuffled[i], shuffled[lc(i, opt)], shuffled) - 1
    end
    if rc(i, opt) then
      potential = potential + hop_distance(shuffled[i], shuffled[rc(i, opt)], shuffled) - 1
    end
  end
  
  return potential
end


function update_potential()
  local marker = WorldInformation.get_robot_information(shuffled[1])
  marker:add_data("potential", phi())
  WorldInformation.set_robot_information(shuffled[1], marker)
end

function update_cost(request, heap)
  -- IDs
  local a = request.first
  local b = request.second
  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  -- are a and b neighors in the current tree
  cost = cost + hop_distance(pa, pb, heap) - 1
end

-------------------------------------------------------------------------------
-- IO helper functions --------------------------------------------------------
-------------------------------------------------------------------------------

function setup_IO()
  -- use ISO-like timestamp [Year]-[Month]-[Day]T[Hours]-[Minutes]-[Seconds]
  timestamp = os.date("%Y-%m-%dT%H-%M-%S")

  --potential files:
  -- create new file and write header
  local handler = assert(io.open(file_prefix .. file_path:format(projectname,timestamp), "w"))
  handler:write(file_header)
  -- calculate data row for start configuration
  handler:write(file_data:format(WorldInformation.get_time(), phi()))	
  handler:close()

  -- create the .plt file for convenience
  local plt_handler = assert(io.open(file_prefix .. file_path_plt:format(projectname,timestamp), "w"))
  plt_handler:write("set terminal " .. gnuplot_terminal .. "\n")
  plt_handler:write("set output '" .. file_name:format(projectname,timestamp) .. "." .. gnuplot_terminal .. "'" .. "\n")
  plt_handler:write("plot '" .. file_path:format(projectname,timestamp) .. "' using 1:2 with lines" .. "\n")
  plt_handler:close()

  -- cost files:
  -- create new file and write header
  local handler = assert(io.open(file_prefix .. file_path_cost:format(projectname,timestamp), "w"))
  handler:write(file_header_cost)
  -- calculate data row for start configuration
  handler:write(file_data:format(WorldInformation.get_time(), cost))	
  handler:close()

  -- create the .plt file for convenience
  local plt_handler = assert(io.open(file_prefix .. file_path_plt_cost:format(projectname,timestamp), "w"))
  plt_handler:write("set terminal " .. gnuplot_terminal .. "\n")
  plt_handler:write("set output '" .. file_name_cost:format(projectname,timestamp) .. "." .. gnuplot_terminal .. "'" .. "\n")
  plt_handler:write("plot '" .. file_path_cost:format(projectname,timestamp) .. "' using 1:2 with lines" .. "\n")
  plt_handler:close()

end

function write_potential()
  -- write potential to .dat file - note that update_potentaiL() is not called automatically here
  local handler = assert(io.open(file_prefix .. file_path:format(projectname,timestamp), "a"))
  -- write current potential
  handler:write(file_data:format(WorldInformation.get_time(), phi()))
  handler:close() 
end

function write_cost()
  -- write potential to .dat file - note that update_potentaiL() is not called automatically here
  local handler = assert(io.open(file_prefix .. file_path_cost:format(projectname,timestamp), "a"))
  -- write current cost
  handler:write(file_data:format(WorldInformation.get_time(), cost))
  handler:close() 
end

-------------------------------------------------------------------------------
-- Helper functions for ANM experiments ---------------------------------------
-------------------------------------------------------------------------------

function switch_robots(a, b, heap)

  if posmap[a] == 1 or posmap[b] == 1 then
    local marker = WorldInformation.get_robot_information(heap[1])
    marker:remove_data("potential")
    WorldInformation.set_robot_information(heap[1], marker)
    --marker:add_data(":color", color)
  end

  local pa = posmap[a]
  local pna = posmap[b]


  heap[pa], heap[pna] = heap[pna], heap[pa]
  posmap[a], posmap[b] = posmap[b], posmap[a]

  if posmap[a] == 1 or posmap[b] == 1 then
    local marker = WorldInformation.get_robot_information(heap[1])
    marker:add_data("potential", phi())
    WorldInformation.set_robot_information(heap[1], marker)
  end


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


-------------------------------------------------------------------------------
-- Setup of a new ANM tree experiment -----------------------------------------
-------------------------------------------------------------------------------

function setup_anm_tree(depth, weightFunc, projectName, onlyNeighborsInOptimalSolution)
  levels = depth

  -- 1) create the 'optimal' solution
  opt = {}
  for i = 1, 2^depth-1 do
    opt[i] = i
  end
  
  -- 2) create the request set
  requests = {}
  weight = 0
  for i = 1, #opt do
    for j = 1, #opt do
      if (i ~= j) and ((onlyNeighborsInOptimalSolution == false) or (hop_distance(opt[i], opt[j], opt) == 1)) then 
	local k = #requests+1
	requests[k] = {}
	requests[k].first = opt[i]
	requests[k].second = opt[j]
	requests[k].weight = weightFunc(hop_distance(requests[k].first, requests[k].second, opt))
	if requests[k].weight == 0 then
	  requests[k] = nil
	else
	  weight = weight + requests[k].weight
	end
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
    if i == 1 then
      marker:add_data("potential", phi())
    end
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

  -- 9) setup IO
  if projectName ~= nil then
    projectname = projectName
    setup_IO()
  end
end

function generic_main(name, depth, weightFunc, handleFunc, onlyNeighborsInOptimalSolution, localSetup)
  if status == "SETUP" then
    setup_anm_tree(depth, weightFunc, name, onlyNeighborsInOptimalSolution)
    status = "ANM"
    if localSetup ~= nil then
      localSetup(depth)
    end
  else
    local request = chose_request(math.random(), requests)
    update_cost(request, shuffled)
    handleFunc(request, shuffled)
    update_potential()
    write_potential()
    write_cost()
  end
end
