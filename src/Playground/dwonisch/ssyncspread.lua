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

function on_line(pos, linepos, line) 
	if(linepos == nil or line == nil) then
		error("on_line: linepos/line may not be nil.");
	end
	if(line.x == 0) then
		return math.abs(pos.x - linepos.x) < eps;
	end
	local alpha = (linepos.x - pos.x) / line.x;
	local spos2 = pos.y + alpha * line.y;
	return math.abs(linepos.y - spos2) < eps;
end

-- function compute_new_pos(left, right) 
	-- local kDesiredDist = 5; --maybe the same as view radius or a bit less
	-- local kMaxStepDist = 2; --avoid moving into each other

	-- local left_pos = left;
	-- local right_pos = right;	

	-- if(dist(left_pos) < kDesiredDist and dist(right_pos) < kDesiredDist) then
		-- --do nothing
		-- return Vector3d(0,0,0);
	-- end

	-- if(dist(left_pos) < kDesiredDist) then
		-- --move to right
		-- stepDist = (kDesiredDist - dist(left_pos));
		-- return math.min(stepDist, kMaxStepDist) * normalize(right_pos);
	-- end

	-- if(dist(right_pos) < kDesiredDist) then
		-- --move to left
		-- stepDist = kDesiredDist - dist(right_pos);
		-- return math.min(stepDist, kMaxStepDist) * normalize(left_pos);
	-- end

	-- --else
	-- -- do nothing
	-- return Vector3d(0,0,0);
-- end

function compute_new_pos(left, right) 
	return (left + right)/2;
end

function normalize(vec) 
	return vec/dist(vec);
end

-- Computes neighbors pos1,..,pos4 using sector strategy
function compute_neighbours(robots, pos)
	local robot_pos = {};
	local j = 1;
	for i =1, #robots do 
		local new_pos = View.get_position(robots[i]) - pos; --pos relative to given vector
		if(dist(new_pos) > eps) then
			robot_pos[j] = new_pos;
			j = j + 1;
		end
	end
	if(pos ~= zero) then
		local new_pos = (-1) * pos; --pos relative to given vector
		robot_pos[j] = new_pos;
		j = j + 1;		
	end
	robot_pos = Geometry.sort_vectors_by_length(robot_pos);
	
	local pos1 = robot_pos[1];
	local pos2 = nil;
	local pos3 = nil;
	local pos4 = nil;
	
	--iterate over all robots in order to find nearest neighbours in each sector
	for i = 2, #robots do 
		local angle = get_angle(pos1, robot_pos[i]);
		
		--calculating normal to check whether angle is a reflex angle (>180°)
		local normx1 = get_normal(pos1);
		
		--forsign of distance given by HNF decides whether robot[i] is left or right from the line through x1 and therefore rotation direction
		if spr( normx1, robot_pos[i]) < 0 then
			angle = 2*math.pi - angle;
		end
		
		if (angle > (1/4 * math.pi)) and (angle <= (3/4 * math.pi)) and (pos2 == nil) then
			pos2 = robot_pos[i];
		elseif (angle > (3/4 * math.pi)) and (angle <= (5/4 * math.pi)) and (pos3 == nil) then
			pos3 = robot_pos[i];
		elseif (angle > (5/4 * math.pi)) and (angle <= (7/4 * math.pi)) and (pos4 == nil) then
			pos4 = robot_pos[i];
		else
			-- robot_pos[i] is in 1st sector
		end

		if(pos1 ~= nil and pos2 ~= nil and pos3 ~= nil and pos4 ~= nil) then
			break;
		end
	end
	
	if(pos1 ~= nil) then
		pos1 = pos1 + pos;
	end
	if(pos2 ~= nil) then
		pos2 = pos2 + pos;
	end
	if(pos3 ~= nil) then
		pos3 = pos3 + pos;
	end
	if(pos4 ~= nil) then
		pos4 = pos4 + pos;
	end
	
	return pos1,pos2,pos3,pos4;
end

