function main() 

	robots = get_visible_robots();
	center = get_position(get_own_identifier());
	
	for i = 1, #robots do
		center = center + get_position(robots[i]);
	end
	
	center = center / (#robots+1);
	
	add_position_request(center);

end