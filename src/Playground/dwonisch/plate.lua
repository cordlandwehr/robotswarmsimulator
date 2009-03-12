function main() 

	robots = get_visible_robots();
	if(#robots <= 4) then
	
		center = get_position(get_own_identifier());
	
		for i = 1, #robots do
			center = center + get_position(robots[i]);
		end
		
		center = center / (#robots+1);
		
		add_position_request(center);

	else 
		dist = Vector3d(0,0,0);
		
		dist.y = math.random(-3,3);
		dist.z = math.random(-3,3);
		
		add_position_request(dist);
	
	end
end