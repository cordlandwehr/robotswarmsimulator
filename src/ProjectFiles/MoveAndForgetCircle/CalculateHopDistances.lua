hopdistances_overall = {}
history = {}

interval = 32
interval_round = 0
file_path = "maf_circle_stats_%s.dat"

function setup() 
  -- use ISO timestamp [Year][Month][Day]T[Hours][Minutes][Seconds]
  timestamp = os.date("%Y%m%dT%H%M%S")
  -- create new file and write header
  local handler = assert(io.open(file_path:format(timestamp), "w"))
  handler:close()
  
  handler = assert(io.open("plot.plt", "w"))
  handler:write(
  "set pointsize 0.5\n"..
  "set terminal pdf\n" ..
  "set hidden3d\n"..
  "set view 50, 140\n"..
  "set output 'length_distribution.pdf'\n" ..
  "splot '".. file_path:format(timestamp) .."' using 1:2:3 with lines")
  handler:close()
end

function main()
  if WorldInformation.get_time() == 0 then
    log("info", "Setting up output file for statistics")
    setup()
    robots = WorldInformation.get_robots()
    for i = 1, #robots/2 + 1 do
      hopdistances_overall[i] = 0
    end
  end

  if WorldInformation.get_time() % 8 ~= 0 then
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
    log("info", "No long range links found, exiting statistics")
    return
  end
  
  --for key, value in pairs(bla) do end
  --for index, values in ipairs(bla) do end
  
  -- calculate hop distance of long range links
  log("debug", "Calculating hopdistances of " .. #long_range_links .. " long range links")
  hopdistances_round = {}
  robots = WorldInformation.get_robots()
  for i = 1, #robots/2 + 1 do
    hopdistances_round[i] = 0
  end
  
  for i = 1, #long_range_links do
    -- get head and tail of link
    head = WorldInformation.get_head(long_range_links[i])
    tail = WorldInformation.get_tail(long_range_links[i])
    -- calculate distance without long range links
    dist = Statistics.calculate_hop_distance(tail, head, long_range_links)
    dist = dist + 1
    -- store distance
    hopdistances_round[dist] = hopdistances_round[dist] + 1
    hopdistances_overall[dist] = hopdistances_overall[dist] + 1
  end
  
  -- store in gnuplot file
  if interval_round == interval then
    interval_round = 0
  
    log("debug", "Saving hopdistances...")
    local handler = assert(io.open(file_path:format(timestamp), "a"))
    for i = 1, #hopdistances_overall do
      handler:write(WorldInformation.get_time() .. " " .. i .. " " .. hopdistances_overall[i] .. "\n")
    end
    handler:write("\n")
    for i = 1, #hopdistances_overall do
      handler:write(WorldInformation.get_time()+0.01 .. " " .. i .. " " .. -1 .. "\n")
    end
    handler:write("\n\n")
    handler:close()
    
    robots = WorldInformation.get_robots()
    for i = 1, #robots/2 + 1 do
      hopdistances_overall[i] = 0
    end
  else
    interval_round = interval_round + 1
  end
end