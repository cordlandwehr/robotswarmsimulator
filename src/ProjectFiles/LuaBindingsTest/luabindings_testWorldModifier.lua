round = -1
error_count = 0;

function eval(func, obj, val, info)
	if info == nil then
		if obj == val then
			log("debug", tostring(func) .. ": passed ")
		else
			error_count = error_count + 1
			log("error", tostring(func) .. ": failed (" .. tostring(obj) .. " != " .. tostring(val) .. ")")
		end
	else
		if obj == val then
			log("debug", tostring(func) .. ": passed (" .. tostring(info) .. ")")
		else
			error_count = error_count + 1
			log("error", tostring(func) .. ": failed (" .. tostring(info) .. ")(" .. tostring(obj) .. " != " .. tostring(val) .. ")")
		end
	end
end

function main()
	round = round + 1
	
	-- Graph looks like:	0 <-(0)-- 1 --(1)-- 2
	
	
	-- Graph looks like(after round 1): 0 <-(0)-- 1 --(1)-- 2
	--                                    --(2)->
	
	-- Graph looks like(after round 5): 0 <-(0)-- 1         2
	--                                    --(2)->
	
	-- Graph looks like(after end of test):  0 <-(0)-- 1 --(3)-- 2
	
	if round == 0 then
		log("Testing WorldInformation bindings...")
		
		-- testing get_time
		timestep = WorldInformation.get_time()
		eval("get_time()", timestep, 0)

		-- testing get_edges
		all_edges = WorldInformation.get_edges("all")
		eval("get_edges(all)", #all_edges, 2)
		all_edges = WorldInformation.get_edges("directed")
		eval("get_edges(directed)", #all_edges, 1)
		all_edges = WorldInformation.get_edges("undirected")
		eval("get_edges(undirected)", #all_edges, 1)
		
		edges_of_1 = WorldInformation.get_edges(1, "all")
		eval("get_edges(1, all)", #edges_of_1, 2)
		edges_of_1 = WorldInformation.get_edges(1, "directed")
		eval("get_edges(1, directed)", #edges_of_1, 1)
		edges_of_1 = WorldInformation.get_edges(1, "undirected")
		eval("get_edges(1, undirected)", #edges_of_1, 1)

		edges_of_0 = WorldInformation.get_edges(0, "all")
		eval("get_edges(0, all)", #edges_of_0, 1)
		edges_of_1 = WorldInformation.get_edges(0, "directed")
		eval("get_edges(0, directed)", #edges_of_1, 1)
		edges_of_1 = WorldInformation.get_edges(0, "undirected")
		eval("get_edges(0, undirected)", #edges_of_1, 0)

		-- testing add_edges
		local edge_marker = MarkerInformation()
		edge_marker:add_data("test", "test_data")
		WorldInformation.add_edge(0, 1, edge_marker, "directed")
		
		edges_of_0 = WorldInformation.get_edges(0, "all")
		eval("add_edges(0, 1, directed)", #edges_of_0, 2, "same round")
	elseif round == 1 then
		edges_of_0 = WorldInformation.get_edges(0, "all")
		eval("add_edges(0, 1, directed)", #edges_of_0, 2, "next round")
		
		-- Graph looks like 0 <-(0)-- 1 --(1)-- 2
		--                    --(2)->
		
		-- testing marker information
		test_marker = MarkerInformation()
		test_marker_keys = test_marker:get_keys()
		eval("get_keys()", #test_marker_keys, 0)
		
		test_marker:add_data("test1", "test_data")
		test_marker:add_data("test2", 42)
		test_marker:add_data("test3", true)
		test_marker_keys = test_marker:get_keys()
		
		eval("get_keys()", #test_marker_keys, 3)
		eval("has_key()", test_marker:has_key("test1"), true)
		eval("has_key()", test_marker:has_key("bla"), false)
		eval("get_data(test1)", test_marker:get_data("test1"), "test_data")
		eval("get_data(test2)", test_marker:get_data("test2"), 42)
		eval("get_data(test3)", test_marker:get_data("test3"), true)
		
		test_marker:remove_data("test1")
		test_marker:remove_data("bla")
		test_marker_keys = test_marker:get_keys()
		eval("remove_data(test1)", #test_marker_keys, 2)
		
		-- testing get_messages
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		messages_of_1 = WorldInformation.get_messages(1)
		eval("get_messages()", #all_messages, 0)
		eval("get_messages(0)", #messages_of_0, 0)
		eval("get_messages(1)", #messages_of_1, 0)
		
		-- testing add_message
		local message_marker = MarkerInformation()
		message_marker:add_data("test", "test_data")
		WorldInformation.add_message(1, 0, message_marker)
		
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		messages_of_1 = WorldInformation.get_messages(1)
		eval("get_messages()", #all_messages, 1, "same round")
		eval("get_messages(0)", #messages_of_0, 1, "same round")
		eval("get_messages(1)", #messages_of_1, 0, "same round")
	elseif round == 2 then
		all_messages = WorldInformation.get_messages()
		messages_of_0 = WorldInformation.get_messages(0)
		messages_of_1 = WorldInformation.get_messages(1)
		eval("get_messages()", #all_messages, 1, "next round")
		eval("get_messages(0)", #messages_of_0, 1, "next round")
		eval("get_messages(1)", #messages_of_1, 0, "next round")
		
		-- testing get_head
		edge_head = WorldInformation.get_head(0)
		eval("get_head(0)", edge_head, 0)
		
		-- testing get_tail
		edge_tail = WorldInformation.get_tail(0)
		eval("get_tail(0)", edge_tail, 1)

		-- testing get_edge_information
		edge_marker = WorldInformation.get_edge_information(2)
		eval("get_edge_information(2)", edge_marker:get_data("test"), "test_data")
		
		-- testing get_message_information
		message_marker = WorldInformation.get_message_information(0)
		eval("get_message_information(0)", message_marker:get_data("test"), "test_data")
		
		-- testing get_receiver
		receiver = WorldInformation.get_receiver(0)
		eval("get_receiver(0)", receiver, 0)
		
		-- testing get_sender
		sender = WorldInformation.get_sender(0)
		eval("get_sender(0)", sender, 1)
		
		-- testing get_robots
		robots = WorldInformation.get_robots()
		eval("get_robots()", #robots, 3)
		
		-- testing is_directed and is_undirected
		is_dir_0 = WorldInformation.is_directed(0)
		is_undir_0 = WorldInformation.is_undirected(0)
		is_dir_1 = WorldInformation.is_directed(1)
		is_undir_1 = WorldInformation.is_undirected(1)
		eval("is_directed(0)", is_dir_0, true)
		eval("is_undirected(0)", is_undir_0, false)
		eval("is_directed(1)", is_dir_1, false)
		eval("is_undirected(1)", is_undir_1, true)
		
		-- testing set_robot_information and get_robot_information
		local robot_marker = MarkerInformation()
		robot_marker:add_data("test", "test_data")
		WorldInformation.set_robot_information(0, robot_marker)
		
		robot_marker = WorldInformation.get_robot_information(0)
		eval("get_robot_information(0)", robot_marker:get_data("test"), "test_data", "same round")
	elseif round == 3 then
		robot_marker = WorldInformation.get_robot_information(0)
		eval("get_robot_information(0)", robot_marker:get_data("test"), "test_data", "next round")
		
		-- testing set_marker_information
		local edge_marker = MarkerInformation()
		edge_marker:add_data("test", "test_data")
		WorldInformation.set_edge_information(0, edge_marker)
		
		edge_marker = WorldInformation.get_edge_information(0)
		eval("set_edge_information(0)", edge_marker:get_data("test"), "test_data", "same round")
	elseif round == 4 then
		edge_marker = WorldInformation.get_edge_information(0)
		eval("set_edge_information(0)", edge_marker:get_data("test"), "test_data", "next round")
		
		-- testing remove_edge
		WorldInformation.remove_edge(1)
		
		edges_of_1 = WorldInformation.get_edges(1, "all")
		edges_of_2 = WorldInformation.get_edges(2, "all")
		all_edges = WorldInformation.get_edges("all")
		eval("remove_edge(1)", #edges_of_1, 2, "same round")
		eval("remove_edge(1)", #edges_of_2, 0, "same round")
		eval("remove_edge(1)", #all_edges, 2, "same round")
	elseif round == 5 then
		edges_of_1 = WorldInformation.get_edges(1, "all")
		edges_of_2 = WorldInformation.get_edges(2, "all")
		all_edges = WorldInformation.get_edges("all")
		eval("remove_edge(1)", #edges_of_1, 2, "next round")
		eval("remove_edge(1)", #edges_of_2, 0, "next round")
		eval("remove_edge(1)", #all_edges, 2, "next round")
		
		-- Graph looks like: 0 <-(0)-- 1         2
		--                     --(2)->
	
		-- testing remove_message
		WorldInformation.remove_message(0)
		
		messages_of_0 = WorldInformation.get_messages(0)
		all_messages = WorldInformation.get_messages()
		eval("remove_message(0)", #messages_of_0, 0, "same round")
		eval("remove_message(0)", #all_messages, 0, "same round")
	elseif round == 6 then
		messages_of_0 = WorldInformation.get_messages(0)
		all_messages = WorldInformation.get_messages()
		eval("remove_message(0)", #messages_of_0, 0, "next round")
		eval("remove_message(0)", #all_messages, 0, "next round")
		
		-- reset graph for robot test
		WorldInformation.add_edge(1, 2, "undirected")
		WorldInformation.remove_edge(2)
		
		log("Finished WorldInformation bindings test with " .. error_count .. " errors.")
		
		-- Graph looks like: 0 <-(0)-- 1 --(3)-- 2
	end
end
