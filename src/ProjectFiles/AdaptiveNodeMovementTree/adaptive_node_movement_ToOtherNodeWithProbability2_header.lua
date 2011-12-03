dofile(WorldInformation.get_project_path() .. "/anm_generics.lua")
dofile(WorldInformation.get_project_path() .. "/includes.lua")

probability = arg1
mean = arg2
s = arg3
dg = DistributionGenerator(1)

function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second

  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  
--  log("info", "Request between " .. a .. " and " .. b .. ", positioned at " .. pa .. " and " .. pb .. ", respectively")

  local aborted = false

  local steps = hop_distance(pa, pb, heap) * dg.get_value_normal(dg)

  -- are a and b neighbors in the current tree
  while steps > 1 do
    -- get neighbor position (heap indices) on path
    local pna = get_next(pa, pb, heap)
    -- get neighbors (robot IDs) on path
    local na = heap[pna]
    -- switch a and neighbor of a
    switch_robots(a, na, heap)

    -- update positions for they could have changed
    pa = posmap[a]
    pb = posmap[b]
    steps = steps -1  
  end
end


function get_weight(distance)
  -- the larger the return value the more likely the selection of the request
  if distance > 1 then
    return 0
  else
    return 1/distance
  end
end

function localSetup(depth)
  dg.init_normal(dg, mean, s)
end


function main()
generic_main("ToOtherNodeWithProbability2_m25_s1", 4, get_weight, handle_request, true, localSetup)
end
