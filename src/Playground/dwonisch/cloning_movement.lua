-- spheric_view, id_view, time_view, position_view

last_activation_time = nil;
last_positions = nil; --map<id,position>
eps = 0.0001;

function main()
	local robots = View.get_visible_robots();

	local new_position = compute_new_position(robots);
	
	-- save configuration so we can compute movementvectors
	save_configuration(robots);
	
	if(new_position ~= nil) then
		add_position_request(new_position);	
	end
end

function compute_new_position(robots)
	if(last_activation_time ~= nil and View.get_time() - last_activation_time < 9) then
		--not my turn to move..
		return nil;
	end
	if(last_positions == nil) then
		return nil;
	end
	--check for movement
	local movement_vector = nil;
	for i = 1, #robots do
		if(last_positions[View.get_id(robots[i])] ~= nil) then
			local movement = View.get_position(robots[i]) - last_positions[View.get_id(robots[i])];
			if(get_length(movement) > eps) then
				movement_vector = movement;
				break;
			end
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
	last_positions = {};
	for i = 1, #robots do
		last_positions[View.get_id(robots[i])] = View.get_position(robots[i]);
	end
end
