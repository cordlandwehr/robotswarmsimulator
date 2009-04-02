-- LUA File for lattice alignment
--   by Andreas Cord-Landwehr, 
--   last update \today ;) To heavy work to update each time :)
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
	
	-- -- oberer rand
	-- if(not pos_o and not pos_or and not pos_ol and not second_o) then
		-- if(not pos_u and not pos_ur and not pos_ur and not second_u) then
			-- return;
		-- end		
		-- if(not pos_u) then
			-- add_position_request(Vector3d(0,-1,0));
			-- return;
		-- end
		-- if(not pos_ul) then
			-- add_position_request(Vector3d(-1,-1,0));
			-- return;
		-- end
		-- if(not pos_l) then
			-- add_position_request(Vector3d(-1,0,0));
			-- return;
		-- end
	-- end

	if(not pos_u and (second_u or pos_ul or pos_ur)) then
		add_position_request(Vector3d(0,-1,0));
		return;
	end
	
	if(not pos_r and (second_r or pos_or or pos_ur)) then
		add_position_request(Vector3d(1,0,0));
		return;
	end
	
	if(not pos_ur and pos_u and pos_r) then
		add_position_request(Vector3d(1,-1,0));
		return;
	end
	
	if(pos_r and not pos_or and not pos_o and not pos_ol and not pos_l and not pos_ul and not pos_u and not pos_ur) then
		--links zipfel
		add_position_request(Vector3d(1,1,0));
		return;
	end
	
	if(pos_u and not pos_or and not pos_o and not pos_ol and not pos_l and not pos_ul and not pos_r and not pos_ur) then
		--oben zipfel
		add_position_request(Vector3d(-1,-1,0));
		return;
	end	
	
	-- if(pos_r and pos_or and not pos_o and not pos_ol and pos_l and pos_ul and pos_u and pos_ur) then
		-- add_position_request(Vector3d(0,1,0));
		-- return;
	-- end
	
	-- if(pos_u and pos_or and pos_o and not pos_ol and not pos_l and pos_ul and pos_r and pos_ur) then
		-- add_position_request(Vector3d(-1,0,0));
		-- return;
	-- end	
	
	if(pos_u and pos_or and pos_o and not pos_ol and pos_l and pos_ul and pos_r and pos_ur) then
		add_position_request(Vector3d(-1,1,0));
		return;
	end	

	
end

