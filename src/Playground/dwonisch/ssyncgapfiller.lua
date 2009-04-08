--[[
	LATTICE
	Script for building a lattice in 2 dimensions
	Algorithm works for each robot as follows:
	1. Robot identifies his nearest neighbour x_1
		the line from x to the origin marks the anglebisector of the 1st sector,
		defining an angle of 0°
		1st sector runs from -45° to 45°
		2nd sector runs from 45° to 135°
		3rd sector runs from 135 to 225°
		4th sector runs from 225° to 315°
	2. In each sector, the robot tries to find a nearest neighbour x_1, x_2, x_3 and x_4. 
	2.1 If successful, the robot moves to the intersection of the lines through x_1x_3 and x_2x_4
	2.2 If not successful, the roboter assumes to be on the edge or on the corner of the lattice.
	    If on edge, robot moves to intersection of the line orthogonal through the third given point
	    If on a corner, he goes to the middlepoint of the line defined through the other two robots
	    If all his neighbours are in one sector, he stands still.
	TODO: implement some mechanism to keep some constant distance for meshsize
--]]

--lua-function for calculating distances
function dist(vector)
	local distance = math.sqrt(vector.x^2 + vector.y^2 + vector.z^2);
	return distance;
end

--comperator-function to sort robots by distance
function comp_robot_by_distance(robotx, roboty)
	if dist(get_position(robotx)) < dist(get_position(roboty)) then
		return true;
	else
		return false;
	end
end

--calculates scalarproduct of vectors a and b
function spr(a, b)
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
end

--calculates angle between vectors a and b
function get_angle(a, b)
	return math.acos(spr(a, b) / (dist(a) * dist(b)));
end

--checks whether two vectors are linear independant
function linear_independant(a, b)
	local lambda = a.x / b.x;
	if (a.y == lambda * b.y) and (a.z == lambda * b.z) then
		return false;
	else
		return true;
	end
end

--calculates the intersection of the lines through p1p2 and p3p4 in 2d
function intersectlines(p1, p2, p3, p4)
	local intersection = Vector3d(0.0,0.0,0.0);
	if linear_independant(p2 - p1, p4 - p3) then
		if (p2.x - p1.x) == 0 then
			intersection.x = p1.x;
			local m2 = (p4.y - p3.y) / (p4.x - p3.x);
			local n2 = p3.y - (m2 * p3.x);
			intersection.y = m2 * p1.x + n2;
		elseif (p3.x - p4.x) == 0 then
			intersection.x = p3.x;
			local m1 = (p2.y - p1.y) / (p2.x - p1.x);
			local n1 = p1.y - (m1 * p1.x);
			intersection.y = m1 * p3.x + n1;
		else			
			local m2 = (p4.y - p3.y) / (p4.x - p3.x);
			local n2 = p3.y - (m2 * p3.x);
			local m1 = (p2.y - p1.y) / (p2.x - p1.x);
			local n1 = p1.y - (m1 * p1.x);
			intersection.x = (n2 - n1) / (m1 - m2)
			intersection.y = m1 * intersection.x + n1;
		end
	else
		return nil
	end
	return intersection
end
 
--function returns 2dimensional normal-vector
function get_normal(vector)
		local norm = Vector3d(0.0,0.0,0.0);
		norm.x =  1 * vector.y / dist(vector);
		norm.y = -1 * vector.x / dist(vector);
		return norm;
end


