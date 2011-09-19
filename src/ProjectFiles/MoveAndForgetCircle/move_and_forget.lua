long_range_link = nil
neighbors = nil
me = nil
my_marker = nil

function forget_long_range_link(creation_time)
	local age = (View.get_time()-creation_time)
	-- log("info", "[move_and_forget.lua][Robot #" .. me .. "] Age " .. age .. ".") 
	return math.random() < (1/(age*0.275))
end

function handle_message(msg)
	-- get message marker
	local msg_marker = View.get_message_information(msg)
	assert(msg_marker:has_key("type"))
	local msg_type = msg_marker:get_data("type")
	-- process actual message
	if msg_type == "ask_neighbor" then
		-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Handling 'ask_neighbor' message.") 
		-- sender of the message wants a new long range neighbor, chose from my neighbors
		local new_marker = MarkerInformation()
		new_marker:add_data("type", "tell_neighbor")
		-- TODO: Use random generator or the simulator..
		new_marker:add_data("neighbor", neighbors[math.random(#neighbors)])
		-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Selected neighbor = " .. new_marker:get_data("neighbor") .. ".") 
		-- send answer back to sender
		View.add_send_message_request(View.get_sender(msg), new_marker)
	elseif msg_type == "tell_neighbor" then
		-- somebody send me a new neighbor
		-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Handling 'tell_neighbor' message.") 
		if my_marker:get_data("status") == "listening" then		
			-- remove existing and request new long range link
			local new_marker = nil
			if (long_range_link) then
				new_marker = View.get_edge_information(long_range_link)
				View.add_remove_edge_request(long_range_link)
			else
				new_marker = MarkerInformation()
				new_marker:add_data("long_range_link", true)
				new_marker:add_data("creation_time", View.get_time())
			end
			View.add_insert_edge_request(me, msg_marker:get_data("neighbor"), new_marker, "undirected")
			-- change status back to ask
			my_marker:add_data("status", "ask")
		else
			-- log("error", "[move_and_forget.lua][Robot #" .. me .. "] Received unexpected 'tell_neighbor' message!")
		end
	else
		-- log("error", "[move_and_forget.lua][Robot #" .. me .. "] Unsupported message type!")
	end
	-- remove the message
	View.add_remove_message_request(msg)
end

function print_edges()
	-- print all edges
	local edges = View.get_visible_edges()
	-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Edge overview:")
	for i, edge in ipairs(edges) do
		local marker = View.get_edge_information(edge)
		local link = ""
		if marker:has_key("long_range_link") then
			link = " (long range link)"
		end
		if View.is_directed(edge) then
			-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Directed edge (#" .. edge .. ") from " .. View.get_tail(edge) .. " to " .. View.get_head(edge) .. link .. ".")
		else
			-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Undirected edge (#" .. edge .. ") between " .. View.get_tail(edge) .. " and " .. View.get_head(edge) .. link .. ".")
		end		
	end
end

function update_links()
	-- update my own ID (should not change in between rounds, but who knows)
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)
	-- check visible edges (search for long range link)
	neighbors = nil
	long_range_link = nil
	local edges = View.get_visible_edges()
	for i, edge in ipairs(edges) do
		local marker = View.get_edge_information(edge)
		if marker:has_key("long_range_link") then
			-- found long range link, is it mine?
			if View.get_tail(edge) == me then
				long_range_link = edge
			end
		else
			-- get correct robot ID (not me)
			local neighbor = View.get_tail(edge)
			if neighbor == me then
				neighbor = View.get_head(edge)
			end
			-- edge is no long range link, create new neighbor set or add to existing
			if not neighbors then
				neighbors = {neighbor}
			else
				neighbors[#neighbors+1] = neighbor
			end
		end
	end
end

function main()
	-- update links / IDs
	update_links()
	
	-- say who I am
	-- log("debug", "Running move_and_forget.lua ... (Robot #" .. me .. ").")
	
	-- check whether I have exaclty TWO non long range neighbors (circle...)
	if (#neighbors ~= 2) then
		-- log("error", "[move_and_forget.lua][Robot #" .. me .. "] I seem to have " .. #neighbors .. " neighbors!")
	end
	
	-- initialize marker if empty
	if (#my_marker:get_keys() == 0) then
		my_marker:add_data("status", "ask")
	end
	
	-- print my edges
	print_edges()
	 
	-- check my status
	if my_marker:get_data("status") == "ask" then
		-- this is an ASK round
		-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Asking for new long range neighbor.") 
		-- do create new marker for message
		local new_marker = MarkerInformation()
		new_marker:add_data("type", "ask_neighbor")
		-- do I have an existing long range neighbor?
		if (long_range_link) then
			local lr_marker = View.get_edge_information(long_range_link)
			if forget_long_range_link(lr_marker:get_data("creation_time")) then
				View.add_remove_edge_request(long_range_link)
				local new_marker = MarkerInformation()
				new_marker:add_data("long_range_link", true)
				new_marker:add_data("creation_time", View.get_time())
				View.add_insert_edge_request(me, me, new_marker, "undirected")
			else
				-- log("debug", "[move_and_forget.lua][Robot #" .. me .. "] LRL: " .. View.get_head(long_range_link)) 
				-- send message to my long range link
				View.add_send_message_request(View.get_head(long_range_link), new_marker)
				-- change status to wait for 'tell_neighbor' message
				my_marker:add_data("status", "listening")
			end
		else
			-- create self loop (initialization)
			local new_marker = MarkerInformation()
			new_marker:add_data("long_range_link", true)
			new_marker:add_data("creation_time", View.get_time())
			View.add_insert_edge_request(me, me, new_marker, "undirected")
		end
	end
	
	-- handle all messages
	for i = 1, View.get_number_of_messages() do
		-- loop goes from 1 .. n, message indices from 0 .. n-1
		handle_message(View.get_message(i-1))
	end 
	
	-- create request to update my own marker
	View.add_marker_request(my_marker)
end