function main() 

	io.write("Running ", _VERSION, "\n")
	
	my_int = 0;
	my_int = 5/my_int;
	print(my_int);
	
	print("Own pos" .. tostring(get_position(get_own_identifier())));
	
	print("Robots")
	result = get_visible_robots()
	for k, v in ipairs(result) do
	        print(k .. ": " .. v)
	        pos = get_position(v);
	        print("x = " .. pos.x .. ", y = " .. pos.y .. ", z = " .. pos.z);
	        cs = get_robot_coordinate_system_axis(v);
	        print(cs.x_axis.x);
	        type = get_robot_type(v);
	        print(type .. " = " .. RobotType.MASTER);
	end 
	
	print("Obstacles")
	result = get_visible_obstacles()
	for k, v in ipairs(result) do
	        print(k .. ": " .. v)
	        pos = get_position(v);
	        print("x = " .. pos.x .. ", y = " .. pos.y .. ", z = " .. pos.z);
	        print("Box-Obstacle: " .. tostring(is_box_identifier(v)));
	        print("Sphere-Obstacle " .. tostring(is_sphere_identifier(v)));
	        
	end 
	
	print("Marker")
	result = get_visible_markers()
	for k, v in ipairs(result) do
	        print(k .. ": " .. v)
	end 
	
	print("Some tests")
	myvec = Vector3d(5,3,7)
	print("x = " .. myvec.x .. ", y = " .. myvec.y .. ", z = " .. myvec.z);
	myvec = Vector3d()
	myvec.x = 3;
	myvec.y = 3;
	myvec.z = 3;
	myvec = myvec * myvec + myvec;
	print(myvec);
	
	test = MarkerInformation();
	test:add_data("my_var", 5);
	test:add_data("my_string_var", "Hello World!");
	print(test:get_data("my_var"));
	print(test:get_data("my_string_var"));
	
	add_marker_request(test);
	
	print(test:get_data("my_string_var"));

end