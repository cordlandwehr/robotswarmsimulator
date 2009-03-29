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


function compute_nearby_robots(robots, vector, view_radius) 
	local eps = 0.1;
	
	view_radius = view_radius * math.sqrt(2) + eps;
	
	local robot_pos = {};
	j = 1;
	for i =1, #robots do 
		local new_pos = View.get_position(robots[i]) - vector; --pos relative to given vector
		if(dist(new_pos) > eps and dist(new_pos) <= view_radius) then
			robot_pos[j] = new_pos;
			j = j + 1;
		end
	end
	if(vector.x ~= 0 or vector.y ~= 0 or vector.z ~= 0) then
		local new_pos = (-1) * vector; --pos relative to given vector
		if(dist(new_pos) > eps and dist(new_pos) <= view_radius) then
			robot_pos[j] = new_pos;
			j = j + 1;
		end
	end
	robot_pos = Geometry.sort_robots_by_distance(robot_pos);
	
	--setting initial nearest neighbours in each sector
	--  1  5  2
	--  8  o  6
	--  4  7  3
	
	local pos1 = nil;
	local pos2 = nil;
	local pos3 = nil;
	local pos4 = nil;
	local pos5 = robot_pos[1];
	local pos6 = nil;
	local pos7 = nil;
	local pos8 = nil;

	--iterate over all robots in order to find nearest neighbours in each sector
	for i = 1, #robot_pos do 
		local angle = get_angle(robot_pos[1], robot_pos[i]);
		
		--calculating normal to check whether angle is a reflex angle (>180°)
		local normx1 = get_normal(robot_pos[1]);
		
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
		return nil,nil,nil,nil,nil,nil,nil,nil;
	end
	
	local right = get_normal(robot_pos[1]);
	local up = robot_pos[1];
	
	pos1 = validate_pos(pos1, pos2, right);
	pos1 = validate_pos(pos1, pos5, right);
	pos1 = validate_pos(pos1, pos4, up);
	pos1 = validate_pos(pos1, pos8, up);
	
	pos2 = validate_pos(pos2, pos1, right);
	pos2 = validate_pos(pos2, pos5, right);
	pos2 = validate_pos(pos2, pos3, up);
	pos2 = validate_pos(pos2, pos6, up);
	
	pos3 = validate_pos(pos3, pos4, right);
	pos3 = validate_pos(pos3, pos7, right);
	pos3 = validate_pos(pos3, pos2, up);
	pos3 = validate_pos(pos3, pos6, up);
	
	pos4 = validate_pos(pos4, pos3, right);
	pos4 = validate_pos(pos4, pos7, right);
	pos4 = validate_pos(pos4, pos1, up);
	pos4 = validate_pos(pos4, pos8, up);
	
	pos5 = validate_pos(pos5, pos1, right);
	pos5 = validate_pos(pos5, pos2, right);
	
	pos6 = validate_pos(pos6, pos2, up);
	pos6 = validate_pos(pos6, pos3, up);
	
	pos7 = validate_pos(pos7, pos3, right);
	pos7 = validate_pos(pos7, pos4, right);	
	
	pos8 = validate_pos(pos8, pos4, right);
	pos8 = validate_pos(pos8, pos1, right);	
	
	if(pos1 ~= nil) then
		pos1 = pos1 + vector;
	end
	if(pos2 ~= nil) then
		pos2 = pos2 + vector;
	end
	if(pos3 ~= nil) then
		pos3 = pos3 + vector;
	end
	if(pos4 ~= nil) then
		pos4 = pos4 + vector;
	end	
	if(pos5 ~= nil) then
		pos5 = pos5 + vector;
	end	
	if(pos6 ~= nil) then
		pos6 = pos6 + vector;
	end	
	if(pos7 ~= nil) then
		pos7 = pos7 + vector;
	end	
	if(pos8 ~= nil) then
		pos8 = pos8 + vector;
	end	

	return pos1,pos2,pos3,pos4,pos5,pos6,pos7,pos8;
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

function compute_new_position(robots, gap_pos) 
	local pos1,pos2,pos3,pos4,nearest = compute_nearby_robots(robots, gap_pos, 5.0);
	if(own_id == 51) then
		print("INNNER");
		print("Own_id: " .. own_id);
		print("Pos1: ");
		print(pos1);
		print("Pos2: ");
		print(pos2);
		print("Pos3: ");
		print(pos3);
		print("Pos4: ");
		print(pos4);
		print("Nearest: ");
		print(nearest);		
		print("Normal: ");
		print(normal);		
	end
	local count = 0;
	local jump_count = 0;
	if(pos1 ~= nil) then
		count = count + 1;
		if(may_jump_to_nearby_gap(robots, pos1)) then
			if(own_id == 51) then
				print("pos1 may jump");
			end
			jump_count = jump_count + 1;
		end
	end
	if(pos2 ~= nil) then
		count = count + 1;
		if(may_jump_to_nearby_gap(robots, pos2)) then
			jump_count = jump_count + 1;
		end
	end
	if(pos3 ~= nil) then
		count = count + 1;		
		if(may_jump_to_nearby_gap(robots, pos3)) then
			jump_count = jump_count + 1;
		end
	end
	if(pos4 ~= nil) then
		count = count + 1;
		if(may_jump_to_nearby_gap(robots, pos4)) then
			jump_count = jump_count + 1;
		end
	end
	
	if(own_id == 51) then
		print("count:");
		print(count);
		print("jump_count");
		print(jump_count);
	end
	
	if(jump_count > 1) then
		-- other robots may also want to jump to this gap, so dont move
		-- Note: this creates "deadlocks" and leaves small gaps.
		-- TODO: jump to gap with probability of 1/jump_count + add treatment for multi robots at same pos
		return Vector3d(0,0,0); 
	end
	
	-- jump to gap! :-)
	return gap_pos;		
