round = -1
error_count = 0

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
	if View.get_time() > 40 then
		round = round + 1
	end
	
	if round == 0 then
	
		if View.get_own_id() == 0 then
			log("Testing View bindings...")
		end
		
		-- testing if included robot exists
		if View.get_own_id() == 3 then
			log("Added robot gets executed")
		end
		
		-- Graph looks like:  0 <-(0)-- 1 --(3)-- 2
		
		-- testing get_time
		if View.get_own_id() == 0 then
			time_0 = View.get_time()
			eval("0.get_time()", time_0, 41, "")
		elseif View.get_own_id() == 1 then
			time_1 = View.get_time()
			eval("1.get_time()", time_1, 41, "")
		elseif View.get_own_id() == 2 then
			time_2 = View.get_time()
			eval("2.get_time()", time_2, 41, "")
		end
		
		-- testing get_visible_edges
		if View.get_own_id() == 0 then
			edges_of_0 = View.get_visible_edges()
			eval("0.get_visible_edges()", #edges_of_0, 0)
		elseif View.get_own_id() == 1 then
			edges_of_1 = View.get_visible_edges()
			eval("1.get_visible_edges()", #edges_of_1, 2)
		elseif View.get_own_id() == 2 then
			edges_of_2 = View.get_visible_edges()
			eval("2.get_visible_edges()", #edges_of_2, 1)
		end
		
		-- testing get_visible_robots
		if View.get_own_id() == 0 then
			robots_of_0 = View.get_visible_robots()
			eval("0.get_visible_robots()", #robots_of_0, 0)
		elseif View.get_own_id() == 1 then
			robots_of_1 = View.get_visible_robots()
			eval("1.get_visible_robots()", #robots_of_1, 2)
		elseif View.get_own_id() == 2 then
			robots_of_2 = View.get_visible_robots()
			eval("2.get_visible_robots()", #robots_of_2, 1)
		end
		
		-- testing is_directed is_undirected
		if View.get_own_id() == 1 then
			eval("1.is_directed(0)", View.is_directed(0), true)
			eval("1.is_undirected(0)", View.is_undirected(0), false)
			eval("1.is_directed(1)", View.is_directed(3), false)
			eval("1.is_undirected(1)", View.is_undirected(3), true)
		end
		
		-- testing get_head get_tail
		if View.get_own_id() == 1 then
			eval("1.get_head(0)", View.get_head(0), 0)
			eval("1.get_tail(0)", View.get_tail(0), 1)
			eval("1.get_head(2)", View.get_head(3), 2)
			eval("1.get_tail(2)", View.get_tail(3), 1)
		end
		
		-- testing add_insert_message_request
		if View.get_own_id() == 1 then
			message_marker = MarkerInformation()
			message_marker:add_data("test", "test_data")
			View.add_send_message_request(0, message_marker)
		end
		
	elseif round == 1 then
		-- testing add_insert_message_request, get_message, get_number_of_messages, get_message_information, get_last_request_successful, get_sender
		if View.get_own_id() == 0 then
			eval("0.get_number_of_messages()", View.get_number_of_messages(), 1, "message arrived?")
			eval("0.get_message(0)", View.get_message(0), 1)
			mm = View.get_message_information(1)
			eval("0.get_message_information(1)", mm:get_data("test"), "test_data")
			eval("0.get_sender(1)", View.get_sender(1), 1)
		elseif View.get_own_id() == 1 then
			eval("1.get_robot_last_request_successful()", View.get_robot_last_request_successful(1), true)
		end
		
		-- testing add_insert_edge_request
		if View.get_own_id() == 0 then
			em = MarkerInformation()
			em:add_data("test", "test_data")
			View.add_insert_edge_request(0, 1, em, "undirected")
		end
	elseif round == 2 then
		-- Graph looks like:  0 <-(0)-- 1 --(3)-- 2
		--                      --(4)--
		if View.get_own_id() == 0 then
			edges_of_0 = View.get_visible_edges()
			eval("0.add_insert_edge_request(0, 1, em, undirected)", #edges_of_0, 1)
			em = View.get_edge_information(4)
			eval("0.get_edge_information(4)", em:get_data("test"), "test_data")
		elseif View.get_own_id() == 1 then
			edges_of_1 = View.get_visible_edges()
			eval("1.add_insert_edge_request(0, 1, em, undirected)", #edges_of_1, 3)
		elseif View.get_own_id() == 2 then
			edges_of_2 = View.get_visible_edges()
			eval("2.add_insert_edge_request(0, 1, em, undirected)", #edges_of_2, 1)
		end
		
		-- testing add_marker_request, get_robot_information
		if View.get_own_id() == 2 then
			rm = MarkerInformation()
			rm:add_data("test", "test_data")
			View.add_marker_request(rm)
		end
	elseif round == 3 then
		if View.get_own_id() == 2 then
			rm = View.get_robot_information(2)
			eval("2.add_marker_request(rm)", rm:get_data("test"), "test_data")
		end
		
		-- testing add_remove_message_request
		if View.get_own_id() == 0 then
			View.get_message(0)
			View.add_remove_message_request(1)
		end
	elseif round == 4 then
		if View.get_own_id() == 0 then
			eval("0.add_remove_message_request(1)", View.get_number_of_messages(), 0)
		end
		
		-- testing add_remove_edge_request
		if View.get_own_id() == 1 then
			View.get_visible_edges()
			View.add_remove_edge_request(4)
		end
	elseif round == 5 then
		if View.get_own_id() == 0 then
			edges_of_0 = View.get_visible_edges()
			eval("0.add_remove_edge_request()", #edges_of_0, 0)
		elseif View.get_own_id() == 1 then
			edges_of_1 = View.get_visible_edges()
			eval("1.add_remove_edge_request()", #edges_of_1, 2)
		elseif View.get_own_id() == 2 then
			edges_of_2 = View.get_visible_edges()
			eval("2.add_remove_edge_request()", #edges_of_2, 1)
		end
	elseif round == 6 then
		log("Finished test of robot bindings for robot " .. View.get_own_id() .. " with " .. error_count .. " errors. (Check for \"Added robot gets executed\" message at beginning)")
	end
end