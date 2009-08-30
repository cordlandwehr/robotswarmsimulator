--[[
	GridPullSpin
	This is an implementation of the PullSpin aproach on a grid. The grid is spanned by the vectors (1,0) and (0,1).
	Push operations will move either horizontally or vertically towards the target point and therefore decrease the L1
	distance by exactly one. Spin operations on the other hand move one unit horizontally and vertically, such that the
	L1 distance to the target point remains unchanged.
	
	Note that this algorithm does operate on the XY-plane only. Thus, any z-coordinate should be set to zeor.
--]]


-------------- some helper methods --------------

--[[
---- Computes the l1 norm of the given vector
--]]
function l1norm(input_vector)
	return math.abs(input_vector.x) + math.abs(input_vector.y)
end

--[[
---- Computes the l1 distance of the given vectors
--]]
function l1distance(v1, v2)
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
---- Returns the target point in the robot's local coordinate system.
--]]
function target_point()
	-- this does currently rely on the robots having a standard coordinate system which is simply translated by their
	-- position (not rotated/scaled)
	return Vector3d(0,0,0) - View.get_robot_coordinate_system_origin(View.get_own_identifier())
end


--[[
---- Returns whether the given position is currently occupied by one of the visible robots (or by the robot itself).
--]]
function position_occupied(position)
	if (position.x == 0 and position.y == 0 and position.z == 0) then
		return true
	end
	
	local visible_robots = View.get_visible_robots()
	for ignore, robot in pairs(visible_robots) do
		local robot_position = View.get_position(robot)
		if (robot_position.x == position.x and robot_position.y == position.y and robot_position.z == position.z) then
			return true
		end
	end
	return false
end

--[[
---- Returns true if there is a robot standing at delta_position+position and this robot will perform a pull towards position.
--]]
function robot_pulling_at_position(delta_position, position)
	local robot_position = delta_position + position
	if (not position_occupied(robot_position) or position_occupied(position)) then
		return false
	end
	
	local local_target   = target_point() - robot_position
	local local_position = position - robot_position
	
	if (l1norm(local_position) ~= 1) then
		return false
	end
	
		
	if (local_target.x == 0) then
		return sign(local_target.y) == local_position.y
	end
	
	if (local_target.y == 0) then
		return sign(local_target.x) == local_position.x
	end
	
	
	if (sign(local_target.x) == local_position.x) then
		return true
	elseif (sign(local_target.y) == local_position.y and
		    position_occupied(robot_position + Vector3d(sign(local_target.x), 0, 0))) then
		return true
	else
		return false
	end
end


-------------- main program --------------
function main()
	local movement_vector = nil
	local target_point = target_point()
	
	-- compute possible pull movements
	local pull1 = Vector3d(sign(target_point.x), 0, 0)
	local pull2 = Vector3d(0, sign(target_point.y), 0)
	
	-- compute possible spin movements
	local spin = Vector3d(0,0,0)
	if (target_point.x < 0 and target_point.y <= 0) then
		spin = Vector3d(-1, 1, 0)
	elseif (target_point.x >= 0 and target_point.y < 0) then
		spin = Vector3d(-1, -1, 0)
	elseif (target_point.x > 0 and target_point.y >= 0) then
		spin = Vector3d(1, -1, 0)
	elseif (target_point.x <= 0 and target_point.y > 0) then
		spin = Vector3d(1, 1, 0)
	end
	
	-- now check whether we can pull along x, y or spin (in that order!)
	if (not position_occupied(pull1)) then
		movement_vector = pull1
		add_color_change_request(1)
	elseif (not position_occupied(pull2)) then
		movement_vector = pull2
		add_color_change_request(2)
	elseif (not position_occupied(spin)) then
		-- spin is a bit more complicate, because we manually have to check whether another robot is trying a pull on
		-- our target position (in this case, we resign)
		local higher_priority_candidates = { Vector3d(1,0,0), Vector3d(0,1,0), Vector3d(-1,0,0), Vector3d(0,-1,0) }
		
		if (not robot_pulling_at_position(Vector3d(1,0,0), spin) and
			not robot_pulling_at_position(Vector3d(0,1,0), spin) and
			not robot_pulling_at_position(Vector3d(-1,0,0), spin) and
			not robot_pulling_at_position(Vector3d(0,-1,0), spin)) then
			movement_vector = spin
			add_color_change_request(3)
		end
	end
	
	if (movement_vector ~= nil) then
		View.add_position_request(movement_vector)
	else
		add_color_change_request(0)
	end
end
