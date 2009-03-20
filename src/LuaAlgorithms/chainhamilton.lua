--[[
	CHAIN HAMILTON
	scripts  uses the chain view to get move directly to the nearest neighbour of the robot.
	if the nearest neighbour is close enough, the robot stops and just jumps to it
--]]

--lua-function for calculating distances
function dist(vector)
	distance = math.sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
	return distance;
end

function main()
	--setting the distance to nearest neighbour that will cause stopping of movement
	MIN_DIST = 1.0;

	--setting nullvector
	zero = Vector3d(0.0,0.0,0.0);

	--getting robots
	robots = get_visible_robots();
	
	--getting position of the nearest neighbour
	--remark: even though robots[1] should be the nearest neighbour, it does not look like it really is...
	npos = get_position(robots[1]);
		
	--calculate distance
	distance = dist(npos);

	if distance > MIN_DIST then
		--move to nearest neighbour
		--add_velocity_request(neighbourposition);
		add_velocity_request(npos);
	else
		--stop movement 
		add_velocity_request(zero);
		--jump to neighbour
		add_position_request(npos);
	end
end
