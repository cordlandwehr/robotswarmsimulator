--[[
	SmartPullSpin
	TODO(peter): description
--]]



-------------- globals/initialization --------------

-- some configuration variables
step_length = 0.2;
spin_length = math.pi;
push_length = 1.0;
seed        = 42-23;


-- variables to maintain internal state
PULL = 1; SPIN = 2; PUSH = 3;
current_state = PULL;
pulllock = false;
spinlock = false;

-- random number generator used to get random vectors on unit sphere
-- Note: Every robot needs its own seed, otherwise they will get the same sequence of 'random' values. Therefor, we use
--       Lua's random number generator to generate seeds for each robot.
--       This still is not perfect, because the probability that two robots get the same seed is still relatively high
--       and if this happens this two robots will generate once more the exactly same 'random' values :-/.
math.randomseed(42-23)
distribution_generator_seed = math.random(2000000000) -- this should be as large a integer as possible
distribution_generator = DistributionGenerator(distribution_generator_seed)
distribution_generator:init_uniform_on_sphere(3)

-- more internal variables and initialization
spin_counter = 0;       -- used to count the number of steps done in this spin phase
push_counter = 0;
max_number_of_spin_steps = math.floor(spin_length/step_length+0.5);
max_number_of_push_steps = math.floor(push_length/step_length+0.5);
movement_circle_center = nil;
movement_circle_base1  = nil;
movement_circle_base2  = nil;
movement_circle_radius = nil;



-------------- some helper methods --------------
--[[
---- Computes the inner (i.e. scalar) product of two vectors.
--]]
function inner_product(vector1, vector2)
	local component_wise = vector1*vector2;
	return component_wise.x + component_wise.y + component_wise.z;
end


--[[
---- Computes the length (euclidean norm) of the given vector
--]]
function length(input_vector)
	local zero_vector   = Vector3d(0, 0, 0);
	return Geometry.compute_distance(zero_vector, input_vector);
end


--[[
---- Computes a random vector that is orthogonal to the given one and has the same length.
---- Note however, that if the input vector equals zero, the zero vector is returned.
--]]
function compute_random_orthogonal_vector(input_vector)
	 -- if we get zero as input vector we return zero ourself...
	local input_length = length(input_vector);
	if (input_length == 0) then
		return Vector3d(0, 0, 0);
	end
	
	-- compute a random vector that is orthogonal to the input_vector
	local output_vector = nil;
	local output_length = nil;
	repeat
		output_vector = distribution_generator:get_value_uniform_on_sphere_3d()
		
		-- project output vector onto plane perpendicular to v
		local input_vector_normalized = input_vector/input_length;
		output_vector = output_vector - inner_product(output_vector, input_vector_normalized)*input_vector_normalized;
		output_length = length(output_vector);
	until output_length > 0
	
	-- scale output_vector to same length as input_vector
	output_vector = input_length * output_vector/output_length;
	
	return output_vector;
end