function compute_nearby_robots(robots, view_radius) 
	local eps = 0.1;
	
	view_radius = view_radius * math.sqrt(2) + eps;
	
	local robot_pos = {};
	j = 1;
	for i =1, #robots do 
		local new_pos = View.get_position(robots[i]);
		if(dist(new_pos) <= view_radius) then
			robot_pos[j] = new_pos;
			j = j + 1;
		end
	end	
	robot_pos = Geometry.sort_vectors_by_length(robot_pos);
	
	--setting initial nearest neighbours in each sector
	--  1  5  2
	--  8  o  6
	--  4  7  3
	
	if(nearest_dir == nil) then
		nearest_dir = robot_pos[1];
	end
	
	local pos1 = nil;
	local pos2 = nil;
	local pos3 = nil;
	local pos4 = nil;
	local pos5 = nil;
	local pos6 = nil;
	local pos7 = nil;
	local pos8 = nil; 
	
	for i = 1, #robot_pos do 
		if(normalize(nearest_dir) == normalize(robot_pos[i])) then
			pos5 = robot_pos[i];
			break;
		end
	end
	
	if(pos5 == nil) then
		-- pos5 must not be nil
		return nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil;
	end
	
	if(dist(robot_pos[1]) < eps) then
		return nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil;
	end

	--iterate over all robots in order to find nearest neighbours in each sector
	for i = 1, #robot_pos do 
		local angle = get_angle(pos5, robot_pos[i]);
		
		--calculating normal to check whether angle is a reflex angle (>180°)
		local normx1 = get_normal(pos5);
		
		--forsign of distance given by HNF decides whether robot[i] is left or right from the line through x1 and therefore rotation direction
		if spr( normx1, robot_pos[i]) < 0 then
			angle = 2*math.pi - angle;
		end		
		
		if (angle > eps) and (angle < (1/2 * math.pi) - eps) and (pos2 == nil) then
			pos2 = robot_pos[i];
		elseif (angle > (1/2 * math.pi) + eps) and (angle < math.pi - eps) and (pos3 == nil) then
			pos3 = robot_pos[i];
		elseif (angle > math.pi + eps) and (angle < (3/2 * math.pi) - eps) and (pos4 == nil) then
			pos4 = robot_pos[i];
		elseif (angle > (3/2 * math.pi) + eps) and (angle < (2 * math.pi)- eps) and (pos1 == nil) then
			pos1 = robot_pos[i];
		elseif (angle >= (1/2 * math.pi) - eps) and (angle <= (1/2 * math.pi) + eps) and (pos6 == nil) then
			pos6 = robot_pos[i];
		elseif (angle >= math.pi - eps) and (angle <= math.pi + eps) and (pos7 == nil) then
			pos7 = robot_pos[i];
		elseif (angle >= (3/2 * math.pi) - eps) and (angle <= (3/2 * math.pi) + eps) and (pos8 == nil) then
			pos8 = robot_pos[i];
		end

		if(pos1 ~= nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil and pos5 ~= nil and pos6 ~= nil and pos7 ~= nil and pos8 ~= nil) then
			break;
		end
	end
	
	if(#robot_pos == 0) then
		return nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil;
	end
	
	if(own_id == interesting_id) then
		print("Pos1: ");
		print(pos1);
		print("Pos2: ");
		print(pos2);
		print("Pos3: ");
		print(pos3);
		print("Pos4: ");
		print(pos4);	
		print("Pos5: ");
		print(pos5);
		print("Pos6: ");
		print(pos6);
		print("Pos7: ");
		print(pos7);
		print("Pos8: ");
		print(pos8);	
		print();
	end
	
	local right = get_normal(pos5);
	local up = pos5;
	local left = (-1) * right;
	local down = (-1) * up;
	
	local result1, result2, result3, result4, result5, result6, result7, result8;
	
	result1 = validate_pos(pos1, pos2, right);
	result1 = validate_pos(result1, pos5, right);
	result1 = validate_pos(result1, pos4, up);
	result1 = validate_pos(result1, pos8, up);
	
	result2 = validate_pos(pos2, pos1, right);
	result2 = validate_pos(result2, pos5, right);
	result2 = validate_pos(result2, pos3, up);
	result2 = validate_pos(result2, pos6, up);
	
	result3 = validate_pos(pos3, pos4, right);
	result3 = validate_pos(result3, pos7, right);
	result3 = validate_pos(result3, pos2, up);
	result3 = validate_pos(result3, pos6, up);
	
	result4 = validate_pos(pos4, pos3, right);
	result4 = validate_pos(result4, pos7, right);
	result4 = validate_pos(result4, pos1, up);
	result4 = validate_pos(result4, pos8, up);
	
	result5 = validate_pos(pos5, pos1, right);
	result5 = validate_pos(result5, pos2, right);
	
	result6 = validate_pos(pos6, pos2, up);
	result6 = validate_pos(result6, pos3, up);
	
	result7 = validate_pos(pos7, pos3, right);
	result7 = validate_pos(result7, pos4, right);	
	
	result8 = validate_pos(pos8, pos4, up);	
	result8 = validate_pos(result8, pos1, up);		
	
	local gappos1, gappos2, gappos3, gappos4, gappos5, gappos6, gappos7, gappos8;
	
	gappos1 = get_initial_gap_pos(pos1, pos8, pos4, pos5, pos2, up, right);
	gappos1 = get_gap_pos(gappos1, pos2, right);
	gappos1 = get_gap_pos(gappos1, pos5, right);
	gappos1 = get_gap_pos(gappos1, pos4, up);
	gappos1 = get_gap_pos(gappos1, pos8, up);
	
	gappos2 = get_initial_gap_pos(pos2, pos6, pos3, pos5, pos1, up, right);
	gappos2 = get_gap_pos(gappos2, pos1, right);
	gappos2 = get_gap_pos(gappos2, pos5, right);
	gappos2 = get_gap_pos(gappos2, pos3, down);
	gappos2 = get_gap_pos(gappos2, pos6, down);
	
	gappos3 = get_initial_gap_pos(pos3, pos6, pos2, pos7, pos4, up, right);
	gappos3 = get_gap_pos(gappos3, pos4, left);
	gappos3 = get_gap_pos(gappos3, pos7, left);
	gappos3 = get_gap_pos(gappos3, pos2, down);
	gappos3 = get_gap_pos(gappos3, pos6, down);
	
	gappos4 = get_initial_gap_pos(pos4, pos8, pos1, pos7, pos3, up, right);
	gappos4 = get_gap_pos(gappos4, pos3, left);
	gappos4 = get_gap_pos(gappos4, pos7, left);
	gappos4 = get_gap_pos(gappos4, pos1, up);
	gappos4 = get_gap_pos(gappos4, pos8, up);
	
	--TODO: intersect with 0,0,0 to get initial pos
	gappos5 = pos5;
	gappos5 = get_gap_pos(gappos5, pos1, right);
	gappos5 = get_gap_pos(gappos5, pos2, right);
	gappos5 = get_gap_pos(gappos5, pos7, up);
	gappos5 = get_gap_pos(gappos5, pos7, down);
	
	gappos6 = pos6;
	gappos6 = get_gap_pos(gappos6, pos2, down);
	gappos6 = get_gap_pos(gappos6, pos3, down);
	gappos6 = get_gap_pos(gappos6, pos8, left);
	gappos6 = get_gap_pos(gappos6, pos8, right);
	
	gappos7 = pos7;
	gappos7 = get_gap_pos(gappos7, pos3, left);
	gappos7 = get_gap_pos(gappos7, pos4, left);	
	gappos7 = get_gap_pos(gappos7, pos5, up);
	gappos7 = get_gap_pos(gappos7, pos5, down);
	
	gappos8 = pos8;
	gappos8 = get_gap_pos(gappos8, pos4, up);
	gappos8 = get_gap_pos(gappos8, pos1, up);
	gappos8 = get_gap_pos(gappos8, pos6, left);
	gappos8 = get_gap_pos(gappos8, pos6, right);

	if(result1 == nil and (result8 == nil and result4 == nil or result5 == nil and result2 == nil)) then
		gappos1 = nil;
	end
	if(result2 == nil and (result5 == nil and result1 == nil or result6 == nil and result3 == nil)) then
		gappos2 = nil;
	end
	if(result3 == nil and (result4 == nil and result7 == nil or result6 == nil and result2 == nil)) then
		gappos3 = nil;
	end
	if(result4 == nil and (result8 == nil and result1 == nil or result7 == nil and result3 == nil)) then
		gappos4 = nil;
	end
	if(result5 == nil and (result1 == nil and result2 == nil or result7 == nil)) then
		gappos5 = nil;
	end	
	if(result6 == nil and (result2 == nil and result3 == nil or result8 == nil)) then
		gappos6 = nil;
	end	
	if(result7 == nil and (result4 == nil and result3 == nil or result5 == nil)) then
		gappos7 = nil;
	end	
	if(result8 == nil and (result1 == nil and result4 == nil or result6 == nil)) then
		gappos8 = nil;
	end	

	return result1,result2,result3,result4,result5,result6,result7,result8,gappos1,gappos2,gappos3,gappos4,gappos5,gappos6,gappos7,gappos8;
end

function get_initial_gap_pos(pos, ref1, ref2, ref3, ref4, up, right)
	if(pos ~= nil) then
		return pos;
	else		
		local line1, line2;
		if(ref1 ~= nil) then
			line1 = ref1;
		else
			line1 = ref2;
		end
		if(ref3 ~= nil) then
			line2 = ref3;
		else
			line2 = ref4;
		end		
		if(line1 == nil or line2 == nil) then
			return nil;
		else					
			return intersectlines(line1, line1 + up, line2, line2 + right);
		end		
	end
end

function get_gap_pos(pos, ref, refdir) 
	if(pos == nil) then
		return nil;
	end
	if(on_line(pos, ref, refdir)) then
		return pos;
	else
		if(is_nearer(pos, ref, refdir) == nil) then
			local zero = Vector3d(0,0,0);
			local dist1 = get_distance_to_line(zero, pos, refdir);
			local dist2 = get_distance_to_line(zero, ref, refdir);
			local dist = dist1 - dist2; -- dist1 > dist2		
			return pos + dist * get_normal(refdir);					
		else 
			return pos;
		end
	end	
end

function validate_pos(pos, ref, refdir) 
	if(pos == nil) then
		return nil;
	end
	if(on_line(pos, ref, refdir)) then
		return pos;
	else
		return is_nearer(pos, ref, refdir);
	end	
end

function is_nearer(pos, refpos, line) 
	local zero = Vector3d(0,0,0);
	local dist1 = get_distance_to_line(zero, pos, line);
	local dist2 = get_distance_to_line(zero, refpos, line);	
	if(dist1 <= dist2+0.01) then
		return pos;
	else
		return nil;
	end
end

function get_distance_to_line(pos, linepos, linedir) 
	local normal = get_normal(linedir);
	local intersect = intersectlines(pos, pos + normal, linepos, linepos + linedir);
	return dist(intersect - pos);
end

function on_line(pos1, pos2, line) 
	if(pos2 == nil or line == nil) then
		return true;
	end
	if(line.x == 0) then
		return math.abs(pos1.x - pos2.x) < 0.01;
	end
	local alpha = (pos2.x - pos1.x) / line.x;
	local spos2 = pos1.y + alpha * line.y;
	return math.abs(pos2.y - spos2) < 0.01;
end

function normalize(vec) 
	return vec/dist(vec);
end

own_id = 0;
interesting_id = 35;
last_dir1 = nil;
last_dir2 = nil;
nearest_dir = nil;



function try_gap(new_position, pos, gap, gapcount) 
	if(new_position ~= nil) then
		return new_position;
	end
	if(pos == nil) then
		if(gap == nil) then
			return nil;
		end
		return check_dir(gap, gapcount);
	end
	return nil;
end

function check_dir(gap, gapcount) 
	if(last_dir1 == nil) then
		remember(gapcount);
		return gap;
	elseif(last_dir1 == gapcount) then
		return gap;
	elseif(last_dir2 == nil) then
		if(gapcount == 1 and (last_dir1 == 5 or last_dir1 == 8)) or
		  (gapcount == 2 and (last_dir1 == 5 or last_dir1 == 6)) or
		  (gapcount == 3 and (last_dir1 == 7 or last_dir1 == 6)) or
		  (gapcount == 4 and (last_dir1 == 8 or last_dir1 == 7)) or
		  (gapcount > 4) then
			remember(gapcount);
			return gap;
		end
	elseif(gapcount == 1 and last_dir1 == 5 and last_dir2 == 8) or
		  (gapcount == 2 and last_dir1 == 5 and last_dir2 == 6) or
		  (gapcount == 3 and last_dir1 == 6 and last_dir2 == 7) or
		  (gapcount == 4 and last_dir1 == 7 and last_dir2 == 8) or
		  (lastdir1 == gapcount or lastdir2 == gapcount) then
		  return gap;
	else
		return nil;
	end
end

function remember(gapcount) 
	--  1  5  2
	--  8  o  6
	--  4  7  3
	if(gapcount == 1) then
		last_dir1 = 5;
		last_dir2 = 8;
	elseif(gapcount == 2) then
		last_dir1 = 5;
		last_dir2 = 6;
	elseif(gapcount == 3) then
		last_dir1 = 6;
		last_dir2 = 7;
	elseif(gapcount == 4) then
		last_dir1 = 7;
		last_dir2 = 8;
	else
		if(lastdir1 == nil) then
			last_dir1 = gapcount;
		elseif(lastdir2 == nil) then
			if(last_dir1 > gapcount) then
				last_dir2 = last_dir1;
				last_dir1 = gapcount;
			else
				last_dir2 = gapcount;
			end
		else
			print("Something strage");
			print(last_dir1);
			print(last_dir2);
			print(gapcount);
		end
	end
end		  
		

-- function check_dir(gap) {
	-- if(lastdir1 == nil) then
		-- lastdir1 = normalize(gap);
		-- return gap;
	-- else
		-- local angle1 = get_angle(lastdir1, gap);
		-- if(Math.abs(angle1) < 0.001) then
			-- return gap;
		-- else
			-- if(lastdir2 == nil) then
				-- lastdir2 = normalize(gap);
				-- return gap;
			-- else
				-- local angle2 = get_angle(lastdir2, angle);
				-- if(angle1 <= Math.pi/2 and angle2 <= Math.pi/2) then
					-- return gap;
				-- end
			-- end
		-- end
	-- end	
-- }

function main()
	own_id = View.get_id(View.get_own_identifier());

	--setting nullvector
	local zero = Vector3d(0.0,0.0,0.0);
	local robots = View.get_visible_robots();
	local pos1,pos2,pos3,pos4,pos5,pos6,pos7,pos8,gappos1,gappos2,gappos3,gappos4,gappos5,gappos6,gappos7,gappos8 = compute_nearby_robots(robots, 1000.0);
	
	local new_position = nil;
	
	new_position = try_gap(new_position, pos5, gappos5, 5);
	new_position = try_gap(new_position, pos6, gappos6, 6);
	new_position = try_gap(new_position, pos7, gappos7, 7);	
	new_position = try_gap(new_position, pos8, gappos8, 8);	
	new_position = try_gap(new_position, pos1, gappos1, 1);
	new_position = try_gap(new_position, pos2, gappos2, 2);
	new_position = try_gap(new_position, pos4, gappos4, 3);
	new_position = try_gap(new_position, pos3, gappos3, 4);		

	
	if(new_position == nil) then
		new_position = zero;
	end
	
	if(own_id == interesting_id) then
		print("----------------");
		print("Time:");
		print(View.get_time());
		print("Own_id: " .. own_id);
		print("Pos1: ");
		print(pos1);
		print("Pos2: ");
		print(pos2);
		print("Pos3: ");
		print(pos3);
		print("Pos4: ");
		print(pos4);	
		print("Pos5: ");
		print(pos5);
		print("Pos6: ");
		print(pos6);
		print("Pos7: ");
		print(pos7);
		print("Pos8: ");
		print(pos8);
		print("gapPos1: ");
		print(gappos1);
		print("gapPos2: ");
		print(gappos2);
		print("gapPos3: ");
		print(gappos3);
		print("gapPos4: ");
		print(gappos4);	
		print("gapPos5: ");
		print(gappos5);
		print("gapPos6: ");
		print(gappos6);
		print("gapPos7: ");
		print(gappos7);
		print("gapPos8: ");
		print(gappos8);	
		print("Last_dir1");
		print(last_dir1);
		print("Last_dir2");
		print(last_dir2);
		print();
	end
	
	View.add_position_request(new_position);
	
	
end
 

