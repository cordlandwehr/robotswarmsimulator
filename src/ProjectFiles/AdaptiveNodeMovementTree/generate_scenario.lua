name = arg[1]
depth = arg[2]
script_out = arg[3]
onios = arg[4]
project_file_out = arg[5]
if #arg > 5 then
	params = arg[6]
else
	params = 0
end

func_header = "\nfunction main()\n"
func_call = "generic_main(\"" .. name .. "_" .. depth .. params .. "\", " .. depth .. ", get_weight, handle_request, " .. onios .. ", localSetup" .. ")\n"
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
  for i = 7, #arg do
    handler:write("arg" .. i-6 .. " = " .. arg[i] .. "\n")
  end
  handler:close()
end

if #arg < 5 then
  print("Usage: 'lua " .. arg[0] .. " <strategy name> <tree depth> <script output file> <onios (true/false)> <project_file_out> <paramsWithDash> <params...>' ...")
else
  print("Generating '" .. script_out .. "' with depth " .. depth .. " and script " .. script_out .. ".")
  generate_script()
  generate_project_file()
  generate_includes()
end

