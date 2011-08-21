-- IO settings
file_path = "./calculated_defects_%s.dat"
file_header = "Time\tMax. Defect\tTotal Defects\n"
file_data = "%d\t%d\t%d\n"

-- calculation settings
calc_interval = 100

-- global variables
timestamp = nil
last_calc = nil

function setup() 
  -- use ISO timestamp [Year][Month][Day]T[Hours][Minutes][Seconds]
  timestamp = os.date("%Y%m%dT%H%M%S")
  -- create new file and write header
  local handler = assert(io.open(file_path:format(timestamp), "w"))
  handler:write(file_header)
  handler:close()
  -- calculate data row for start configuration
  calculate()
end

function calculate()
  -- get new data set
  local time = WorldInformation.get_time()
  local max_defect = Statistics.calculate_maximal_defect()
  local total_defects = Statistics.calculate_total_defects()
  -- open file in append mode and write new data row
  local handler = assert(io.open(file_path:format(timestamp), "a"))
  handler:write(file_data:format(time, max_defect, total_defects))
  handler:close()
  -- rember last calculation
  last_calc = time
end

function main()
  -- say who I am
  log("debug", "Running CalculateDefects.lua ...")
  -- check setup
  if not timestamp then
    -- setup and first calculation
    log("debug", "[CalculateDefects.lua] Setting up IO.")
    setup()
  else
    -- check current time
    local time = WorldInformation.get_time()
    if time - last_calc >= calc_interval then
      log("debug", "[CalculateDefects.lua] Calclating new data row.")
      calculate()
    else
      log("debug", "[CalculateDefects.lua] Ignoring round.")
    end
  end
end
