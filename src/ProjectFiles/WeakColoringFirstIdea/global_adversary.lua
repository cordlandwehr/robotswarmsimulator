
function main()
	log("---------------------------- Start Adversary ----------------------------") 
	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		if marker:has_key("role") then
			local role = marker:get_data("role")
			if role == "private" then
				local edges = WorldInformation.get_edges(nodes[i], "")
				local colors = {}
				for j = 1 , #edges do
					local neighbour = WorldInformation.get_head(edges[i])
					if neighbour == nodes[i] then
						neighbour = WorldInformation.get_tail(edges[i])
					end
					local n_marker = WorldInformation.get_robot_information(neighbour)
					if n_marker:has_key(":color") then
						table.insert(colors, n_marker:get_data(":color"))
					end
				end
				if #colors > 0 then
					marker:add_data(":color", colors[1])
				end
				WorldInformation.set_robot_information(nodes[i], marker)
			end
		end
	end	
	log("---------------------------- End Adversary ----------------------------") 
end



