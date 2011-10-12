round=0

d = 1

function table.equals_table(t1, t2)
	if #t1 ~= #t2 then
		return false
	end
	for k, v in pairs(t1) do
		if v ~= t2[k] then
			return false
		end
	end
	return true
end

function table.contains_table(t, value)
	for _, v in pairs(t) do
		if table.equals_table(v, value) then
			return true
		end
	end
	return false
end

function table.contains(t, value)
	for _, v in pairs(t) do
		if v == value then
			return true
		end
	end
	return false
end


function generate_polynomials(degree, modulo, colors)
	local polynomials = {}
	
	-- math.randomseed(os.time())
	
	for _, col in ipairs(colors) do
		local poly = {}
		repeat
			for j = 0, degree do 
				local c = math.random(0, modulo-1)
				table.insert(poly, c)
			end
		until not table.contains_table(polynomials, poly)
		
		polynomials[col] = poly
			
	end
	return polynomials
end

function getColors()
	local colors = {}

	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		local color
		if marker:has_key(":color") then
			color = marker:get_data(":color")
		else
			color = i
		end
		if not table.contains(colors, color) then
			table.insert(colors, color)
		end
	end
  
  return colors
end

function main()
	log("---------------------------- Start PolynomGenerator ----------------------------") 
	round = round+1
	log("round: " .. round)
	
	local colors = getColors()
	local M = #colors
	log("Colors(" .. M .."): " .. table.concat(colors, ","))
	if M==0 then
		log("warning", "number of colors=0") 
		log("---------------------------- End PolynomGenerator ----------------------------") 
		return
	end	
	
	local max_defect = math.max(Statistics.calculate_maximal_defect(),0)
	if max_defect>d then
		log("warning", "maximal defect=" .. max_defect .. ">d=" .. d ) 
		log("---------------------------- End PolynomGenerator ----------------------------") 
		return
	end
	
	local Delta = Statistics.calculate_degree()	
	local Upsilon = (Delta-max_defect)/(d+1-max_defect)
		
	if Upsilon <= 1  then
		log("warning", "invalid Upsilon=" .. Upsilon .. "<=1" ) 
		log("---------------------------- End PolynomGenerator ----------------------------") 
		return
	end
		
	local kappa = math.ceil(math.log(M)/math.log(Upsilon))
	local q_min = math.floor(kappa*Upsilon+1)
	local q = Statistics.generate_primes(q_min+1, 2*q_min)[1]
	
	log("Delta=" .. Delta .. ", max_defect=" .. max_defect 	.. ", Upsilon=" .. Upsilon .. ", kappa=" .. kappa .. ", qmin=" .. q_min .. ", q=" .. q )

	local polynomials = generate_polynomials(kappa, q, colors)
	local pstrings = {}
	for c, p in pairs(polynomials) do
		local poly = c .. "|" ..table.concat(p, ",")
		table.insert(pstrings, poly)
	end

	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		marker:add_data("num_colors", M)
		marker:add_data("polynomials", table.concat(pstrings, ";"))
		marker:add_data("prime", q)
		marker:add_data("max_defect", max_defect)
		WorldInformation.set_robot_information(nodes[i], marker)
	end
	log("Polynomials: " .. table.concat(pstrings, ";"))
	
	log("---------------------------- End PolynomGenerator ----------------------------") 
end



