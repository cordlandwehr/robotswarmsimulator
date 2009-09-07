--[[
	A GridPullSpin variant that uses only pull/spin operations on the grid but decides it's next move by minimizing the
	euclidean distance (instead of manhattan distance as standard GridPullSpin) to the target point.
--]]


-------------- some helper methods --------------


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


-------------- main program --------------
function main()
	local possible_moves = {
		Vector3d(1, 0, 0), Vector3d(-1, 0, 0), Vector3d(0, 1, 0), Vector3d(0, -1, 0),
		Vector3d(1, 1, 0), Vector3d(-1, 1, 0), Vector3d(-1, -1, 0), Vector3d(1, -1, 0)
	}
	
	local target_point = target_point()
	local min_distance = Geometry.compute_distance(Vector3d(0,0,0), target_point)
	local movement_vector = nil
	for ignore, position in pairs(possible_moves) do
		if (not position_occupied(position)) then
			if (Geometry.compute_distance(position, target_point) < min_distance) then
				min_distance = Geometry.compute_distance(position, target_point)
				movement_vector = position
			end
		end
	end
	
	if (movement_vector ~= nil) then
		View.add_position_request(movement_vector)
	end
end
