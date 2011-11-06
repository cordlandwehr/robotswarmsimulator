dofile(WorldInformation.get_project_path() .. "/anm_generics.lua")

threshold = 5

memory = {}

function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second

  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  
  --log("info", "Request between " .. a .. " and " .. b .. ", positioned at " .. pa .. " and " .. pb .. ", respectively")

  local pna = get_next(pa, pb, heap)
  local over = false

  --log("info", "pna " .. pna .. ", parent(a): " .. parent(pa, heap) .. ", lc(a): " .. lc(pa, heap) .. ", rc(a): " .. rc(pa, heap) )

  --update local threshold values
  if pna == parent(pa, heap) then
    memory[a].p = memory[a].p + 1
    memory[a].l = 0
    memory[a].r = 0
    if memory[a].p >= threshold then
      over = true  
    end
  elseif pna == lc(pa, heap) then
    memory[a].p = 0
    memory[a].l = memory[a].l + 1
    memory[a].r = 0    
    if memory[a].l >= threshold then
      over = true
   end
  elseif pna == rc(pa, heap) then
    memory[a].p = 0
    memory[a].l = 0
    memory[a].r = memory[a].r + 1    
    if memory[a].r >= threshold then
      over = true
   end
  end

  -- if threshold exceeded, move node
  if (over == true) then 
    -- move a to b
    while hop_distance(pa, pb, heap) > 1 do
      -- get neighbor position (heap indices) on path
      local ppna = get_next(pa, pb, heap)
      -- get neighbors (robot IDs) on path
      local na = heap[ppna]
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

function localSetup(depth)
  --memory = {}
  for i = 1, 2^depth-1 do
    memory[i] = {}
    memory[i].p = 0
    memory[i].l = 0
    memory[i].r = 0
  end
end

function main()
  generic_main("directlyToOtherNodeWithOneWayThreshold_ONIOS", 7, get_weight, handle_request, true, localSetup)
end

