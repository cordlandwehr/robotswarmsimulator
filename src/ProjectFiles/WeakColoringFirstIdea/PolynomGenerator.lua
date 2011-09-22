
local d = 1

function getNumberOfColors()
	local number_of_colors = 0;
	
	local colors = {}

	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		if marker:has_key(":color") then
			local color = marker:get_data(":color")
			if color ~= nil then
				if colors[color] == nil then
					colors[color] = true
					number_of_colors = number_of_colors + 1				
				end
			end

		end
	end
  
  return number_of_colors
end

function table.equals_table(t1, t2)
	if #t1 ~= #t2 then
		return false
	end
	for k, v in ipairs(t1) do
		if v ~= t2[k] then
			return false
		end
	end
	return true
end

function table.contains_table(t, value)
	for _, v in ipairs(t) do
		if table.equals_table(v, value) then
			return true
		end
	end
	return false
end


function generate_polynomials(degree, modulo, count)
	local polynomials = {}
	while #polynomials < count do
		local poly = {}
		for j = 1, degree do 
			local c = math.random(0, modulo-1)
			table.insert(poly, c)
		end
		if not table.contains_table(polynomials, poly) then
			table.insert(polynomials, poly)
		end
	end
	return polynomials
end

function main()

	local M = getNumberOfColors()
	log("Number of colors: " .. M)
	
	local Delta = Statistics.calculate_degree()
	local Upsilon = Delta-d
	
	if M==0 or Upsilon <= 1 then
		return
	end
	
	log("M=" .. M .. ", Delta=" .. Delta .. ", Upsilon=" .. Upsilon  )
	local kappa = math.ceil(math.log(M)/math.log(Upsilon))
	local q_min = math.floor(kappa*Upsilon+1)
	log("kappa=" .. kappa .. ", qmin=" .. q_min )
	local q = Statistics.generate_primes(q_min, 2*q_min)[1]
	log("q=" .. q)

	local polynomials = generate_polynomials(kappa, q, M)
	local pstrings = {}
	for i = 1, #polynomials do
		local poly = table.concat(polynomials[i], ",")
		table.insert(pstrings, poly)
	end


	local nodes = WorldInformation.get_robots()
	for i = 1 , #nodes do
		marker = WorldInformation.get_robot_information(nodes[i])
		marker:add_data("num_colors", M)
		marker:add_data("polynomials", table.concat(pstrings, ";"))
		WorldInformation.set_robot_information(nodes[i], marker)
	end
end



