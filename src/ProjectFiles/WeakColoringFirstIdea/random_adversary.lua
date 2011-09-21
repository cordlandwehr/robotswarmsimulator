

function main()
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)

	color = math.random(0,2)

	my_marker:add_data(":color", color)

	log("Adversary " .. me .. " set its color to " .. color);		

	View.add_marker_request(my_marker)
end