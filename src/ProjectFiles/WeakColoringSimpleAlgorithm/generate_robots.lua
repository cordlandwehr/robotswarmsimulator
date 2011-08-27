n = arg[1]
algo = arg[2]
robot_out = arg[3]
edge_out = arg[4]

robot_header = "\"ID\",\"x-position\",\"y-position\",\"z-position\",\"marker-info\",\"algorithm\"\n"
robot_row = "%d,%.3f,%.3f,%.3f,\"%s\"\n"
edge_header = "\"source\",\"dest\",\"Undirected\"\n"
edge_row = "%d,%d,%d\n"

r = 100

function generate_robots()
  local handler = assert(io.open(robot_out, "w"))
  handler:write(robot_header)
  for i = 1, n do
    -- calculate position on circle
    local p = i / n
    local x = math.cos(p * 2*math.pi) * r
    local y = math.sin(p * 2*math.pi) * r
    handler:write(robot_row:format(i-1, x, y, 0, algo))
  end
  handler:close()
end

function generate_edges()
  local handler = assert(io.open(edge_out, "w"))
  handler:write(edge_header)
  for i = 1, n do
    local id = i-1
    handler:write(edge_row:format(id, (id+1)%n, 1))
  end
  handler:close() 
end

if #arg < 4 then
  print("Usage: 'lua " .. arg[0] .. " <num_of_robots> <algorithm> <robot_output_file> <edge_output_file>' ...")
else
  print("Generating '" .. robot_out .. "' with " .. n .. " '" .. algo .. "' robots.")
  generate_robots()
  print("Generating '" .. edge_out .. "' for " .. n .. " '" .. algo .. "' robots.")
  generate_edges()
end

