name = arg[1]
depth = arg[2]
script_out = arg[3]
onios = arg[4]

func_header = "\nfunction main()\n"
func_call = "generic_main(\"" .. name ..  "\", " .. depth .. ", get_weight, handle_request, " .. onios .. ", localSetup" .. ")\n"
func_footer = "end\n"



function generate_script()
  local handler = assert(io.open(script_out, "aw"))
  handler:write(func_header)
  handler:write(func_call)
  handler:write(func_footer)  
  handler:close()
end

if #arg < 4 then
  print("Usage: 'lua " .. arg[0] .. " <strategy name> <tree depth> <script output file> <onios (true/false)>' ...")
else
  print("Generating '" .. script_out .. "' with depth " .. depth .. " and script " .. script_out .. ".")
  generate_script()
end

