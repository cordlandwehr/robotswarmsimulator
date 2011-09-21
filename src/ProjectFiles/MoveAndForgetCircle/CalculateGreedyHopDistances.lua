next_active = 0
file_path_temp = "maf_hop_distances_(%d_robots)_%s"
file_path = nil
max = nil
time = nil

plot = [[# gnuplot script for hop distances (greedy)

# configure terminal
set terminal pdf
set ouput '%s.pdf'

# labels
set log x
set xlabel 'time'
set ylabel 'hop distance'

# plot mean and sd
plot '%s.dat' using 1:11:12 title 'mean hop distance' with errorbars

]]

function setup() 
  -- use ISO timestamp [Year][Month][Day]T[Hours][Minutes][Seconds]
  timestamp = os.date("%Y%m%dT%H%M%S")
  -- create new file and write header
  n = #(WorldInformation.get_robots())
  -- plot file
  local handler = assert(io.open( file_path_temp:format(n, timestamp) .. ".plt", "w"))
  handler:write(plot:format(file_path_temp:format(n, timestamp), file_path_temp:format(n, timestamp)))
  handler:close()
  
  file_path = file_path_temp:format(n, timestamp) .. ".dat"
  local handler = assert(io.open(file_path, "w"))
  handler:close()
  -- calclate max
  max = 48 * n
end

function check_wait()
  time = WorldInformation.get_time() - 24
  if next_active <= time then
    if time > max then
      next_active = time + max
    else
      next_active = time + time*0.5
    end
    return true
  end
  return false
end

function main()
  if not max then
    setup()
  end
  
  if check_wait() then
    results = Statistics.evaluate_first_mf_experiment()
    local handler = assert(io.open(file_path, "a"))
    handler:write("" .. WorldInformation.get_time() .. "\t")
    s = "%.3f\t"
    for i, value in ipairs(results) do
      handler:write(s:format(value))
    end
    handler:write("\n")
    handler:close()
    log("info", "Calculated results in round ".. WorldInformation.get_time() .. ".")
  end
end