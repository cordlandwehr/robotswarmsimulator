--[[
---- MovingChain Algorithm
----
---- Model requirements:
---- 	* synchronous time model
---- 	* robots having a limited (spherical) view range of 1
---- 	* robots have access to the position of robots they see
---- 	* robots have access to their position and type
----
---- Scenario requirements:
---- 	* there has to be at least one master
----
---- The movement of all masters is determined by a data file (see definition of 'master_moves' below. The file has to
---- be contained in the directory RSS is executed from.
----
---- All robots try allways to wait for their successors. Masters have a fixed path they follow, while slaves try to
---- follow their predecessor. If they can not identify their predecessor, they idle and hope to be able to  identify
---- their predecessor in the next round. The the functions master() and slave() for a more detailed description.
--]]


----------------------------- globals/initialization ---------------------------


-- load the (fixed) path followed by master robots
master_moves    = dofile("4Clique04.dat")
master_moves_nr = table.getn(master_moves)


-- some useful values
zero_vector = Vector3d(0,0,0)
own_id      = get_own_identifier()
own_type    = nil -- View.get_type() seems not to be callable before main is called


-- indicates whether the robot has been initialized
initialized = false


-- last and next moves
next_move = nil
last_move = nil


-- indicates the next step of the fixed path a master robot will do
nr_steps = 1


-- used by slaves to guess the position of their predecessor
current_view        = nil
candidate_positions = nil


-- used by slaves to check whether they have to wait for a successors
has_successor = true -- will be set to false if only one other robot visible in initial position


--------------------------- main program/algorithms ----------------------------


--[[
---- Dispatches the work to the master() or slave() function, depending on the robot's type.
--]]
function main()
	init()
	if (own_type == RobotType.MASTER) then
		master()
	else
		slave()
	end
end


--[[
---- Algorithm for masters:
---- 	* follow the path defined in MovingChainPath.dat
---- 	* however, move only if your last position has been occupied by by another robot
---- 	* the first move (--> no 'last position') is always executed
--]]
function master()
	-- check whether we have traversed the path completely
	if (nr_steps > master_moves_nr) then
		if (nr_steps == master_moves_nr+1) then
			print("\nChain movement ended.\n")
			nr_steps = nr_steps + 1
		end
		return
	end
	
	-- if our successor arrived, we move
	if (successor_arrived()) then
		add_position_request(next_move)
		
		-- update moves
		nr_steps = nr_steps + 1
		last_move = next_move
		next_move = master_moves[nr_steps]
	end
end


--[[
---- Algorithm for the slaves:
---- 	* get the positions of all visible robots --> 'current view'
---- 	* if we do not yet have any 'candidate positions', the current view becomes our candidate positions and we're 
---- 	  done for this time step (we won't move)
---- 	* otherwise
---- 		* if we do have a 'last known predecessor position', check whether our last position has been occupied by
---- 		  another robot; if so: move on to the last known predecessor position, otherwise: idle:
---- 		* otherwise
---- 			* if there is exactly one position in the candidate positions that is not in our current view, this is
---- 			  assumed to be the last known predecessor position
----
---- Note:
---- 	If a robot initially had only one neighbour, it will not wait for another robot to occupy its last position, but
---- 	move on (consider the last robot of a robot chain).
---- 
--]]
function slave()
	current_view = get_visible_positions()
	
	-- if we do not yet know where to go, try to compute a target position
	if (next_move == nil) then
		next_move = get_slave_target_position()
	end
	
	if (next_move ~= nil and position_occupied(next_move) == false) then
		-- if we know where to go (and it is free) and either have no successor or it has already arrived, proceed
		if (not has_successor or successor_arrived()) then
			add_position_request(next_move)
			last_move = next_move
			next_move = nil
		end
	end
end


------------------------------ some helper methods -----------------------------


--[[
---- Initialization of masters and slaves (done only once)
--]]
function init()
	-- do this only once
	if (initialized == true) then
		return
	end
	initialized = true
	
	-- master or slave?
	own_type = View.get_robot_type(own_id)
	if (own_type == RobotType.MASTER) then
		next_move = master_moves[1] -- masters do always know their first move
	else
		-- if a slave sees only one other robot, it assumes to have no succesor to wait for
		local visible_robots = View.get_visible_robots();
		has_successor = (table.getn(visible_robots) > 1)
		
		-- initially, we know that our predecessor is to the right
		candidate_positions = { Vector3d(1,0,0) }
		freed_candidate_positions = { }
	end
end


--[[
---- Returns whether the given position is currently occupied by one of the visible robots.
--]]
function position_occupied(position)
	visible_robots = View.get_visible_robots()
	for ignore, robot in pairs(visible_robots) do
		local robot_position = View.get_position(robot)
		if (math.abs((position - robot_position).x) +
		    math.abs((position - robot_position).y) +
		    math.abs((position - robot_position).z) < 0.000000001) then
			return true
		end
	end
	return false
end


--[[
---- Returns whether a possible successor arrived, i.e. whether the position we came from is occupied by a visible 
---- robot. If there is no position we came from (--> first move), true is returned.
--]]
function successor_arrived()
	return (last_move == nil) or position_occupied(zero_vector - last_move)
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
	-- if we do not have any candidates for next move, set them to the current view and return nil (idle one step)
	if (candidate_positions == nil) then
		candidate_positions = current_view
		freed_candidate_positions = { }		
		return nil
	end
	
	-- if we have candidates, compute the ones that have become free (so that we can possibly move there)
	local tmp_candidate_positions = { }
	for ignore, candidate_position in pairs(candidate_positions) do
		if (position_occupied(candidate_position) == false) then
			table.insert(freed_candidate_positions, candidate_position)
		else
			table.insert(tmp_candidate_positions, candidate_position)
		end
	end
	candidate_positions = tmp_candidate_positions
	
	-- freed candidate positions that have been occupied once more can be rejected
	local tmp_freed_candidate_positions = { }
	for ignore, freed_candidate_position in pairs(freed_candidate_positions) do
		if (position_occupied(freed_candidate_position) == false) then
			table.insert(tmp_freed_candidate_positions, freed_candidate_position)
		end
	end
	freed_candidate_positions = tmp_freed_candidate_positions
	
	-- if there is exactly one candidate left (freed or not), we have found our predecessor
	if (table.getn(candidate_positions) + table.getn(freed_candidate_positions) ~= 1) then
		return nil
	end
		
	if (table.getn(candidate_positions) == 1) then
		result_candidate = candidate_positions[1]
		candidate_positions = nil
		return result_candidate
	elseif (table.getn(freed_candidate_positions) == 1) then
		candidate_positions = nil
		return freed_candidate_positions[1]
	end
	
	return nil -- several feasible candidates --> we don't know what to do, so idle one step
end
