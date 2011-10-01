-- this WM creates and evaluates uniform LR link distributions

min_size = 64
max_size = 1024
step_size = 64
samples = 10
done = false

timestamp = nil
file_path = "local_greedy_(%d_robots)_%s"

gnuplot = [[# gnuplot script for hop reductions (greedy)

# configure terminal
set terminal pdf font ",4"
set output '<FILENAME>.pdf'

# labels
set xlabel 'distance'
set ylabel 'avg. reduction'

# plot mean and sd
plot '<FILENAME>.dat' using 1:2:3 title 'mean reduction' with errorbars
  
]]

function main()
  --check whether we are already done
  if done then
    return
  end
  
  -- use ISO timestamp [Year][Month][Day]T[Hours][Minutes][Seconds]
  timestamp = os.date("%Y%m%dT%H%M%S")
    
  -- simulate for different rig sizes and distributions
  for size = min_size, max_size, step_size do
    -- say what we want to do here
    log("info", "Remove all existing and create " .. size .. " new robots.")
    
    -- create files
    local handler = assert(io.open(file_path:format(size, timestamp) .. ".dat", "w"))
    handler:close()
    local script = gnuplot:gsub("<FILENAME>", file_path:format(size, timestamp))
    local handler = assert(io.open(file_path:format(size, timestamp) .. ".plt", "w"))
    handler:write(script)
    handler:close()
    
    -- remove all existing robots
    local robots = WorldInformation.get_robots()
    for i, robot in ipairs(robots) do
      WorldInformation.remove_robot(robot)
    end
    
    -- create 'size' new robots
    for i = 1, size do
      WorldInformation.add_robot(i-1, Vector3d(i/32,i/32,0), "SimpleRobot")
    end
    
    log("info", "There are " .. #(WorldInformation.get_robots()) .. " robots.")
    
    -- create ring
    for i = 1, size do
      local id = i-1
      WorldInformation.add_edge(id, (id+1)%size, "undirected")
    end
    
    --create tables for storing results
    sample_results = {}
    
    for s = 1, samples do
      -- say what we are doing
      log("info", "Computing smaple #" .. s .. ".")
      
      -- create and evaluate 'samples' uniform long range link distributions
      local long_range_links = {}
      
      -- generate onelong range link per robot
      for i = 1, size do
	local id = i-1
	local marker = MarkerInformation()
	marker:add_data("long_range_link", true)
	local new_id = WorldInformation.add_edge(id, math.random(0, size-1), marker, "undirected")
	long_range_links[new_id] = true
      end
      
      -- evaluate configuration
      local results = Statistics.evaluate_uniform_lrlink_distribution()
      log("info", "Result set has length " .. #results .. ".")
      for d = 3, #results, 2 do
	--log("info", "d = " .. (d-3)/2+2 .. ", m = " .. results[d] .. ".")
	ld = (d-3)/2+2
	if sample_results[ld] == nil then
	  sample_results[ld] = {results[d]}
	else
	  sample_results[ld][#(sample_results[ld])+1] = results[d]
	end
      end
      
      -- remove all long range links
      for edge, flag in pairs(long_range_links) do
	WorldInformation.remove_edge(edge)
      end
    end
    
    for d, table in pairs(sample_results) do
     -- calculate mean for current distance
     mean = 0.0
     for i, value in ipairs(table) do
       mean = mean + value
     end
     mean = mean / (#table)
     
     -- calculate sd for current distance
     sd = 0.0
     for i, value in ipairs(table) do
       sd = sd + (value-mean)*(value-mean)
     end
     sd = sd / (#table - 1)
     
     -- write to file
     local handler = assert(io.open(file_path:format(size, timestamp) .. ".dat", "a"))
     row = "%d\t%.3f\t%.3f\n"
     handler:write(row:format(d, mean, sd))
     handler:close()
    end
    
  end
    
  -- do not repeat the computations above
  done = true
end