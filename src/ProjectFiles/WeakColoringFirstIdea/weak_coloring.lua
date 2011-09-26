-- colors 0, .., available_color-1 are avaiable
d = 2;

function string:split(sep)
	local sep, fields = sep or ":", {}
	local pattern = string.format("([^%s]+)", sep)
	self:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end

function main()
	me = View.get_own_id()
	my_marker = View.get_robot_information(me)
	
	if not my_marker:has_key("polynomials") then
		return
	end
	
	pstrings = my_marker:get_data("polynomials")
	M = my_marker:get_data("num_colors")
	q = my_marker:get_data("prime")
	
	if my_marker:has_key(":color") then
		my_color = my_marker:get_data(":color")
	else
		my_color = me + 1
	end
	log("my_color = " .. my_color)
	
	-- decode polynomials
	log("pstrings = " .. pstrings)
	local psplitstrings = pstrings:split(";")
	log ("#psplitstrings = " .. #psplitstrings)
	local polynomials = {}
	for i = 1, #psplitstrings do
		local pstring = psplitstrings[i]
		local pcoefficientstrings = pstring:split(",")
		local pcoefficients = {}
		for j =1, #pcoefficientstrings do
			pcoefficients[j] = tonumber(pcoefficientstrings[j])		
		end
		polynomials[i] = pcoefficients
	end
	
	
	-- find alpha with least defects
	local robots = View.get_visible_robots()
	
	local defects = 10000000;
	local best_alpha = 0;
	local best_phi_result = 0;
	for alpha = 0, q -1 do
		local nodes = View.get_visible_robots()
		
		log("polynomials[my_color] = " .. table.concat( polynomials[my_color], ":"))
		
		local phi_result = Statistics.evaluate_polynomial(polynomials[my_color], alpha)
		
		if (phi_result >= q) then
			log("error: phi_result must not be bigger than q")
		end
		
		local new_defects = 0;
		
		for i = 1 , #nodes do
			marker = View.get_robot_information(nodes[i])
			if marker:has_key(":color") then			
				color = marker:get_data(":color")
			else
				color = nodes[i]
			end
			neighbor_phi = Statistics.evaluate_polynomial(polynomials[color], alpha)
			if phi_result == neighbor_phi then
				new_defects = new_defects + 1
			end
		end	
		if new_defects < defects then
			defects = new_defects
			best_alpha = alpha
			best_phi_result = phi_result
		end		
	end
	
	local my_color = best_alpha * q + best_phi_result + 1
	
	my_marker:add_data(":color", my_color)
	
	log("Node " .. me .. " set its color to (" .. best_alpha .. ", " .. best_phi_result .. ") = " .. my_color);	

	View.add_marker_request(my_marker)
end