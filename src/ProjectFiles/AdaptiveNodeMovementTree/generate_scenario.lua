seed = arg[1]
name = arg[2]
depth = arg[3]
script_out = arg[4]
onios = arg[5]
project_file_out = arg[6]
if #arg > 6 then
	params = arg[7]
else
	params = ""
end

func_header = "\nfunction main()\n"
func_call = "generic_main(" .. seed .. ", " .. "\"" .. name .. "_" .. depth .. params .. "\", " .. depth .. ", get_weight, handle_request, " .. onios .. ", localSetup" .. ")\n"
func_footer = "end\n"
includes_out = "includes.lua"

project_file ="PROJECT_NAME=Adaptive Node Movement for Trees\nROBOT_FILENAME=robots.csv\nEDGE_FILENAME=edges.csv\nASG=SYNCHRONOUS_WM\nWORLD_MODIFIERS=" .. script_out .. "\n"

function generate_script()
  local handler = assert(io.open(script_out, "aw"))
  handler:write(func_header)
  handler:write(func_call)
  handler:write(func_footer)  
  handler:close()
end

function generate_project_file()
  local handler = assert(io.open(project_file_out, "w"))
  handler:write(project_file)
  handler:close()
end

function generate_includes()
  local handler = assert(io.open(includes_out, "w"))
  for i = 8, #arg do
    handler:write("arg" .. i-7 .. " = " .. arg[i] .. "\n")
  end
  handler:close()
end

if #arg < 6 then
  print("Usage: 'lua " .. arg[0] .. " <seed> <strategy name> <tree depth> <script output file> <onios (true/false)> <project_file_out> <paramsWithDash> <params...>' ...")
else
  print("Generating '" .. script_out .. "' with depth " .. depth .. " and script " .. script_out .. ".")
  generate_script()
  generate_project_file()
  generate_includes()
end

