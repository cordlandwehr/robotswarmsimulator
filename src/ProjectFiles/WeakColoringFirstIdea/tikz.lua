-------------------------------------------------------------------------------
-- TikZ generator -------------------------------------------------------------
-------------------------------------------------------------------------------

template = {}

template.colors = {"black", "blue", "green", "red", "cyan", "yellow", "orange", "magenta"}

template.latex = [[% auto generated LaTeX file by RobotSwarmSimulator

\documentclass[a4, landscape]{scrreprt}

\usepackage[utf8]{inputenc}

\usepackage{tikz}
\usetikzlibrary{shapes}

% stylesheet for general graphs (using namespace 'simulator')

% vertices
\tikzset{simulator:vert/.style={draw = none, shape = circle, minimum size = 0.125cm, fill = black}}

% edges
\tikzset{simulator:edge/.style={thick, color = black}}

\begin{document}
<CONTENT>
\end{document}

]]

template.graph = [[
\begin{tikzpicture}[scale = <SCALE>]

<NODES>

<EDGES>

\end{tikzpicture}
]]

template.node = "\\node[simulator:vert, <STYLE>] (<NAME>) at (<X>, <Y>) {};\n"

template.edge = "\\path[simulator:edge, <STYLE>] (<TAIL>) edge (<HEAD>);\n"

template.placeholders = {"CONTENT", "SCALE", "NODES", "EDGES", "STYLE", "NAME", "X", "Y", "TAIL", "HEAD"}

function template.replace(string, map)
  local result = string
  for i, placeholder in ipairs(template.placeholders) do
    if map[placeholder] then
      result = result:gsub("<" .. placeholder .. ">", map[placeholder])
    else
      result = result:gsub("<" .. placeholder .. ">", "")
    end
  end
  return result
end

function table.contains(t, value)
	for _, v in pairs(t) do
		if v == value then
			return true
		end
	end
	return false
end

function getColors()
	local colors = {}

	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		local color
		if marker:has_key(":color") then
			color = marker:get_data(":color")
		else
			color = i
		end
		if not table.contains(colors, color) then
			table.insert(colors, color)
		end
	end
  
  return colors
end
-------------------------------------------------------------------------------
-- Iterate over all nodes and edges -------------------------------------------
-------------------------------------------------------------------------------

function main()
  name = "node%05d"
  local colors = {}
  for i, color in ipairs(getColors()) do
  	colors[color] = i-1
  end
  
  nodes = ""
  minx, miny, maxx, maxy = nil, nil, nil, nil
  for i, robot in ipairs(WorldInformation.get_robots()) do
    local map = {}
    map["NAME"] = name:format(robot)
    local position = WorldInformation.get_robot_position(robot)
    if not minx or position.x < minx then minx = position.x end
    if not miny or position.y < miny then miny = position.y end
    if not maxx or position.x > maxx then maxx = position.x end
    if not maxy or position.y > maxy then maxy = position.y end
    map["X"] = position.x
    map["Y"] = position.y
    local marker = WorldInformation.get_robot_information(robot)
    if marker:has_key(":color") then
      map["STYLE"] = "color = " .. template.colors[(colors[marker:get_data(":color")] % #template.colors)+1]
    end
    nodes = nodes .. template.replace(template.node, map)
  end

  scale = math.max(maxx-minx, maxy-miny)
  scale = 20 / scale
  
  edges = ""
  for i, edge in ipairs(WorldInformation.get_edges("all")) do
    local map = {}
    map["TAIL"] = name:format(WorldInformation.get_tail(edge))
    map["HEAD"] = name:format(WorldInformation.get_head(edge))
    local marker = WorldInformation.get_edge_information(edge)
    if marker:has_key(":color") then
      map["STYLE"] = "color = " .. template.colors[(marker:get_data(":color") % #template.colors)+1] .. ", "
    else
       map["STYLE"] = ""
    end
    if WorldInformation.is_directed(edge) then
      map["STYLE"] = map["STYLE"] .. "->" 
    else
      map["STYLE"] = map["STYLE"] .. "-"
    end
    edges = edges .. template.replace(template.edge, map)
  end

  map = {}
  map["NODES"] = nodes
  map["EDGES"] = edges
  map["SCALE"] = "" .. scale

  graph = template.replace(template.graph, map)

  map = {}
  map["CONTENT"] = graph

  latex = template.replace(template.latex, map)
    
  handler = assert(io.open("screenshot.tex", "w"))
  handler:write(latex)
  handler:close()
end