function print_edges()
  edges = WorldInformation.get_edges("all")
  directed_edges = WorldInformation.get_edges("directed")
  undirected_edges = WorldInformation.get_edges("undirected")
  
  counter = 0
  for i = 1, #edges do
    id = edges[i]
    marker = WorldInformation.get_edge_information(id)
    if #marker:get_keys() > 0 then
      counter = counter + 1
    end
  end
  
  output = "Edge overview:\nDirected edges: #%d\nUndirected edges: #%d\nAll edges: #%d\n\nWith keys: #%d\n"
  print(output:format(#directed_edges, #undirected_edges, #edges, counter))
end

function main()
  -- say who I am ;-)
  print("\nEdgeTest.lua:\n")
  
  bots = WorldInformation.get_robots()
  edges = WorldInformation.get_edges("all")
  
  if #edges > 0 then
    print("There are edges... \n")
    print_edges()
    print("...attempting removal\n")
    for i = 1, #edges do
      id = edges[i]
      WorldInformation.remove_edge(id)
    end
    print_edges()
  else
    print("There are no edges...\n")
    print_edges()
    print("...creating list (using different edge types).\n")
    for i = 1, (#bots-1) do
      ida = bots[i]
      idb = bots[i+1]
      if (i % 4 == 0) then
	WorldInformation.add_edge(ida, idb, "directed")
      elseif (i % 4 == 1) then
	WorldInformation.add_edge(ida, idb, "undirected")
      elseif (i % 4 == 2) then
	marker = MarkerInformation()
	marker:add_data("test_key", 23)
	WorldInformation.add_edge(ida, idb, marker, "directed")
      else
	marker = MarkerInformation()
	marker:add_data("test_key", 42)
	WorldInformation.add_edge(ida, idb, marker, "undirected")
      end
    end
    print_edges()
  end
end
