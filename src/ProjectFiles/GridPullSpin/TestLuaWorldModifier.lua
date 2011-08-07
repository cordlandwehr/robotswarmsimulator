function main()
	t = WorldInformation.get_robots()
	for i=1,#t do
	  print("ID: "..t[i])
	  m = WorldInformation.get_robot_information(t[i])
	  m:add_data("test", 42)
	  m:add_data("muh", 23)
	  m:add_data("blub", 7)
	  k = m:get_keys()
	  for j=1,#k do
	    print(k[j])
	  end
	end
	print("\nLua Script: 'Hello World!'", t)
end
