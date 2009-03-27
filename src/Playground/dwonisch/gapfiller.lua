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
	distance = math.sqrt(vector.x^2 + vector.y^2 + vector.z^2);
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
	lambda = a.x / b.x;
	if (a.y == lambda * b.y) and (a.z == lambda * b.z) then
		return false;
	else
		return true;
	end
end

--calculates the intersection of the lines through p1p2 and p3p4 in 2d
function intersectlines(p1, p2, p3, p4)
	intersection = Vector3d(0.0,0.0,0.0);
	if linear_independant(p2 - p1, p4 - p3) then
		if (p2.x - p1.x) == 0 then
			intersection.x = p1.x;
			m2 = p4.y - p3.y / p4.x - p3.x;
			n2 = p3.y / m2 * p3.x
			intersection.y = m2 * p1.x + n2;
		elseif (p3.x - p4.x) == 0 then
			intersection.x = p3.x;
			m1 = p2.y - p1.y / p2.x - p1.x;
			n1 = p1.y / m1 * p1.x
			intersection.y = m1 * p3.x + n1;
		else
			m2 = p4.y - p3.y / p4.x - p3.x;
			n2 = p3.y / m2 * p3.x
			m1 = p2.y - p1.y / p2.x - p1.x;
			n1 = p1.y / m1 * p1.x
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
		norm = Vector3d(0.0,0.0,0.0);
		norm.x =  1 * vector.y / dist(vector);
		norm.y = -1 * vector.x / dist(vector);
		return norm;
end


function compute_nearby_robots(robots, vector) 
	robot_pos = {};
	for i =1, #robots do 
		robot_pos[i] = View.get_position(robot_pos[i]) - vector; --pos relative to given vector
	end
	robot_pos = Geometry.sort_robots_by_distance(robot_pos);
	
	--setting initial nearest neighbours in each sector
	pos1 = robot_pos[1];
	pos2 = nil;
	pos3 = nil;
	pos4 = nil;

	--iterate over all robots in order to find nearest neighbours in each sector
	for i = 2, #robots do 
		angle = get_angle(pos1, robot_pos[i]);
		
		--calculating normal to check whether angle is a reflex angle (>180°)
		normx1 = get_normal(pos1);
		
		--forsign of distance given by HNF decides whether robot[i] is left or right from the line through x1 and therefore rotation direction
		if spr( normx1, robot_pos[i])) < 0 then
			angle = 2*math.pi - angle;
		end
		
		if (angle > (1/4 * math.pi)) and (angle <= (3/4 * math.pi)) and (pos2 == nil) then
			pos2 = robot_pos[i] + vector;
		elseif (angle > (3/4 * math.pi)) and (angle <= (5/4 * math.pi)) and (pos3 == nil) then
			pos3 = robot_pos[i] + vector;
		elseif (angle > (5/4 * math.pi)) and (angle <= (7/4 * math.pi)) and (pos4 == nil) then
			pos4 = robot_pos[i] + vector;
		else
			-- robot[i] is in 1st sector
		end

		if(pos1 ~= nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil) then
			break;
		end
	end

	pos1 = pos1 + vector;
	
	return pos1,pos2,pos3,pos4;
end

function normalize(vec) 
	return vec/dist(vec);
end

function compute_new_position(robots, gap_pos) 
	pos1,pos2,pos3,pos4 = compute_nearby_robots(robots, gap_pos);
	count = 0;
	jump_count = 0;
	if(pos1 ~= nil) then
		count++;
		if(may_jump_to_gap(robots, pos1)) then
			jump_count++;
		end;
	end
	if(pos2 ~= nil) then
		count++;
		if(may_jump_to_gap(robots, pos2)) then
			jump_count++;
		end;
	end
	if(pos3 ~= nil) then
		count++;
		if(may_jump_to_gap(robots, pos3)) then
			jump_count++;
		end;
	end
	if(pos4 ~= nil) then
		count++;
		if(may_jump_to_gap(robots, pos4)) then
			jump_count++;
		end;
	end
	
	if(count == 1) then
		-- edge robot, dont move!
		return Vector3d(0,0,0);
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
	pos1,pos2,pos3,pos4 = compute_nearby_robots(robots, robot_pos);
	
	if(pos1 == nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos2 == nil and pos1 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos3 == nil and pos1 ~= nil and pos2 ~= nil and pos4 ~= nil)  then
		return true;
	elseif (pos4 == nil and pos1 ~= nil and pos2 ~= nil and pos3 ~= nil)  then
		return true;
	end
	
	return false;
end

function main()
	--setting nullvector
	zero = Vector3d(0.0,0.0,0.0);
	robots = get_visible_robots();
	pos1,pos2,pos3,pos4 = compute_nearby_robots(robots, zero);
	
	new_position = zero;
	
	-- only possibly change pos if exactly one gap is found
	if(pos1 == nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		new_position = compute_new_position(robots, pos1);
	elseif (pos2 == nil and pos1 ~= nil and pos3 ~= nil and pos4 ~= nil)  then
		new_position = compute_new_position(robots, pos2);
	elseif (pos3 == nil and pos1 ~= nil and pos2 ~= nil and pos4 ~= nil)  then
		new_position = compute_new_position(robots, pos3);
	elseif (pos4 == nil and pos1 ~= nil and pos2 ~= nil and pos3 ~= nil)  then
		new_position = compute_new_position(robots, pos4);
	end
	
	View.add_position_request(new_position);
	
	
end
 

