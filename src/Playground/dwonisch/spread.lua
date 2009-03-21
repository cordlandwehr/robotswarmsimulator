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

function main()
	
	--setting nullvector
	zero = Vector3d(0.0,0.0,0.0);
	
	robots = get_visible_robots();

	--sorts robots by distance in place (what is really cool!)
	table.sort(robots, comp_robot_by_distance);
	
	--setting initial nearest neighbours in each sector
	x1 = robots[1];
	x2 = nil;
	x3 = nil;
	x4 = nil;

	--iterate over all robots in order to find nearest neighbours in each sector
	for i = 2, #robots do 
		angle = get_angle(get_position(x1), get_position(robots[i]));
		
		--calculating normal to check whether angle is a reflex angle (>180°)
		normx1 = get_normal(get_position(x1));
		
		--forsign of distance given by HNF decides whether robot[i] is left or right from the line through x1 and therefore rotation direction
		if spr( normx1, get_position(robots[i])) < 0 then
			angle = 2*math.pi - angle;
		end
		
		if (angle > (1/4 * math.pi)) and (angle <= (3/4 * math.pi)) and (x2 == nil) then
			x2 = robots[i];
		elseif (angle > (3/4 * math.pi)) and (angle <= (5/4 * math.pi)) and (x3 == nil) then
			x3 = robots[i];
		elseif (angle > (5/4 * math.pi)) and (angle <= (7/4 * math.pi)) and (x4 == nil) then
			x4 = robots[i];
		else
			-- robot[i] is in 1st sector
		end

		if(x1 ~= nil and x2 ~= nil and x3 ~= nil and x4 ~= nil) then
			break;
		end
	end
	
	newposition = zero;
	
		
	line1 = compute_new_pos(x1,x3);
	line2 = compute_new_pos(x2,x4);
	
	--io.write("Line1: " .. tostring(line1) .. " Line2: " .. tostring(line2) .. "\n");	
	
	newposition = line1 + line2;
	
	--io.write("New_Pos: " .. tostring(newposition) .. "\n\n");
	
	add_position_request(newposition); 
end

function compute_new_pos(left, right) 
	kDesiredDist = 5; --maybe the same as view radius or a bit less
	kMaxStepDist = 2; --avoid moving into each other

	--if(left == nil and right == nil) then
	--	return Vector3d(0,0,0);
	--end
	
	if(left ~= nil) then
		left_pos = get_position(left);
		if(right ~= nil) then
			right_pos = get_position(right);
		else
			right_pos = (-(2^10)) * get_position(left);
		end
	else
		right_pos = get_position(right);
		left_pos = (-(2^10)) * get_position(right);
	end

	if(dist(left_pos) < kDesiredDist and dist(right_pos) < kDesiredDist) then
		--do nothing
		return Vector3d(0,0,0);
	end

	if(dist(left_pos) < kDesiredDist) then
		--move to right
		stepDist = (kDesiredDist - dist(left_pos));
		return math.min(stepDist, kMaxStepDist) * normalize(right_pos);
	end

	if(dist(right_pos) < kDesiredDist) then
		--move to left
		stepDist = kDesiredDist - dist(right_pos);
		return math.min(stepDist, kMaxStepDist) * normalize(left_pos);
	end

	--else
	-- do nothing
	return Vector3d(0,0,0);
end

function normalize(vec) 
	return vec/dist(vec);
end
 

