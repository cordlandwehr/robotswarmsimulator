function main()
	t = WorldInformation.get_robots()
	for i=1,#t do
	  print("ID: "..t[i])
	end
	t = WorldInformation.get_robots()
	for i=1,#t do
	  print("ID: "..t[i])
	end
	print("\nLua Script: 'Hello World!'", t)
end
