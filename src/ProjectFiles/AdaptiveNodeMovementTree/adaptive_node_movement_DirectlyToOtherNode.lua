dofile(WorldInformation.get_project_path() .. "/anm_generics.lua")

function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second
  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  -- are a and b neighors in the current tree

  while hop_distance(pa, pb, heap) > 1 do
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


function get_weight(distance)
  -- the larger the return value the more likely the selection of the request
  if distance > 1 then
    return 0
  else
    return 1/distance
  end
end

function main()
  if status == "SETUP" then
    setup_anm_tree(7, get_weight)
    status = "ANM"
  else
    local request = chose_request(math.random(), requests)
    handle_request(request, shuffled)
    update_potential()
  end
end

