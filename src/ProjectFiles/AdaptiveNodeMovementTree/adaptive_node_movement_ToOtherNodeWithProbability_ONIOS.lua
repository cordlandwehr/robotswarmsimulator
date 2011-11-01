dofile(WorldInformation.get_project_path() .. "/anm_generics.lua")

probability = 0.9

function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second

  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  
--  log("info", "Request between " .. a .. " and " .. b .. ", positioned at " .. pa .. " and " .. pb .. ", respectively")

  local aborted = false

  -- are a and b neighbors in the current tree
  while hop_distance(pa, pb, heap) > 1 and not aborted do
    if not (math.random(100) <= probability * 100) then
      aborted = true
    else 
      -- get neighbor position (heap indices) on path
      local pna = get_next(pa, pb, heap)
      -- get neighbors (robot IDs) on path
      local na = heap[pna]
      -- switch a and neighbor of a
      switch_robots(a, na, heap)

      -- update positions for they could have changed
      pa = posmap[a]
      pb = posmap[b]  
    end
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

function main()
  generic_main("toOtherNodeWithProbability_ONIOS", 7, get_weight, handle_request, false)
end

