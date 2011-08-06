function main()
	t = WorldInformation.get_robots()
	for i=1,#t do
	  print(t[i])
	end
	t = WorldInformation.get_markers()
	for i=1,#t do
	  print(t[i])
	end
	print("\nLua Script: 'Hello World!'", t)
end
