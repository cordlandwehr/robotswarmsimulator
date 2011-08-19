function main()
  -- say who I am ;-)
  log("debug", "Running MarkerInforamtionTest.lua ...")
  -- update bots
  bots = WorldInformation.get_robots()
  now = WorldInformation.get_time()
  key_string = "Key_%02d"
  for i = 1, #bots do
    id = bots[i]
    marker = WorldInformation.get_robot_information(id)
    marker:add_data(key_string:format(now % 3), now)
    marker:add_data("bool_test", true)
    marker:add_data("number_test", 1/3)
    marker:add_data("string_test", "Hello World!")
    WorldInformation.set_robot_information(id, marker)
  end
  -- dump bot information
  log("debug", "[MarkerInforamtionTest.lua] There are #" .. #bots .. " robots.")
  if #bots > 1 then
    log("debug", "[MarkerInforamtionTest.lua] Dumping robot information for first robot (id=" .. id .. ").")
    id = bots[1]
    marker = WorldInformation.get_robot_information(id)
    keys = marker:get_keys()
    for j = 1, #keys do
      log("debug", "[MarkerInforamtionTest.lua] Key: " .. keys[j] .. ", Value: " .. tostring(marker:get_data(keys[j])) .. ".")
    end
  end
end