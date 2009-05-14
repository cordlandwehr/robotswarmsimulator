--[[
---- MovingChain Algorithm
---- TODO(peter): description
--]]


----------------------------- globals/initialization ---------------------------


master_moves = {
	Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0),
	Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0),
	Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0), Vector3d(1,0,0)
}
master_moves_nr = table.getn(master_moves)

zero_vector = Vector3d(0,0,0)

own_id = get_own_identifier()

nr_steps = 1

current_view = nil
candidate_positions    = nil


------------------------------ some helper methods -----------------------------


--[[
---- Returns whether the last master position is currently occupied by one of the visible robots.
--]]
function last_master_position_occupied()
	visible_robots = View.get_visible_robots()
	for ignore, robot in pairs(visible_robots) do
		local robot_position = View.get_position(robot)
		if (master_moves[nr_steps-1] + robot_position == zero_vector) then
			return true
		end
	end
	return false
end


--[[
---- Returns whether the given position is currently occupied by one of the visible robots.
--]]
function position_occupied(position)
	visible_robots = View.get_visible_robots()
	for ignore, robot in pairs(visible_robots) do
		local robot_position = View.get_position(robot)
		if (position == robot_position) then
			return true
		end
	end
	return false
end


--[[
---- Returns a table containing the positions of all currently visible robots.
--]]
function get_visible_positions()
	local visible_robots = View.get_visible_robots()
	local visible_positions = { }
	for ignore, robot in pairs(visible_robots) do
		table.insert(visible_positions, View.get_position(robot))
	end
	return visible_positions
end


--[[
---- Algorithm that computes the next position (or nil) for slave robots.
--]]
function get_slave_target_position()
	if (candidate_positions == nil) then
		candidate_positions = current_view
		return nil
	end
	
	local feasible_candidates = { }
	for ignore, candidate_position in pairs(candidate_positions) do
		if (position_occupied(candidate_position) == false) then
			table.insert(feasible_candidates, candidate_position)
		end
	end
	
	local feasible_candidates_nr = table.getn(feasible_candidates)
	if (feasible_candidates_nr == 1) then
		return feasible_candidates[1]
	end
	return nil
end


--------------------------- main program/algorithms ----------------------------


--[[
---- Dispatches the work to the master() or slave() function, depending on the robot's type.
--]]
function main()
	local type = get_robot_type(own_id)
	if (type == RobotType.MASTER) then
		master()
	else
		slave()
	end
end


--[[
---- TODO(peter): description
--]]
function master()
	if (nr_steps > master_moves_nr) then
		if (nr_steps == master_moves_nr+1) then
			print("\nChain movement ended.\n")
		end
		nr_steps = nr_steps + 1
		return
	end

	if (nr_steps == 1 or last_master_position_occupied()) then
		add_position_request(master_moves[nr_steps])
		nr_steps = nr_steps + 1
	end
end


--[[
---- TODO(peter): description
--]]
function slave()
	current_view = get_visible_positions()
	
	local target_position = get_slave_target_position()
	if (target_position ~= nil) then
		add_position_request(target_position)
		candidate_positions = nil
	end
end
