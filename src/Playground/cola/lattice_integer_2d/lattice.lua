-- LUA File for lattice alignment
--   by Andreas Cord-Landwehr, 
--   last update at Su, 22th of March
--
--  This algorithm is intended to align a set of robots 
--    on a 2d layer
--    with common coordinate systems
--    with common knowledge of lattice distance delta (this is 2)
--    with uniform robots
--    with ability to walk exactly on the integer posititions on the layer
--    with limited view on twice the walking distance (this is 2)
--


function distance(A, B)
	return math.sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) + (A.z-B.z)*(A.z-B.z));
end

function main() 

	delta =  2;
	robots = get_visible_robots();

	-- compute all distances
	dist = {};
	for i = 1, #robots do
		dist[i] = distance(get_position(robots[i]), get_position(get_own_identifier()));
	end

--	for i = 1, #robots do
--		if (dist[i] == 0.5) then
--			add_position_request(get_position(robots[i]) -  get_position(get_own_identifier()));
--			return;
--		end
--	end

	--test if robot is above
	pos_r = false;
	pos_or = false;
	pos_o = false;
	pos_ol = false
	pos_l = false;
	pos_ul = false
	pos_u = false;
	pos_ur = false;


	for i = 1, #robots do
		if (get_position(robots[i]).x==1 and get_position(robots[i]).y==0) then
			pos_r = true;
		end
		if (get_position(robots[i]).x==1 and get_position(robots[i]).y==1) then
			pos_or = true;
		end
		if (get_position(robots[i]).x==0 and get_position(robots[i]).y==1) then
			pos_o = true;
		end
		if (get_position(robots[i]).x==-1 and get_position(robots[i]).y==1) then
			pos_ol = true;
		end
		if (get_position(robots[i]).x==-1 and get_position(robots[i]).y==0) then
			pos_l = true;
		end
		if (get_position(robots[i]).x==-1 and get_position(robots[i]).y==-1) then
			pos_ul = true;
		end
		if (get_position(robots[i]).x==0 and get_position(robots[i]).y==-1) then
			pos_u = true;
		end
		if (get_position(robots[i]).x==1 and get_position(robots[i]).y==-1) then
			pos_ur = true;
		end
	end


	second_r = false;
	second_l = false;
	second_o = false;
	second_u = false;
	for i=1, #robots do
		if (get_position(robots[i]).x>=2) then
			second_r = true;
		end
		if (get_position(robots[i]).x<=-2) then
			second_l = true;
		end
		if (get_position(robots[i]).y>=2) then
			second_o = true;
		end
		if (get_position(robots[i]).y<=-2) then
			second_u = true;
		end
	end


	-- do not move if you see nothing right and below
--	if (pos_or==false and pos_r==false and pos_ur==false and pos_u==false and pos_ul==false and pos_ol and pos_o and pos_l) then
--		return;
--	end
--

	-- not moving if stable pattern
	--   |    |      |    |
	-- --+--  +--  --+  --+--  --+--  +-+
	--   |    |      |           |    +-+
	-- check if robots are left or above, else move away
	if (pos_l and pos_o and pos_u and pos_r) then
		-- do nothing
		return;
	end
	if ((pos_o and pos_r and pos_u) or (pos_ol and pos_l and pos_ul)) then
		-- do nothing
		return;
	end
	if ((pos_o and pos_l and pos_u)) then
		-- do nothing
		return;
	end
	if ((pos_l and pos_o and pos_r)) then
		-- do nothing
		return;
	end
	if ((pos_l and pos_u and pos_r)) then
		-- do nothing
		return;
	end
	if ((pos_r and pos_ur and pos_u) or (pos_l and pos_ul and pos_u) or (pos_l and pos_ol and pos_o) or (pos_o and pos_or and pos_r)) then
		-- do nothing
		return;
	end

	
	-- create L.s
	if (pos_l and pos_r and pos_o==false and pos_u==false) then
		if (second_l and pos_ul==false) then
			add_position_request(Vector3d(-1,-1,0));
			return;
		end
		if (second_l and pos_ol==false) then
			add_position_request(Vector3d(-1,1,0));
			return;
		end
		if (second_r and pos_ur==false) then
			add_position_request(Vector3d(1,-1,0));
			return;
		end
		if (second_r and pos_or==false) then
			add_position_request(Vector3d(1,1,0));
			return;
		end

	end
	if (pos_u and pos_o and pos_l==false and pos_r==false) then
		if (second_u and pos_ul==false) then
			add_position_request(Vector3d(-1,-1,0));
			return;
		end
		if (second_u and pos_ur==false) then
			add_position_request(Vector3d(1,-1,0));
			return;
		end
		if (second_o and pos_ol==false) then
			add_position_request(Vector3d(-1,1,0));
			return;
		end
		if (second_o and pos_or==false) then
			add_position_request(Vector3d(1,1,0));
			return;
		end
	end
		

	-- moving to fill space	
	if (pos_ul and pos_u==false and pos_ur) then
		add_position_request(Vector3d(0,-1,0));
		return;
	end
	if (pos_ol and pos_l==false and pos_ul) then
		add_position_request(Vector3d(-1,0,0));
		return;
	end
	if (pos_ol and pos_o==false and pos_or) then
		add_position_request(Vector3d(0,1,0));
		return;
	end
	if (pos_or and pos_r==false and pos_ur) then
		add_position_request(Vector3d(1,0,0));
		return;
	end

	-- if first radius is not working, try second
	if (second_u and (second_o==false or pos_o or pos_ol or pos_or)) then
		add_position_request(Vector3d(0,-1,0));
	end
	if (second_l and (second_r==false or pos_r or pos_or or pos_ur)) then
		add_position_request(Vector3d(-1,0,0));
	end
	if (second_o and (second_u==false or pos_u or pos_ul or pos_ur)) then
		add_position_request(Vector3d(0,1,0));
	end
	if (second_r and (second_l==false or pos_l or pos_ul or pos_ol)) then
		add_position_request(Vector3d(1,0,0));
	end

	-- try something is better then nothing: no near robots, but some in each direction
		
	return;

	
end

