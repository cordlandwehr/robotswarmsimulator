long_range_link = nil
neighbors = nil
me = nil

function handle_message(msg)
  -- get message marker
  local marker = View.get_message_information(msg)
  assert(marker:has_key("type"))
  local msg_type = marker:get_data("type")
  -- process actual message
  if msg_type == "get_neighbor" then
    
  elseif msg_type == "tell_neighbor" then
    
  else
    log("error", "[move_and_forget.lua][Robot #" .. me .. "] Unsupported message type!")
  end
  -- remove the message
  View.add_remove_message_request(msg)
end

function print_edges()
  -- print all edges
  local edges = View.get_visible_edges()
  log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Edge overview:")
  for i, edge in ipairs(edges) do
    if View.is_directed(edge) then
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Directed edge from " .. View.get_tail(edge) .. " to " .. View.get_head(edge) .. ".")
    else
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Undirected edge between " .. View.get_tail(edge) .. " and " .. View.get_head(edge) .. ".")
    end    
  end
end

function update_links()
  -- update my own ID (should not change in between rounds, but who knows)
  me = View.get_own_id()
  -- check visible edges (search for long range link)
  neighbors = nil
  long_range_link = nil
  local edges = View.get_visible_edges()
  for i, edge in ipairs(edges) do
    local marker = View.get_edge_information(edge)
    if marker:has_key("long_range_link") then
      -- found long range link
      long_range_link = edge
    else
      -- edge is no long range link, create new neighbor set or add to existing
      if not neighbors then
	neighbors = {edge}
      else
	neighbors[#neighbors+1] = edge
      end
    end
  end
end

function main()
  -- update links / IDs
  update_links()
  
  -- say who I am
  log("debug", "Running move_and_forget.lua ... (Robot #" .. me .. ").")
  
  -- get marker (initialize if empty)
  my_marker = View.get_robot_information(me)
  if (#my_marker:get_keys() == 0) then
    my_marker:add_data("status", "ask")
  end
  
  -- print my edges
  print_edges()
  
  -- handle all messages
  for i = 1, View.get_number_of_messages() do
    -- loop goes from 1 .. n, message indices from 0 .. n-1
    handle_message(View.get_message(i-1))
  end  
  
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