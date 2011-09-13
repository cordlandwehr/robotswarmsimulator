-- global settings
timeout = 256

function main()
  -- get own marker
  local id = View.get_own_id()
  local marker = View.get_robot_information(id)
  
  -- initialize marker if 'waiting' key is unknown, reset values if waiting for too long
  if not marker:has_key("waiting") then
    log("info", "[example_robot.lua][Robot #" .. id .. "] Initializing my marker.")
    marker:add_data("waiting", false)
  elseif marker:get_data("waiting") then
    local duration = View.get_time() - marker:get_data("waiting_since")
    if duration >= timeout then
      log("error", "[example_robot.lua][Robot #" .. id .. "] Timeout, resetting my marker.")
      marker:add_data("waiting", false)
    end
  end
  
  -- get list of neighbors
  local neighbors = View.get_visible_robots()
  if #neighbors == 0 then
    log("error", "[example_robot.lua][Robot #" .. id .. "] I see no neighbors!")
  end
  
  -- process all incoming messages
  for i = 1, View.get_number_of_messages() do
    -- loop goes from 1 .. n, message indices from 0 .. n-1
    local message = View.get_message(i-1)
    
    -- get message marker and contained counter
    local message_marker = View.get_message_information(message)
    local hop_counter = message_marker:get_data("hop_counter")
    
    -- check whether this robot initiated the message chain
    if message_marker:get_data("requesting_robot") == id then
      -- output result and change status
      log("info", "[example_robot.lua][Robot #" .. id .. "] Counted " .. hop_counter .. " hops.")
      marker:add_data("waiting", false)
    else
      -- increase hop counter and send message to first (and only) neighbor
      message_marker:add_data("hop_counter", hop_counter+1)
      View.add_send_message_request(neighbors[1], message_marker)
    end
    
    -- request removal of handled message
    View.add_remove_message_request(message)
  end
  
  -- if not already waiting for a message, send a new one
  if not marker:get_data("waiting") then
    log("debug", "[example_robot.lua][Robot #" .. id .. "] Sending new message.")
    -- create and send message
    local message_marker = MarkerInformation()
    message_marker:add_data("requesting_robot", id)
    message_marker:add_data("hop_counter", 1)
    View.add_send_message_request(neighbors[1], message_marker)
    -- update my own marker
    marker:add_data("waiting", true)
    marker:add_data("waiting_since", View.get_time())
  end
  
  -- test special visualization marker informations
  for i = 0, 9 do
	marker:add_data(":vector" .. i, i .. "," .. math.random(1,10) .. ",3")
  end
  marker:add_data(":color", math.floor(math.random(0,9)))
  marker:add_data(":size", math.random())
  
  -- request update of own marker
  View.add_marker_request(marker)
end