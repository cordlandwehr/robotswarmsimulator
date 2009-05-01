-- as it seems, global vector variables are not definable outside a function :-/ --> declare the vector components one by one
position_diff_x = 0.1;
position_diff_y = 0.0;
position_diff_z = 0.0;

function main()
	type = get_robot_type(get_own_identifier());
	if (type == 0) then -- master
		if (get_robot_last_request_successful(get_own_identifier()) == 0) then -- last request was not successful --> collision (assumes that we are not using inaccurate/discarding vector modifiers or similar things)
			position_diff_x = -1 * position_diff_x;
		end
		local position_diff = Vector3d(position_diff_x, position_diff_y, position_diff_z);
		add_position_request(position_diff);
	else -- slave
		-- sit down and relax... (this is unusale for slaves, isn't it?)
	end
end

