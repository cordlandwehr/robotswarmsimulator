--[[
	GridPullSpin
	This is an implementation of the PullSpin aproach on a grid. The grid is spanned by the vectors (1,0,0), (0,1,0) and
	(0,0,1). Push operations will move either horizontally or vertically towards the target point and therefore decrease
	the L1 distance by exactly one. Spin operations on the other hand move one unit horizontally and vertically, such
	that the L1 distance to the target point remains unchanged.
--]]



-------------- globals/initialization --------------

math.randomseed(23)
target_point = Vector3d(0, 0, 0) -- In global coordinates! Robots have to convert this in their local coordinate system.
local_target_point = nil         -- Computed at begin of main method.
direction_signs = nil            -- Computed at begin of main method (indicates direction of target point)
direction_signs_abs_sum = nil    -- Computed at begin of main method.

PULL = 0
SPIN = 1
last_phase = PULL



-------------- some helper methods --------------

--[[
---- Computes the l1 norm of the given vector
--]]
function l1norm(input_vector)
	return math.abs(input_vector.x) + math.abs(input_vector).y + math.abs(input_vector).z
end

--[[
---- Computes the l1 distance of the given vectors
--]]
function l2distance(v1, v2)
	return l1norm(v1-v2)
end


--[[
---- Returns the sign of the given value.
--]]
function sign(value)
	if (value == 0) then
		return 0
	elseif (value > 0) then
		return 1
	else
		return -1
	end
end


--[[
---- Flips a fair coin and returns either 0 or 1, depending on the result of the coin flip
--]]
function flip_coin()
	return math.random(0,1)
end

--[[
---- Return either -1 or 1, both with probability 0.5.
--]]
function random_sign()
	local value = math.random(-1,1)
	while (value == 0) do
		value = math.random(-1,1)
	end
	return value
end


--[[
---- Returns the quadrant of the global coordinate system in which we lie. The quadrants are numberd counterclockwise
---- starting with 1 for the upper-right quadrant.
--]]
function quadrant()
	local global_pos = View.get_robot_coordinate_system_origin(View.get_own_identifier())
	if (global_pos.x >= 0) then
		if (global_pos.y >= 0) then
			return 1
		else
			return 4
		end
	else
		if (global_pos.y >= 0) then
			return 2
		else
			return 3
		end
	end
end


--[[
---- Returns the target point in the robot's local coordinate system.
--]]
function target_point()
	-- this does currently rely on the robots having a standard coordinate system which is simply translated by their
	-- position (not rotated/scaled)
	return Vector3d(0,0,0) - View.get_robot_coordinate_system_origin(View.get_own_identifier())
end


--[[
---- Chooses a random move that decreases the l1 distance to the target point by exactly one.
--]]
function pull()
	local movement_vector
	
	local dimension = math.random(0,2)
	while (direction_signs[dimension] == 0) do
		dimension = math.random(0,2)
	end
	
	if (dimension == 0) then
		movement_vector = Vector3d(direction_signs[dimension], 0, 0)
	elseif (dimension == 1) then
		movement_vector = Vector3d(0, direction_signs[dimension], 0)
	else
		movement_vector = Vector3d(0, 0, direction_signs[dimension])
	end
	View.add_position_request(movement_vector);
end

--[[
---- Chooses a random move that does not change the l1 distance to the target point.
--]]
function spin()
	local movement_vector
	
	local dim1 = nil
	local dim2 = nil
	local signs = {}
	signs[0] = 0
	signs[1] = 0
	signs[2] = 0
	if (direction_signs_abs_sum == 1) then
		if (direction_signs[0] ~= 0) then
			dim1 = 0
		elseif (direction_signs[1] ~= 0) then
			dim1 = 1
		else
			dim1 = 2
		end
		
		dim2 = dim1
		while (dim2 == dim1) do
			dim2 = math.random(0,2)
		end
		
		signs[dim1] = direction_signs[dim1]
		signs[dim2] = random_sign()
	elseif (direction_signs_abs_sum == 2) then
		dim1 = math.random(0,2)
		dim2 = dim1
		while (dim2 == dim1) do
			dim2 = math.random(0,2)
		end
		
		if (direction_signs[dim1] == 0) then
			signs[dim1] = random_sign()
			signs[dim2] = direction_signs[dim2]
		elseif (direction_signs[dim2] == 0) then
			signs[dim1] = direction_signs[dim1]
			signs[dim2] = random_sign()
		else
			rs = random_sign()
			signs[dim1] =  rs * direction_signs[dim1]
			signs[dim2] = -rs * direction_signs[dim2]
		end
	else -- direction_signs_abs_sum == 3
		dim1 = math.random(0,2)
		dim2 = dim1
		while (dim2 == dim1) do
			dim2 = math.random(0,2)
		end
		
		rs = random_sign()
		signs[dim1] =  rs * direction_signs[dim1]
		signs[dim2] = -rs * direction_signs[dim2]
	end
	
	movement_vector = Vector3d(signs[0], signs[1], signs[2])
	View.add_position_request(movement_vector)
end



-------------- main program --------------
function main()
	local_target_point = Vector3d(0,0,0) - View.get_robot_coordinate_system_origin(View.get_own_identifier())	
	direction_signs = {}
	direction_signs[0] = sign(local_target_point.x)
	direction_signs[1] = sign(local_target_point.y)
	direction_signs[2] = sign(local_target_point.z)
	direction_signs_abs_sum = math.abs(direction_signs[0]) + math.abs(direction_signs[1]) + math.abs(direction_signs[2])
	
	if (direction_signs_abs_sum == 0) then
		return
	end

	if (get_robot_last_request_successful(get_own_identifier()) == 0 and last_phase == PULL) then
		add_color_change_request(3)
		spin()
		last_phase = SPIN
	else
		add_color_change_request(0)
		pull()
		last_phase = PULL
	end
end
