

function getColors()
	local colors = {}
	local colorcounts = {}

	local nodes = View.get_visible_robots()
	for i = 1 , #nodes do
		marker = View.get_robot_information(nodes[i])
		local color
		if marker:has_key(":color") then
			color = marker:get_data(":color")
		else
			color = i
		end
		table.insert(colors, color)
		if colorcounts[color] == nil then
			colorcounts[color] = 1
		else
			colorcounts[color] = colorcounts[color]+1
		end
	end
  
  return colors, colorcounts
end

function main()
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)

	colors, colorcounts = getColors()
	local max_col = 0
	local best_color = colors[1]
	for i = 1, #colors do
		if colorcounts[colors[1]] > max_col then
			color = colors[1]
			max_col = colorcounts[colors[1]]
		end
	end

	my_marker:add_data(":color", color)
	my_marker:add_data("role", "private")

	log("Adversary " .. me .. " set its color to " .. color);		

	View.add_marker_request(my_marker)
end