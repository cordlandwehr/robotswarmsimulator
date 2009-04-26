--[[
	LATTICE
	Script for building a lattice in 2 dimensions
	let meshsize be the distance the roboters shall have in the final lattice
	Algorithm works for each robot as follows:
	1. Robot identifies his nearest neighbour x_1
		if this roboter is too near (distance smaller than meshsize) it moves directly away from it
		if the roboter is more than delta * meshsize away, it halves the distance to it
		if the roboter is not too far and not too near, it performs the following algorithm:
		the line from x to the origin marks the anglebisector of the 1st sector,
		defining an angle of 0°
		1st sector runs from -45° to 45°
		2nd sector runs from 45° to 135°
		3rd sector runs from 135 to 225°
		4th sector runs from 225° to 315°
	2. In each sector, the robot tries to find a nearest neighbour x_1, x_2, x_3 and x_4. 
	2.1 If successful, the robot moves to the intersection of the normal through x_1x_3 and the line through x_2x_4
	2.2 If not successful, the roboter assumes to be on the edge or on the corner of the lattice.
	    If on edge, robot moves to intersection of the line orthogonal through the third given point
	    If on a corner, he goes to the middlepoint of the line defined through the other two robots
	    If all his neighbours are in one sector, he stands still.
--]]

--lua-function for calculating distances
function dist(vector)
	return math.sqrt(vector.x^2 + vector.y^2 + vector.z^2);
end

--comperator-function to sort robots by distance
function comp_robot_by_distance(robotx, roboty)
	return (dist(get_position(robotx)) < dist(get_position(roboty)));
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
	return ((a.y ~= lambda * b.y) or (a.z ~= lambda * b.z));
end

--function returns 2dimensional normal-vector
function get_normal(vector)
	return Vector3d((1 * vector.y / dist(vector)), (-1 * vector.x / dist(vector)), 0.0);
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

function main()

	--setting nullvector
	zero = Vector3d(0.0,0.0,0.0);
	
	--constants for our lattice
	meshsize = 1.5;
	delta = 5;
	
	robots = get_visible_robots();

	--sorts robots by distance in place (what is really cool!)
	table.sort(robots, comp_robot_by_distance);

	newposition = zero;
		
	if dist(get_position(robots[1])) == 0.0 then
		--robots shall not be on the same position
		--TODO: find a solution that makes more sense
		print "robots are on same position!";
		newposition = Vector3d(math.random(), math.random(), math.random());
		newposition = newposition / dist(newposition) * meshsize;
	elseif dist(get_position(robots[1])) < meshsize then
		--robots are too near, moving away from nearest neighbour
		c = meshsize / dist(get_position(robots[1]));
		newposition = (meshsize - c) * get_position(robots[1]);
	elseif dist(get_position(robots[1])) < delta * meshsize then
		-- nearest neighbour is not to near and not too far away. Apply algorithm.
		
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
				angle = 2 * math.pi - angle;
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
		end
		
		-- case: robot in the middle
		if (x2 ~= nil and x3 ~= nil and x4 ~= nil) then
			normal = get_normal(get_position(x1), get_position(x3));
			newposition = intersectlines(get_position(x1), get_position(x3), get_position(x2), get_position(x2) + normal);
		-- edgecases  (cross line constructed from opposite sectors with the orthogonal through the third given point)
		elseif (x2 ~= nil and x3 ~= nil and x4 == nil) then
			normal = get_normal(get_position(x1), get_position(x3));
			newposition = intersectlines(get_position(x1), get_position(x3), get_position(x2), get_position(x2) + normal);
		elseif (x2 ~= nil and x3 == nil and x4 ~= nil) then
			normal = get_normal(get_position(x2), get_position(x4));
			newposition = intersectlines(get_position(x2), get_position(x4), get_position(x1), get_position(x1) + normal);
		elseif (x2 == nil and x3 ~= nil and x4 ~= nil) then
			normal = get_normal(get_position(x1), get_position(x3));
			newposition = intersectlines(get_position(x1), get_position(x3), get_position(x4), get_position(x4) + normal);
		-- cornercases (at the moment: move to midpoint - this might be not a good idea?)	
		elseif (x2 ~= nil and x3 == nil and x4 == nil) then
			newposition = 0.5 * get_position(x1) + get_position(x2);
		elseif (x2 == nil and x3 ~= nil and x4 == nil) then
			newposition = 0.5 * get_position(x1) + get_position(x3);
		elseif (x2 == nil and x3 == nil and x4 ~= nil) then
			newposition = 0.5 * get_position(x1) + get_position(x4);
		-- bad cases
		else
			--stand still!
		end
		
		--check whether newposition would be too close to another robot
		for i = 1, #robots do
			if Geometry.compute_distance(newposition, get_position(robots[i])) < meshsize then
				newposition = zero;
			end
		end
	else
		--nearest neighbour too far away, move nearer.
		newposition = 0.5 * get_position(robots[1]);
	end
	
	add_position_request(newposition); 
end
 