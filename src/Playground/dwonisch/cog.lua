function main() 

	robots = get_visible_robots()
	center = Vector3d(0,0,0);
	
	for i = 1, #robots do
		center = center + get_position(robots[i]);
	end
	
	center = center / #robots;
	
	add_position_request(center);

end