--[[
---- Switches state from PULL to SPIN or SPIN to PULL, depending on the current state. If this enters the SPIN
---- state, some additional initializations are performed (as for example the computation of the movement circle).
--]]
function switch_state(STATE)
	if (STATE == PULL) then
		current_state = PULL;
	elseif (STATE == SPIN) then
		spin_counter = 0;
		current_state = SPIN;
		
		-- computation of the movement circle (represented as center with two orthogonal base vectors both having the
		-- length of the circle's radius)
		local my_position = View.get_position(View.get_own_identifier());
		movement_circle_center = target_point();
		movement_circle_radius = Geometry.compute_distance(my_position, movement_circle_center);
		movement_circle_base1  = my_position - movement_circle_center;
		movement_circle_base2  = compute_random_orthogonal_vector(movement_circle_base1);
	else
		current_state = PUSH;
		push_counter = 0;
	end
end


--[[
---- Returns the target point of the robot (in this case the center of the miniball around all visible robots).
--]]
function target_point()
	-- get all visible robots and gather their (and our own) position in a table
	local visible_robots = View.get_visible_robots();
	local visible_positions = { View.get_position(get_own_identifier()) };
	for ignore, robot in pairs(visible_robots) do
		table.insert(visible_positions, View.get_position(robot));
	end
	
	-- compute and return miniball center
	return Geometry.compute_cminiball(visible_positions);
end


--[[
---- Computes a target point (see target_point() function) and submitts a position request in that direction. The
---- request uses the step length given by the value of the step_length variable.
--]]
function pull()
	local center_mball = target_point();
	local my_position  = View.get_position(View.get_own_identifier());
	local distance     = Geometry.compute_distance(my_position, center_mball);
	
	-- check whether we already reached the target point
	if (distance == 0) then
		return;
	end
	
	-- submit position request
	local position_request = step_length * (center_mball - my_position)/distance;
	View.add_position_request(position_request);
end


--[[
---- The robot will move on it's movement circle (computed by the switch_state() function) up to a distance of
---- spin_length. Note that a collision during a SPIN operation will cause a state change and therefor end the SPIN
---- operation (see the collision check in the main() function).
--]]
function spin()
	-- update spin counter ("time spent in SPIN state so far")
	spin_counter = spin_counter + 1;
	
	-- compute position request
	local position_request = nil;
	if (movement_circle_radius == 0) then
		position_request = Vector3d(0, 0, 0);
		View.add_position_request(position_request);
		return;
	else
		local alpha = spin_counter*step_length / movement_circle_radius;
		position_request = movement_circle_center + math.cos(alpha)*movement_circle_base1
		                                          + math.sin(alpha)*movement_circle_base2;
	end
	
	-- check for the end of this SPIN phase
	if (spin_counter >= max_number_of_spin_steps) then
		switch_state(PULL);
	end
	
	-- update movement circle (for next step in SPIN phase)
	movement_circle_center = movement_circle_center - position_request;
	
	-- submit position request
	View.add_position_request(position_request);
end

--[[
	used to overcome pullspinlocks. Robot is moving the pushdistance backwards if it is the outermost robot
--]]
function push()

	-- flip coin to decide whether push or not to push
	if (math.random() <= 0.5) then
		return;
	end

	push_counter = push_counter + 1;

	local center_mball = target_point();
	local my_position  = View.get_position(View.get_own_identifier());
	local distance     = Geometry.compute_distance(my_position, center_mball);
	
		
	-- check for the end of this PUSH phase
	if (push_counter >= max_number_of_push_steps) then
		switch_state(PULL);
	end

	-- check whether there is a robot with a greater distance to the miniballcenter
----[[	
	local visible_robots = View.get_visible_robots();
	for i = 1, #visible_robots do
		if distance < Geometry.compute_distance(View.get_position(visible_robots[i]), center_mball) then
			return;
		end
	end
--]]

	-- submit position request
	local position_request = (-1.0) * step_length * (center_mball - my_position)/distance;
	View.add_position_request(position_request);
	print("performed push!");
	
end


-------------- main program --------------
function main()
	-- check wether last request was successfull and detect locks if it was not
	if (get_robot_last_request_successful(get_own_identifier()) == 0) then
		
		if (current_state == PULL) then
			pulllock = true;
		elseif (current_state == SPIN) then
			spinlock = true;
		end
		
		if (pulllock and not spinlock) then
			switch_state(SPIN);
		elseif (pulllock and spinlock) then
			switch_state(PUSH);
		else
			switch_state(PULL); --should not happen
		end
		
	else -- last request was successful

		if (current_state == PULL) then
			pulllock = false;
			spinlock = false; --be a bit optimistic!
		elseif (current_state == SPIN) then
			spinlock = false;
		end
		
	end
	
	-- dispatch action depending on current state
	if (current_state == PULL) then
		pull();
	elseif (current_state == SPIN) then
		spin();
	else
		push();
	end
end
