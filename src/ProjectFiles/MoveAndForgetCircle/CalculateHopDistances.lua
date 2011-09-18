hopdistances_overall = {}

function main()
  if WorldInformation.get_time() == 0 then
    log("info", "Setting up output file for statistics")
    io.output("maf_circle_stats.csv")
    robots = WorldInformation.get_robots()
    for i = 1, #robots/2 do
      hopdistances_overall[i] = 0
    end
  end

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
  
  if #long_range_links == 0 then
    log("info", "no long range links found, exiting statistics")
    return
  end
  
  --for key, value in pairs(bla) do end
  --for index, values in ipairs(bla) do end
  
  -- calculate hop distance of long range links
  log("debug", "Calculating hopdistances of " .. #long_range_links .. " long range links")
  hopdistances_round = {}
  robots = WorldInformation.get_robots()
  for i = 1, #robots/2 do
    hopdistances_round[i] = 0
  end
  for i = 1, #long_range_links do
    -- get head and tail of link
    head = WorldInformation.get_head(long_range_links[i])
    tail = WorldInformation.get_tail(long_range_links[i])
    -- calculate distance without long range links
    dist = Statistics.calculate_hop_distance(tail, head, long_range_links)
    -- store distance
    hopdistances_round[dist] = hopdistances_round[dist] + 1
    hopdistances_overall[dist] = hopdistances_overall[dist] + 1
  end
  
  -- store new hopdistances_overall in file
  log("debug", "Saving hopdistances...")
  for i = 1, #hopdistances_overall do
    if i < #hopdistances_overall then
      io.write("" .. hopdistances_overall[i] .. ",")
    else 
      io.write("" .. hopdistances_overall[i] .. "\n")
    end
  end
end