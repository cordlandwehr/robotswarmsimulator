function main()
  if WorldInformation.get_time() % 8 == 0 then
    log("info", "Skipping statistics in round " .. WorldInformation.get_time())
    return
  end
    
  log("info", "Starting statistics in round " .. WorldInformation.get_time())
  
  -- get long range links
  log("debug", "Getting long range links")
  edges = WorldInformation.get_edges("all")
  long_range_links = {}
  for i = 1 , #edges do
    edge_marker = WorldInformation.get_edge_information(edges[i])
    if edge_marker:has_key("long_range_link") then
      table.insert(long_range_links, edges[i])
    end
  end
  
  --for key, value in pairs(bla) do end
  --for index, values in ipairs(bla) do end
  
  -- calculate hop distance of long range links
  log("debug", "Calculating hopdistances of " .. #long_range_links .. " long range links")
  hopdistances = {}
  for i = 1, #long_range_links do
    log("debug", "Getting head and tail of link " .. long_range_links[i])
    head = WorldInformation.get_head(long_range_links[i])
    tail = WorldInformation.get_tail(long_range_links[i])
    log("debug", "Calculating hopdistance from " .. tail .. " to " .. head)
    dist = Statistics.calculate_hop_distance(tail, head, long_range_links)
    table.insert(hopdistances, dist)
    log("debug", "Calculated hopdistance from " .. tail .. " to " .. head .. " as " .. dist)
  end
  
  -- print hopdistances
  log("info", "calculated " .. #hopdistances .. " hopdistances:")
  for i = 1, #hopdistances do
    log("info", "distance[" .. i .. "]=" .. hopdistances[i])
  end
end