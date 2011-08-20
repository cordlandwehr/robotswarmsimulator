function print_messages()
  messages = WorldInformation.get_messages()
  bots = WorldInformation.get_robots()
  
  min_msg = #messages
  max_msg = 0 
  for i = 1, #bots do
    id = bots[i]
    bot_msg = WorldInformation.get_messages(id)
    if #bot_msg < min_msg then
      min_msg = #bot_msg
    elseif #bot_msg > max_msg then
      max_msg = #bot_msg
    end
  end
  
  log("debug", "[MessageTest.lua] Message overview:")
  log("debug", "[MessageTest.lua] Messages: #" .. #messages)
  log("debug", "[MessageTest.lua] Min. messages per robot: #" .. min_msg)
  log("debug", "[MessageTest.lua] Max. messages per robot: #" .. max_msg)
  log("debug", "[MessageTest.lua] Avg. messages per robot: #" .. (#messages / #bots))
end

function main()
  -- say who I am
  log("debug", "Running MessageTest.lua ...")
  print_messages()
  -- randomly delete and generate messages
  messages = WorldInformation.get_messages()
  if #messages == 0 then
    n = 0
  else
    n = math.random(#messages)
  end
  log("debug", "[MessageTest.lua] Deleting " .. n .. " random messages.")
  for i = 1, n do
    index = math.random(#messages)
    WorldInformation.remove_message(messages[index])
    table.remove(messages, index)
  end
  bots = WorldInformation.get_robots()
  n = math.random(#bots)
  log("debug", "[MessageTest.lua] Creating " .. n .. " random messages.")
  for i = 1, n do
    sender = math.random(#bots)
    receiver = math.random(#bots)
    marker = MarkerInformation()
    WorldInformation.add_message(bots[sender], bots[receiver], marker)
  end
  print_messages()
end
