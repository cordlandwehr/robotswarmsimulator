function main()
  -- say who I am ;-)
  print("\nMarkerInformationTest.lua:\n")
  -- update bots
  bots = WorldInformation.get_robots()
  now = WorldInformation.get_time()
  key_string = "Key_%02d"
  for i = 1, #bots do
    id = bots[i]
    marker = WorldInformation.get_robot_information(id)
    --marker:add_data(key_string:format(now % 3), now)
    WorldInformation.set_robot_information(id, marker)
  end
  -- dump bot information
  for i = 1, #bots do
    id = bots[i]
    print("Dumping robot information for ID " .. id .. ":\n")
    marker = WorldInformation.get_robot_information(id)
    keys = marker:get_keys()
    for j = 1, #keys do
      print("Key: " .. keys[j] .. ", Value: " .. marker:get_data(keys[j]) .. ".\n")
    end
  end
end