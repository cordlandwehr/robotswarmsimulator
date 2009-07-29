
--lua-function for calculating distances
function dist(vector)
	local distance = math.sqrt(vector.x^2 + vector.y^2 + vector.z^2);	
	return distance;
end

function normalize(vec) 
	return vec/dist(vec);
end

--function returns 2dimensional normal-vector
function get_normal(vector)
		local norm = Vector3d(0.0,0.0,0.0);
		norm.x =  1 * vector.y / dist(vector);
		norm.y = -1 * vector.x / dist(vector);
		return norm;
end


-- Returns the positions in the lattice structure that have the lowest among of robots
-- x_dir, y_dir are normalized vectors indicating the axis the lattice is oriented to
function find_lowest_positions(robots, x_dir, y_dir) 

	local self_count = countRobotsOnPos(robots, Vector3d(0,0,0));
	local left_count = countRobotsOnPos(robots, (-1) * x_dir);
	local right_count = countRobotsOnPos(robots, x_dir);
	local top_count = countRobotsOnPos(robots, y_dir);
	local bottom_count = countRobotsOnPos(robots, (-1) * y_dir);
	
	local min_count = math.min(self_count, left_count, right_count, top_count, bottom_count);

	local local_positions = {};
	
	if(self_count == min_count) then
		local_positions[#local_positions + 1] = Vector3d(0,0,0);
		if(self_count == 0) then
			--dont move when already at free spot
			add_color_change_request(1)
			return local_positions;
		end			
	end
	if(left_count == min_count) then
		local_positions[#local_positions + 1] = (-1) * x_dir;
	end
	if(right_count == min_count) then
		local_positions[#local_positions + 1] = x_dir;
	end
	if(top_count == min_count) then
		local_positions[#local_positions + 1] = y_dir;
	end
	if(bottom_count == min_count) then
		local_positions[#local_positions + 1] = (-1) * y_dir;
	end
	
	add_color_change_request(4);
	
	return local_positions;	
end

function countRobotsOnPos(robots, vector) 
	local count = 0;
	for i =1, #robots do 
		if(dist(get_position(robots[i]) - vector) < EPS) then
			count = count + 1;
		end
	end
	return count;	
end


-- returns x_dir and y_dir as defined at find_lowest_position, if possible; nil otherwise
function find_lattice_axis(robots) 
	for i =1, #robots do 		
		if(dist(get_position(robots[i])) > EPS) then
			local x_dir = get_position(robots[i]);
			return x_dir, get_normal(x_dir);
		end
	end
	
	return nil, nil;	
end

EPS = 0.001;

function main()
	local robots = get_visible_robots();
	local x_dir, y_dir = find_lattice_axis(robots);
	if(x_dir == nil) then
		-- no other robot found, define lattice by yourself
		add_position_request(Vector3d(0,1.0,0));
	else
		local lowest_positions = find_lowest_positions(robots, x_dir, y_dir);
		gen_init_uniform(1, #lowest_positions);
		local random_index = gen_get_uniform();
		add_position_request(lowest_positions[random_index]);		
	end
end
