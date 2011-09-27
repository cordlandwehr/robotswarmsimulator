-- variables for sampling intervals
next_active = 0
max_wait_period = nil
current_time = nil
shifted_time = nil
shift = -24

-- output pathes
timestamp = nil
n = nil
file_path_evaluation = "maf_deviation_(%d_robots)_%s"
file_path_distribution = "maf_distribution_(%d_robots)_%s"

-- gnuplot script 
gnuplot_script = [[# gnuplot script for hop distances (greedy)

# configure terminal
set terminal pdf font ",4"
set output '<FILENAME>.pdf'
set title 'Move & Forget Circle'

# labels
set log x
set xlabel 'time'
set ylabel 'hop distance'

# plot mean and sd
plot '<FILENAME>.dat' using 1:11:12 title 'mean hop distance' with errorbars

set style fill transparent solid 0.15 noborder
set pointsize 0.5

# plot mean and sd
plot '<FILENAME>.dat' using 1:10 title 'octiles' with filledcurve y1=1 lc rgb "forest-green" , \
     '<FILENAME>.dat' using 1:9 notitle with filledcurve y1=1 lc rgb "forest-green" , \
     '<FILENAME>.dat' using 1:8 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:7 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:6 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:5 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:4 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:3 notitle with filledcurve y1=1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:10 notitle with linespoints pt 1 lc rgb "forest-green" , \
     '<FILENAME>.dat' using 1:9 notitle with linespoints pt 1 lc rgb "forest-green" , \
     '<FILENAME>.dat' using 1:8 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:7 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:5 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:4 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:3 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:2 notitle with linespoints pt 1 lc rgb "forest-green", \
     '<FILENAME>.dat' using 1:6 title 'median' with linespoints pt 1 lw 3 lc rgb "gold"

# plot histograms
set hidden3d
set log x
set xrange [10:<MAXRANGE>]
set xlabel 'time'
set ylabel 'hop distance'
set zlabel 'occurance'

set view 70,140,1,1

splot '<FILENAMEDISTR>.dat' using 1:2:3 notitle with lines lc rgb "forest-green"

]]

function setup() 
  -- use ISO timestamp [Year][Month][Day]T[Hours][Minutes][Seconds]
  timestamp = os.date("%Y%m%dT%H%M%S")
  
  -- how many robots are there? maximum wait interval?
  n = #(WorldInformation.get_robots())
  max_wait_period = 96 * n
  
  -- file pathes
  local file_path = file_path_evaluation:format(n, timestamp)
  local file_path_distr = file_path_distribution:format(n, timestamp)
  
  -- create data files
  local handler = assert(io.open(file_path .. ".dat", "w"))
  handler:close()
  local handler = assert(io.open(file_path_distr .. ".dat", "w"))
  handler:close()

  -- create gnuplot file
  local gnuplot = gnuplot_script:gsub("<FILENAME>", file_path)
  local gnuplot = gnuplot:gsub("<FILENAMEDISTR>", file_path_distr)
  local gnuplot = gnuplot:gsub("<MAXRANGE>", max_wait_period * 48)
  local handler = assert(io.open(file_path .. ".plt", "w"))
  handler:write(gnuplot)
  handler:close()
end

function check_wait()
  -- update time
  current_time = WorldInformation.get_time()
  shifted_time = current_time - shift
  -- decide what to do
  if next_active <= shifted_time then
    -- how long should be waited until the next evaluation?
    if shifted_time > max_wait_period then
      next_active = shifted_time + max_wait_period
    else
      next_active = shifted_time + shifted_time*0.5
    end
    -- it is time to evaluate the world
    return true
  else
    -- not the right time yet
    return false
  end
end

function main()
  -- is everything set up?
  if not current_time then
    setup()
  end
  
  -- is it time to evaluate the world?
  if check_wait() then
    -- get results
    local results = Statistics.evaluate_first_mf_experiment()
    
    -- open evaluation file in append mode
    local handler = assert(io.open(file_path_evaluation:format(n, timestamp) .. ".dat", "a"))
    
    -- time?
    handler:write("" ..current_time .. "\t")
    -- data points ...
    local s = "%.3f\t"
    for i = 1, 13 do
      handler:write(s:format(results[i]))
    end
    -- newline and close file
    handler:write("\n")
    handler:close()
    
    
    -- open distribution file in append mode
    local handler = assert(io.open(file_path_distribution:format(n, timestamp) .. ".dat", "a"))
    -- data points ...
    local s = "%d\t%d\t%d\t\n"
    for i = 14, #results do
      handler:write(s:format(current_time, i-13, results[i]))
    end
    handler:write("\n")
    for i = 14, #results do
      handler:write(s:format(current_time, i-13, 0))
    end
    handler:write("\n\n")
    -- close file
    handler:close()
    
    -- give some feedback
    log("info", "Calculated results in round ".. current_time .. ".")
  end
end