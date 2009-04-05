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
--  Algorithm works as follows:
--    (A) move away, if robot is not alone at current point
--    (B) if angle of stable position -> do not move
--    (C) if non-angle point in semi stable/stable formation 
--    (D) mirror L formation, iff usefull
--    (E) fill holes in lines
--    (F) pull together
--

function main() 

	robots = get_visible_robots();

	--test if robot is above
	pos_r = false;
	pos_or = false;
	pos_o = false;
	pos_ol = false
	pos_l = false;
	pos_ul = false
	pos_u = false;
	pos_ur = false;
	pos_same = false;

	-- start with second robot, because #1 is me :)
	for i = 2, #robots do
		if (get_position(robots[i]).x==0 and get_position(robots[i]).y==0) then
			pos_same = true;
		end
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

	-- STARTING OF THE ALGORITHM
	-- PART A:
	-- if I am not alone at my point: go away
	if (pos_same) then
		if (pos_u==false and (pos_ur or pos_ul)) then
			add_position_request(Vector3d(0,-1,0));
			return;
		end
		if (pos_l==false and (pos_ul or pos_ol)) then
			add_position_request(Vector3d(-1,0,0));
			return;
		end
		if (pos_o==false and (pos_or or pos_ol)) then
			add_position_request(Vector3d(0,1,0));
			return;
		end
		if (pos_r==false and (pos_or or pos_ur)) then
			add_position_request(Vector3d(1,0,0));
			return;
		end
		return;
	end

	-- PART B:
	-- not moving if stable pattern
	--   |  
	-- --+--
	--   | 
	-- check if robots are left or above, else move away
	if (pos_l and pos_o and pos_u and pos_r) then
		-- do nothing
		return;
	end
	-- not moving if stable pattern
	--  |      |    |
	--  +--  --+  --+--  --+--  +-+
	--  |      |           |    +-+
	-- check if robots are left or above, else move away
	if ((pos_o and pos_r and pos_u)) then
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


	-- PART C:
	-- next is: robot is corner of a box, this should not be destroyed in most cases, but to get local swarm, maybe sometimes...
	if ((pos_r and pos_ur and pos_u) or (pos_l and pos_ul and pos_u) or (pos_l and pos_ol and pos_o) or (pos_o and pos_or and pos_r)) then
		-- top-left corner of box
		if (pos_r and pos_ur and pos_u and pos_ul==false and pos_l==false and pos_ol==false and pos_o==false and pos_or==false) then
			if (second_l) then
				add_position_request(Vector3d(-1,0,0));
				return;
			end
			if (second_o) then
				add_position_request(Vector3d(0,1,0));
				return;
			end
		end
		-- top-right corner of box
		if (pos_l and pos_ul and pos_u and pos_ur==false and pos_r==false and pos_or==false and pos_o==false and pos_ol==false) then
			if (second_o) then
				add_position_request(Vector3d(0,1,0));
				return;
			end
			if (second_r) then
				add_position_request(Vector3d(1,0,0));
				return;
			end
		end
		-- bottom-right corner of box
		if (pos_l and pos_ol and pos_o and pos_or==false and pos_r==false and pos_ur==false and pos_u==false and pos_ul==false) then
			if (second_r) then
				add_position_request(Vector3d(1,0,0));
				return;
			end
			if (second_u) then
				add_position_request(Vector3d(0,-1,0));
				return;
			end
		end
		-- bottom-left corner of box
		if (pos_o and pos_or and pos_r and pos_ur==false and pos_u==false and pos_ul==false and pos_l==false and pos_ol==false) then
			if (second_u) then
				add_position_request(Vector3d(0,-1,0));
				return;
			end
			if (second_l) then
				add_position_request(Vector3d(-1,0,0));
				return;
			end
		end
		-- ELSE 
		-- this point is stable and is not allowed to move!
		return;
	end

	-- PART D:
	-- TODO just an idea by now: mirror L formations if usefull and if angle point



	-- PART E:
	-- move to fill hole in line	
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

	-- PART F:	
	-- compact swarm lattice by compacting lines
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

	-- PART G:
	-- check for first row
	-- if first radius is not filled, try second one
	if (second_u and (second_o==false or pos_o or pos_ol or pos_or) and not (pos_ul or pos_u or pos_ur)) then
		add_position_request(Vector3d(0,-1,0));
		return;
	end
	if (second_l and (second_r==false or pos_r or pos_or or pos_ur) and not (pos_ol or pos_l or pos_ul)) then
		add_position_request(Vector3d(-1,0,0));
		return;
	end
	if (second_o and (second_u==false or pos_u or pos_ul or pos_ur) and not (pos_o or pos_or or pos_ol)) then
		add_position_request(Vector3d(0,1,0));
		return;
	end
	if (second_r and (second_l==false or pos_l or pos_ul or pos_ol) and not (pos_or or pos_r or pos_ur)) then
		add_position_request(Vector3d(1,0,0));
		return;
	end

	return;

	
end

