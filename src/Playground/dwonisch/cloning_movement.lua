-- spheric_view, id_view, time_view, position_view

last_activation_time = nil;
last_positions = nil; --map<id,position>
eps = 0.0001;

function main()
	local robots = View.get_visible_robots();

	local new_position = compute_new_position(robots);
	
	-- save configuration so we can compute movementvectors
	save_configuration(robots);
	
	add_position_request(new_position);	
end

function compute_new_position(robots)
	if(last_activation_time ~= nil and View.get_time() - last_activation_time < 3) then
		--not my turn to move..
		return;
	end
	--check for movement
	local movement_vector = nil;
	for i = 1, #robots do
		local movement = View.get_position(robot[i]) - last_position[View.get_id(robot[i])];
		if(get_length(movement) > eps) then
			movement_vector = movement;
			break;
		end
	end
	
	--if noticed any movement clone that movement and update activation timer
	if(movement_vector ~= nil) then
		last_activation_time = View.get_time();	
	end
	return movement_vector;
end

function get_length(vector)
	local distance = math.sqrt(vector.x^2 + vector.y^2 + vector.z^2);
	return distance;
end

--memorises the positions of the robots using last_position map. 
function save_configuration(robots) 
	for i = 1, #robots do
		last_positions[View.get_id(robot[i])] = View.get_position(robot[i]);
	end
end