end

function may_jump_to_nearby_gap(robots, robot_pos) 
	local pos1,pos2,pos3,pos4,pos5,pos6,pos7,pos8 = compute_nearby_robots(robots, robot_pos, 5.0);
	if(own_id == 51) then
		print("INNNERINNER");
		print("RobotPos");
		print(robot_pos);
		print("Pos1: ");
		print(pos1);
		print("Pos2: ");
		print(pos2);
		print("Pos3: ");
		print(pos3);
		print("Pos4: ");
		print(pos4);
		print("Nearest: ");
		print(nearest);		
		print("Normal: ");
		print(normal);		
	end
	if(pos1 == nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos2 == nil and pos1 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos3 == nil and pos1 ~= nil and pos2 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos4 == nil and pos1 ~= nil and pos2 ~= nil and pos3 ~= nil)  then
		return true;
	elseif (pos1 == nil and pos2 == nil and pos3 == nil and pos4 == nil) then
		if(pos6 ~= nil and pos7 == nil and pos8 == nil) then
			return true;
		elseif(pos8 ~= nil and pos6 == nil and pos7 == nil) then
			return true;
		end				
	end
	
	return false;
end

own_id = 0;

function main()
	--setting nullvector
	local zero = Vector3d(0.0,0.0,0.0);
	local robots = View.get_visible_robots();
	local pos1,pos2,pos3,pos4,pos5,pos6,pos7,pos8 = compute_nearby_robots(robots, zero, 5.0);
	local nearest = pos5;
	
	if(nearest == nil) then
		View.add_position_request(zero);
		return;
	end
	
	local normal = get_normal(nearest);
	
	local new_position = zero;
	own_id = View.get_id(View.get_own_identifier());
	
	if(own_id == 51) then
		print("------------------");
		print("---- time: " .. View.get_time());
		print("------------------");
		print("Own_id: " .. own_id);
		local p1,p2,p3,p4,p5,p6,p7,p8 = compute_nearby_robots(robots, zero, 5.0);
		print("Pos1: ");
		print(p1);
		print("Pos2: ");
		print(p2);
		print("Pos3: ");
		print(p3);
		print("Pos4: ");
		print(p4);
		print("Pos5: ");
		print(p5);
		print("Pos6: ");
		print(p6);
		print("Pos7: ");
		print(p7);
		print("Pos8: ");
		print(p8);
		print("Normal: ");
		print(normal);		
	end
	
		
	
	-- only possibly change pos if exactly one gap is found
	if(pos1 == nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil) then
		pos1 = intersectlines(pos2, pos2 + normal, pos4, pos4 + nearest);
		new_position = compute_new_position(robots, pos1);
	elseif (pos2 == nil and pos1 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		pos2 = intersectlines(pos1, pos1 + normal, pos3, pos3 + nearest);		
		new_position = compute_new_position(robots, pos2);
	elseif (pos3 == nil and pos1 ~= nil and pos2 ~= nil and pos4 ~= nil)  then
		pos3 = intersectlines(pos4, pos4 + normal, pos2, pos2 + nearest);
		if(own_id == 51) then
			print("gap:");
			print(pos3);
		end
		new_position = compute_new_position(robots, pos3);
	elseif (pos4 == nil and pos1 ~= nil and pos2 ~= nil and pos3 ~= nil)  then
		pos4 = intersectlines(pos3, pos3 + normal, pos1, pos1 + nearest);
		new_position = compute_new_position(robots, pos4);
	end
	
	if(pos1 == nil and pos2 == nil and pos3 == nil and pos4 == nil) then
		--TODO: avoid jumping to same pos
		if(pos6 ~= nil and pos7 == nil and pos8 == nil) then
			new_position = pos5 + pos6;
		elseif(pos8 ~= nil and pos6 == nil and pos7 == nil) then
			new_position = pos5 + pos8;
		end		
	end
	
	if(own_id == 51) then
		print("New position");
		print(new_position);
		print();
	end
	View.add_position_request(new_position);
	
	
end
 

