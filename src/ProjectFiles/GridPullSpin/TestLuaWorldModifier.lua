function main()
	t = WorldInformation.get_robots()
	for i=1,#t do
	  print("ID: "..t[i])
	  m = WorldInformation.get_robot_information(t[i])
	  k = m:get_keys()
	  for j=1,#k do
	    print(k[j])
	  end
	  if (#k == 0) then 
	    m:add_data("test", 42)
	    m:add_data("muh", 23)
	    m:add_data("blub", 7)
	  end
	  --WorldInformation.add_marker_request(t[i], m)
	end
end
