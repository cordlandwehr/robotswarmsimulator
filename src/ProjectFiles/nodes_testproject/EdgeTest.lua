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
  
  log("debug", "[EdgeTest.lua] Edge overview:")
  log("debug", "[EdgeTest.lua] Directed edges: #" .. #directed_edges)
  log("debug", "[EdgeTest.lua] Undirected edges: #" .. #undirected_edges)
  log("debug", "[EdgeTest.lua] #" .. counter .. " of #" .. #edges .. " edges have keys.")
end

function main()
  -- say who I am ;-)
  log("debug", "Running EdgeTest.lua ...")
  
  bots = WorldInformation.get_robots()
  edges = WorldInformation.get_edges("all")
  
  if #edges > 0 then
    log("debug", "[EdgeTest.lua] Detected edges...")
    print_edges()
    log("debug", "[EdgeTest.lua] ...attempting removal.")
    for i = 1, #edges do
      id = edges[i]
      WorldInformation.remove_edge(id)
    end
    print_edges()
  else
    log("debug", "[EdgeTest.lua] Detected NO edges...")
    print_edges()
    log("debug", "[EdgeTest.lua] ...creating list (using different edge types).")
    for i = 1, (#bots-1) do
      ida = bots[i]
      idb = bots[i+1]
      if (i % 4 == 0) then
	WorldInformation.add_edge(ida, idb, "directed")
      elseif (i % 4 == 1) then
	WorldInformation.add_edge(ida, idb, "undirected")
      elseif (i % 4 == 2) then
	marker = MarkerInformation()
	marker:add_data("test_key", "muh")
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
