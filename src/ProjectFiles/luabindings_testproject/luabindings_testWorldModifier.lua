round = -1
error_count = 0;

function main()
	round = round + 1
	-- Graph looks like:	0 <-(0)-- 1 --(1)-- 2
	if round == 0 then
		log("Testing WorldInformation bindings...")
		
		-- testing get_time
		timestep = WorldInformation.get_time()
		if timestep == 0 then
			log("debug", "get_time(): passed")
		else
			log("error", "get_time(): failed (" .. timestep .. ") != 0)")
		end

		-- testing get_edges
		all_edges = WorldInformation.get_edges("all")
		if #all_edges == 2 then
			log("debug", "get_edges(all): passed")
		else
			log("error", "get_edges(all): failed (" .. #all_edges .. "!=2)")
		end
		all_edges = WorldInformation.get_edges("directed")
		if #all_edges == 1 then
			log("debug", "get_edges(directed): passed")
		else
			log("error", "get_edges(directed): failed (" .. #all_edges .. "!=1)")
		end
		all_edges = WorldInformation.get_edges("undirected")
		if #all_edges == 1 then
			log("debug", "get_edges(undirected): passed")
		else
			log("error", "get_edges(undirected): failed (" .. #all_edges .. "!=1)")
		end
		
		edges_of_1 = WorldInformation.get_edges(1, "all")
		if #edges_of_1 == 2 then
			log("debug", "get_edges(1, all): passed")
		else
			log("error", "bla")
			log("error", "get_edges(1, all): failed (" .. #edges_of_1 .. "!=2)")
		end
		edges_of_1 = WorldInformation.get_edges(1, "directed")
		if #edges_of_1 == 1 then 
			log("debug", "get_edges(1, directed): passed")
		else
			log("error", "get_edges(1, directed): failed (" .. #edges_of_1 .. "!=1)")
		end
		edges_of_1 = WorldInformation.get_edges(1, "undirected")
		if #edges_of_1 == 1 then
			log("debug", "get_edges(1, undirected): passed")
		else
			log("error", "get_edges(1, undirected): failed (" .. #edges_of_1 .. "!=1)")
		end

		edges_of_0 = WorldInformation.get_edges(0, "all")
		if #edges_of_0 == 1 then
			log("debug", "get_edges(0, all): passed")
		else
			log("error", "get_edges(0, all): failed (" .. #edges_of_0 .. "!=1)")
		end

		-- testing add_edges
		local edge_marker = MarkerInformation()
		edge_marker:add_data("test", "test_data")
		WorldInformation.add_edge(0, 1, edge_marker, "directed")
		
		edges_of_0 = WorldInformation.get_edges(0, "all")
		if #edges_of_0 == 2 then
			log("debug", "add_edges(0, 1, directed): passed (same round)")
		else
			log("error", "add_edges(0, 1, directed): failed (same round)(" .. #edges_of_0 .. "!=2)")
		end
	elseif round == 1 then
		edges_of_0 = WorldInformation.get_edges(0, "all")
		if #edges_of_0 == 2 then
			log("debug", "add_edges(0, 1, directed): passed (next round)")
		else
			log("error", "add_edges(0, 1, directed): failed (next round)(" .. #edges_of_0 .. "!=2)")
		end
		
		-- Graph looks like 0 <-(0)-- 1 --(1)-- 2
		--                    --(2)->
		
		-- testing get_messages
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		if #all_messages == 0 and #messages_of_0 == 0 then
			log("debug", "get_messages(): passed")
		else
			log("error", "get_message(): failed (" .. #all_messages .. "!=0)")
		end
		
		-- testing add_message
		local message_marker = MarkerInformation()
		message_marker:add_data("test", "test_data")
		WorldInformation.add_message(1, 0, message_marker)
		
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		if #all_messages == 1 and #messages_of_0 == 1 then
			log("debug", "add_message(1, 0, new_marker): passed (same round)")
		else
			log("error", "add_message(1, 0, new_marker): failed (same round)(" .. #all_messages .. "!=1)")
		end
	elseif round == 2 then
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		if #all_messages == 1 and #messages_of_0 == 1 then
			log("debug", "add_message(1, 0, new_marker): passed (next round)")
		else
			log("error", "add_message(1, 0, new_marker): failed (next round)(" .. #all_messages .. "!=1 or " .. #messages_of_0 .. "!=1)")
		end
		
		-- testing get_head
		edge_head = WorldInformation.get_head(0)
		if  edge_head == 0 then
			log("debug", "get_head(0): passed")
		else
			log("error", "get_head(0): failed (" .. edge_head .. "!=0)")
		end
		
		-- testing get_tail
		edge_tail = WorldInformation.get_tail(0)
		if  edge_tail == 1 then
			log("debug", "get_tail(0): passed")
		else
			log("error", "get_tail(0): failed (" .. edge_tail .. "!=1)")
		end

		-- testing get_edge_information
		edge_marker = WorldInformation.get_edge_information(2)
		if edge_marker:get_data("test") == "test_data" then
			log("debug", "get_edge_information(2): passed")
		else
			log("error", "get_edge_information(2): failed (" .. edge_marker:get_data("test") .. "!=\"test_data\")")
		end
		
		-- testing get_message_information
		message_marker = WorldInformation.get_message_information(0)
		if message_marker:get_data("test") == "test_data" then
			log("debug", "get_message_information(0): passed")
		else
			log("error", "get_message_information(0): failed (" .. message_marker:get_data("test") .. "!=\"test_data\")")
		end
		
		-- testing get_receiver
		receiver = WorldInformation.get_receiver(0)
		if receiver == 0 then
			log("debug", "get_receiver(0): passed")
		else
			log("error", "get_receiver(0): failed (" .. receiver .. "!=0)")
		end
		
		-- testing get_sender
		sender = WorldInformation.get_sender(0)
		if sender == 1 then
			log("debug", "get_sender(0): passed")
		else
			log("error", "get_sender(0): failed (" .. sender .. "!=1)")
		end	
		
		-- testing get_robots
		robots = WorldInformation.get_robots()
		if #robots == 3 then
			log("debug", "get_robots(): passed")
		else
			log("error", "get_robots(): failed (" .. #robots .. "!=3)")
		end
		
		-- testing is_directed and is_undirected
		is_dir_0 = WorldInformation.is_directed(0)
		is_undir_0 = WorldInformation.is_undirected(0)
		is_dir_1 = WorldInformation.is_directed(1)
		is_undir_1 = WorldInformation.is_undirected(1)
		if is_dir_0 and not is_undir_0 and not is_dir_1 and is_undir_1 then
			log("debug", "is_directed/is_undirected: passed")
		else
			log("error", "is_directed/is_undirected: failed (wrong type)")
		end
		
		-- testing set_robot_information and get_robot_information
		local robot_marker = MarkerInformation()
		robot_marker:add_data("test", "test_data")
		WorldInformation.set_robot_information(0, robot_marker)
		
		robot_marker = WorldInformation.get_robot_information(0)
		if robot_marker:get_data("test") == "test_data" then
			log("debug", "set_robot_information(0, robot_marker)/get_robot_information(0): passed (same round)")
		else
			log("error", "set_robot_information(0, robot_marker)/get_robot_information(0): failed (same round)(" .. robot_marker:get_data("test") .. "!=\"test_data\")")
		end
	elseif round == 3 then
		robot_marker = WorldInformation.get_robot_information(0)
		if robot_marker:get_data("test") == "test_data" then
			log("debug", "set_robot_information(0, robot_marker)/get_robot_information(0): passed (next round)")
		else
			log("error", "set_robot_information(0, robot_marker)/get_robot_information(0): failed (next round)(" .. robot_marker:get_data("test") .. "!=\"test_data\")")
		end
		
		-- testing set_marker_information
		local edge_marker = MarkerInformation()
		edge_marker:add_data("test", "test_data")
		WorldInformation.set_edge_information(0, edge_marker)
		
		edge_marker = WorldInformation.get_edge_information(0)
		if edge_marker:get_data("test") == "test_data" then
			log("debug", "set_edge_information(0, edge_marker): passed (same round)")
		else
			log("error", "set_ede_information(0, edge_marker): failed (same round)(" .. edge_marker:get_data("test") .. "!=\"test_data\")")
		end
	elseif round == 4 then
		edge_marker = WorldInformation.get_edge_information(0)
		if edge_marker:get_data("test") == "test_data" then
			log("debug", "set_edge_information(0, edge_marker): passed (next round)")
		else
			log("error", "set_ede_information(0, edge_marker): failed (next round)(" .. edge_marker:get_data("test") .. "!=\"test_data\")")
		end
		
		-- testing remove_edge
		WorldInformation.remove_edge(1)
		
		edges_of_1 = WorldInformation.get_edges(1, "all")
		edges_of_2 = WorldInformation.get_edges(2, "all")
		all_edges = WorldInformation.get_edges("all")
		if #edges_of_1 == 2 and #edges_of_2 == 0 and #all_edges == 2 then
			log("debug", "remove_edge(1): passed (same round)")
		else
			log("error", "remove_edge(1): failed (same round)(" .. #edges_of_1 .. "!=2 or " .. #edges_of_2 .. "!=0 or " .. #all_edges .. "!=2)")
		end
	elseif round == 5 then
		edges_of_1 = WorldInformation.get_edges(1, "all")
		edges_of_2 = WorldInformation.get_edges(2, "all")
		all_edges = WorldInformation.get_edges("all")
		if #edges_of_1 == 2 and #edges_of_2 == 0 and #all_edges == 2 then
			log("debug", "remove_edge(1): passed (next round)")
		else
			log("error", "remove_edge(1): failed (next round)(" .. #edges_of_1 .. "!=2 or " .. #edges_of_2 .. "!=0 or " .. #all_edges .. "!=2)")
		end
	
		-- testing remove_message
		WorldInformation.remove_message(0)
		
		messages_of_0 = WorldInformation.get_messages(0)
		all_messages = WorldInformation.get_messages()
		if #messages_of_0 == 0 and #all_messages == 0 then
			log("debug", "remove_message(0): passed (same round)")
		else
			log("error", "remove_message(0): failed (same round)(" .. #messages_of_0 .. "!=0 or " .. #all_messages .. "!=0)")
		end
	elseif round == 6 then
		messages_of_0 = WorldInformation.get_messages(0)
		all_messages = WorldInformation.get_messages()
		if #messages_of_0 == 0 and #all_messages == 0 then
			log("debug", "remove_message(0): passed (next round)")
		else
			log("error", "remove_message(0): failed (next round)(" .. #messages_of_0 .. "!=0 or " .. #all_messages .. "!=0)")
		end
	end
end
