-- colors 0, .., available_color-1 are avaiable
available_colors = 3;

function getLeastFrequentColor()
	--log("getLeastFrequentColor")
  local edges = View.get_visible_edges()
  
  local colors = {}  
  
  for i, edge in ipairs(edges) do
	local neighbor = View.get_tail(edge)
	
	if (neighbor == View.get_own_id()) then
		neighbor = View.get_head(edge)
	end
	
	local marker = View.get_robot_information(neighbor)
	local color = marker:get_data(":color")
	
	--log("neighbor " .. neighbor .. " has color " .. color)

	if colors[color] == nil then
		colors[color] = 0
	end
	colors[color] = colors[color] + 1
  end
  
	--log(View.get_own_id() .. " colors:")
	--for k,v in pairs(colors) do log(k .. v) end 
  
  local minvalue = 10000000000
  local result = 0
  
  for color=0, available_colors-1 do
	if colors[color] == nil then
		colors[color] = 0
	end	
	
	if colors[color] < minvalue then
		minvalue = colors[color]
		result = color
	end
  end
  
  return result
end


function main()
	--log("main")
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)


	
	if View.get_time() == 1 then
		color = me % available_colors
	else
		prev_color = my_marker:get_data(":color")
		--log("node " .. me .. " has color " .. prev_color)
		
		color = getLeastFrequentColor()
	end
	
	my_marker:add_data(":color", color)
	
	log("Node " .. me .. " set its color to " .. color);	

	View.add_marker_request(my_marker)
end