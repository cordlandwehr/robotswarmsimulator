

function main()
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)	

	my_marker:add_data("color", math.random(0, 2))		

	View.add_marker_request(my_marker)
end