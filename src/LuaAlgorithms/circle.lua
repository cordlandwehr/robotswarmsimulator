function main() 
	MAX_DIST = 100;

	robots = get_visible_robots();
	center = get_position(get_own_identifier());
	
	for i = 1, #robots do
		center = center + get_position(robots[i]);
	end
	
	center = center / (#robots+1);

	length = math.sqrt(center.x*center.x + center.z * center.z);

	phi = MAX_DIST / length;

	if phi > math.pi/4 then
		phi = math.pi/4;
	end;	

	new_pos = Vector3d(0,0,0);
	new_pos.x = -center.x * math.cos(phi) - center.z * math.sin(phi);
	new_pos.z = center.x * math.sin(phi) - center.z * math.cos(phi);

	new_pos = new_pos + center;

	add_velocity_request(new_pos);

end