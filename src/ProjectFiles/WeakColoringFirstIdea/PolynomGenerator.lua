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

function main()

	log("Number of colors: " .. getNumberOfColors())

	local d = Statistics.evaluate_polynomial({1,2,3,4,5},5)
	log("Evaluate Polynomial: " .. d)
	
	local primes = Statistics.generate_primes(10000, 20000);
	local pstring = "{ "
	for i = 1 , #primes do
		pstring = pstring .. primes[i] .. ", "
	end
	pstring = pstring .. " }"
	log("Primes: " .. pstring)

end