-- Reorders pos1,..,pos4 such that pos1=pos
function reorder(pos1, pos2, pos3, pos4, pos)
	if(dist(pos1-pos) < eps) then
		return pos1,pos2,pos3,pos4;
	elseif(dist(pos2-pos) < eps) then
		return pos2,pos3,pos4,pos1;
	elseif(dist(pos3-pos) < eps) then
		return pos3,pos4,pos1,pos2;
	elseif(dist(pos4-pos) < eps) then
		return pos4,pos1,pos2,pos3;
	else	
		print("Reorder: Illegal argument exception");
		print(own_id);		
		print(pos1);
		print(pos2);
		print(pos3);
		print(pos4);
		print(pos);
		error("Reorder: Illegal argument exception");
	end
end

-- rotates vector counterclockwise (2d)
function rotate(vector, angle) 
	local result = Vector3d(0,0,0);
	result.x = math.cos(angle)*vector.x - math.sin(angle)*vector.y;
	result.y = math.sin(angle)*vector.x + math.cos(angle)*vector.y;
	return result;
end

-- Bugs: Beim Bewegen muss auch darauf geachtet werden, dass man nicht die sektoren von obenrechts, untenlinks, untenrechts, obenlinks verändert (reoder fehler)
-- 		 Es muss auch darauf geachtet werden, dass down/up beim Bewegen keine Linie bilden (mehr forbidden pos)
--       Edge cases: Inbesondere up3 == nil führt zu zero, obwohl unerheblich für movement
--       Operator== sollte nicht auf Gleichheit prüfen, da double Werte, sondern auf eps Abstand.

-- Computes new_pos using compute_new_pos and trims the pos such that all invariants are satisfied.
-- May return zero vector
--     up
--left  0  right
function invariant_compute_new_pos(robots, left, up, right)
	if(own_id == interesting_id) then
		print("left");
		print(left);
		print("up");
		print(up);
		print("right");
		print(right);		
	end
	if(left == nil or right == nil) then
		--TODO: edge cases need to be considers, skipping for now
		return zero;
	end
	
	if(not on_line(right, zero, left)) then
		if(own_id == interesting_id) then
			print("left right not on line");
		end
		--left/right not on line -> robot must not move left/right, since otherwise top/bottom wont be on_line anymore too
		return zero;
	end
	local new_pos = compute_new_pos(left,right);
	if(new_pos == zero) then
		--all fine
		return zero;
	end
	local updir = get_normal(left); --because of invariant both left and up are lattice-lines.
	
	if(up == nil) then
		return new_pos; 
	end
	
	local up1,up2,up3,up4 = compute_neighbours(robots, up);
	
	if(up1 == nil or up2 == nil or up3 == nil or up4 == nil) then
		--TODO: edge cases need to be considers, skipping for now
		return zero;
	end
	
	up1,up2,up3,up4 = reorder(up1,up2,up3,up4,zero);
	
	if(own_id == interesting_id) then
		print("up1");
		print(up1);
		print("up2");
		print(up2);
		print("up3");
		print(up3);
		print("up4");
		print(up4);
		
	end	
	--      up3
    --       |	
	-- up2-- up -- up4
	--       |
	--      0/up1
	
	if(not on_line(up4, up, up2-up)) then
		--left/right of up not on line -> robot must not move left/right, since otherwise top/bottom of up wont be one_line anymore too
		return zero;
	end
	
	-- -> we may move without violating our own or ups invariant
	-- but dont move too far and avoid creating (wrong) lines
	
	-- Dont move too far:
	-- avoid moving out of view-field of up-robot
	local max_left_movement = intersectlines(zero, left, up, up+rotate(updir, -math.pi/4)); 
	local max_right_movement = intersectlines(zero, left, up, up+rotate(updir, math.pi/4));
	if(dist(max_left_movement) < eps) then
		max_left_movement = Vector3d(0,0,0);
	end
	if(dist(max_right_movement) < eps) then
		max_right_movement = Vector3d(0,0,0);
	end
	
	if(own_id == interesting_id) then
		print("updir");
		print(updir);
		print("max_left_movement");
		print(max_left_movement);
		print("max_right_movement");
		print(max_right_movement);
		print("new_pos");
		print(new_pos);		
	end
	
	if(normalize(new_pos) == normalize(max_left_movement)) then
		if(dist(new_pos) > dist(max_left_movement)) then
			new_pos = dist(max_left_movement) * normalize(max_left_movement);
		end
	elseif(normalize(new_pos) == normalize(max_right_movement)) then
		if(dist(new_pos) > dist(max_right_movement)) then
			new_pos = dist(max_right_movement) * normalize(max_right_movement);
		end
	elseif(max_left_movement == zero or max_right_movement == zero) then
		return zero;
	else
		error("invariant_compute_new_pos: Something unexpected for id " .. own_id);
	end
	
	if(own_id == interesting_id) then
		print("Trimmed new_pos to: ");
		print(new_pos);
	end
	
	-- new_pos should be nearer to up than left and right (else we get problems with sector computations)
	if(dist(up - new_pos) >= dist(up - right) or dist(up - new_pos) >= dist(up - left)) then
		--TODO: trim new_pos such that dists are fixed afterwards.
		if(own_id == interesting_id) then			
			print("Fallback to zero to ensure sector invariant. (Can be improved.)");
		end
		return zero;
	end
	
	--void creating (wrong) lines
	local forbidden_pos = nil;
	if(normalize(up3-up) ~= normalize(updir)) then
		-- up3-up is not lattice-line, so (up3,up,zero) may not be on a line.
		forbidden_pos = intersectlines(up, up3, zero, left);
	end
	
	if(forbidden_pos ~= nil and new_pos == forbidden_pos) then
		--TODO: instead we also may simple change the pos a little bit to avoid the line
		if(own_id == interesting_id) then
			print("Fallback to zero to ensure correct lines invariant.");
		end
		return zero;
	end
	
	if(own_id == interesting_id) then
		print("Return new_pos");
		print(new_pos);
	end
	-- Hopefully we eventually got here, so we can suggest new_pos as new position..	
	return new_pos; 
