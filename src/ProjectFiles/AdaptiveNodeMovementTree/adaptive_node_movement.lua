dofile "src/ProjectFiles/AdaptiveNodeMovementTree/anm_generics.lua"



function handle_request(request, heap) 
  -- IDs
  local a = request.first
  local b = request.second
  -- positions
  local pa = posmap[a]
  local pb = posmap[b]
  -- are a and b neighors in the current tree
  if hop_distance(pa, pb, heap) == 1 then
    return
  end
  -- get neighbor position (heap indices) on path
  local pna = get_next(pa, pb, heap)
  -- get neighbors (robot IDs) on path
  local na = heap[pna]
  -- switch a and neighbor of a

  switch_robots(a, na, heap)
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

