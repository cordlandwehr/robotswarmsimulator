function getLeastFrequentColor()
  local edges = View.get_visible_edges()
  
  local colors = {}  
  
  for i, edge in ipairs(edges) do
	tail = View.get_tail(edge)
	marker = View.get_robot_information(tail)
	color = marker:get_data("color")

	if colors[color] == nil then
		colors[color] = 0
	end
	colors[color] = colors[color] + 1
  end
  
  local maxvalue = 0
  local max
  
  for key,value in pairs(colors) do
	if value > maxvalue then
		max = key	
	end
  end
  
  return max
end


function main()
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)
	
	if View.get_time() == 1 then
		my_marker:add_data("color", me)
	else
		color = getLeastFrequentColor()
		my_marker:add_data("color", color)	
		
		
		color = my_marker:get_data("color")

		print("me: " .. me .. " color: " .. color)
	end
	

	View.add_marker_request(my_marker)
end