end

eps = 0.0001;
zero = nil;
own_id = nil;
interesting_id = 81;

function main()
	own_id = View.get_id(View.get_own_identifier());
	zero = Vector3d(0.0,0.0,0.0);
	local robots = View.get_visible_robots();	
	local pos1,pos2,pos3,pos4 = compute_neighbours(robots, zero);
	
	local suggested_pos1 = invariant_compute_new_pos(robots, pos2, pos1, pos4);
	local suggested_pos2 = invariant_compute_new_pos(robots, pos4, pos3, pos2);
	local suggested_pos3 = invariant_compute_new_pos(robots, pos1, pos4, pos3);
	local suggested_pos4 = invariant_compute_new_pos(robots, pos3, pos2, pos1);
	
	if(own_id == interesting_id) then
		print("suggested_pos1");
		print(suggested_pos1);
		print("suggested_pos2");
		print(suggested_pos2);
		print("suggested_pos3");
		print(suggested_pos3);
		print("suggested_pos4");
		print(suggested_pos4);
	end
	
	local suggested_left_pos = nil;
	local suggested_right_pos = nil;
	
	if(dist(suggested_pos1) > dist(suggested_pos2)) then
		suggested_left_pos = suggested_pos2;
	else
		suggested_left_pos = suggested_pos1;
	end
	if(dist(suggested_pos3) > dist(suggested_pos4)) then
		suggested_right_pos = suggested_pos4;
	else
		suggested_right_pos = suggested_pos3;
	end
	
	if(own_id == interesting_id) then
		print("suggested_left_pos");
		print(suggested_left_pos);
		print("suggested_right_pos");
		print(suggested_right_pos);		
		print("-------------------");
		print();
	end
	
	if(suggested_left_pos ~= zero) then
		add_position_request(suggested_left_pos); 
	else
		add_position_request(suggested_right_pos); 
	end	
end
 

