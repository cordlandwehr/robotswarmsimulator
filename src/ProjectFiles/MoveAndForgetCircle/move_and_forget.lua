function print_edges()
  -- print all edges
  edges = View.get_visible_edges()
  me = View.get_own_id()
  log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Edge overview:")
  for i = 1, #edges do
    local id = edges[i]
    if View.is_directed(id) then
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Directed edge from " .. View.get_tail(id) .. " to " .. View.get_head(id) .. ".")
    else
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Undirected edge between " .. View.get_tail(id) .. " and " .. View.get_head(id) .. ".")
    end    
  end
end

function main()
  -- get own ID and say who I am
  local me = View.get_own_id()
  log("debug", "Running move_and_forget.lua ... (Robot #" .. me .. ").")
  
  -- get marker (initialize if empty)
  my_marker = View.get_robot_information(me)
  if (#my_marker:get_keys() == 0) then
    my_marker:add_data("status", "ask")
  end
  
  -- print my edges
  print_edges()
  
  -- check my status
  if my_marker:get_data("status") == "ask" then
    -- this is an ASK round
    log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Ask round.") 
    my_marker:add_data("status", "processing")
  else
    -- this is PROCESSING round
    log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Processing round.") 
    my_marker:add_data("status", "ask")
  end
  
  -- create request to update my own marker
  View.add_marker_request(my_marker)
end