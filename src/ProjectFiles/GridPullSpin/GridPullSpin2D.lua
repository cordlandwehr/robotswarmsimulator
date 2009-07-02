--[[
	GridPullSpin
	This is an implementation of the PullSpin aproach on a grid. The grid is spanned by the vectors (1,0) and (0,1).
	Push operations will move either horizontally or vertically towards the target point and therefore decrease the L1
	distance by exactly one. Spin operations on the other hand move one unit horizontally and vertically, such that the
	L1 distance to the target point remains unchanged.
	
	Note that this algorithm does operate on the XY-plane only. Thus, any z-coordinate should be set to zeor.
--]]



-------------- globals/initialization --------------

math.randomseed(23)
target_point = Vector3d(0, 0, 0) -- In global coordinates! Robots have to convert this in their local coordinate system.

PULL = 0
SPIN = 1
last_phase = PULL



-------------- some helper methods --------------

--[[
---- Computes the l1 norm of the given vector
--]]
function l1norm(input_vector)
	return math.abs(input_vector.x) + math.abs(input_vector).y
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
	if (flip_coin() == 0) then
		movement_vector = Vector3d(sign(target_point().x), 0, 0)
	else
		movement_vector = Vector3d(0, sign(target_point().y), 0)
	end
	View.add_position_request(movement_vector);
end

--[[
---- Chooses a random move that does not change the l1 distance to the target point.
--]]
function spin()
	local movement_vector
	
	local sign_x = sign(target_point().x)
	local sign_y = sign(target_point().y)
	
	if (sign_x == 0 and sign_y ~= 0) then
		while (sign_x == 0 and sign_y ~= 0) do
			sign_x = math.random(-1,1)
		end
		movement_vector = Vector3d(sign_x, sign_y, 0)
	elseif (sign_x ~= 0 and sign_y == 0) then
		while (sign_x ~= 0 and sign_y == 0) do
			sign_y = math.random(-1,1)
		end
		movement_vector = Vector3d(sign_x, sign_y, 0)
	else
		if (flip_coin() == 0) then
			movement_vector = Vector3d( sign_x, -sign_y, 0)
		else
			movement_vector = Vector3d(-sign_x,  sign_y, 0)
		end
	end
	View.add_position_request(movement_vector);
end



-------------- main program --------------
function main()
	if (get_robot_last_request_successful(get_own_identifier()) == 0 and last_phase == PULL) then
		spin()
		last_phase = SPIN
	else
		pull()
		last_phase = PULL
	end
end
