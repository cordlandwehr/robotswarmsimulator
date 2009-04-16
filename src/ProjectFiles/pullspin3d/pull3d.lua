--[[
	PULL SPIN 3D
	Script for building a compact sphere out of cloud of robots in 3d. Algorithm consist of 
	1. Pull: uses classical COG-algorithm to move robots towards the center of gravity
	2. Spin: moves robots along a cyclic orbit to avoid deadlock-situations
--]]

--lua-function for calculating distances
function dist(vector)
	return Geometry.compute_distance(zero, vector);
end

--scalar product
function spr(a, b)
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
end

function spin()

	robots = get_visible_robots();

	--calculate center
	--TODO(calculate center and radius of smallest enclosing sphere + ellipsoid)
	center = cog(robots);
	
	--calculate radius of sphere
	radius = dist(center);
	
	--plane is defined by 1 point and 2 direction-vector
	--point is the origin of the local coordinate system (0,0,0);
	--1st direction-vector is vector to midpoint of sphere = cog
	--2nd direction-vector choosen orthogonal to 1st direction-vector by fitting z-coordinate in a way that the scalar product becomes zero
	d1vec = center;
	
	--initialize d2vec by random (it will be modified to be orthogonal to d1vec)
	d2vec = Vector3d(math.random(), math.random(), math.random());

	--checking for errors
	if (d1vec == zero) then
		print ("Robot is directly on COG.");
		exit();
	elseif (d1vec.x ~= 0.0) then
		d2vec.x = ( (d1vec.y * d2vec.y) + (d1vec.z * d2vec.z) ) / ( (-1) * d1vec.x );
	elseif (d1vec.y ~= 0.0) then
		d2vec.y = ( (d1vec.x * d2vec.x) + (d1vec.z * d2vec.z) ) / ( (-1) * d1vec.y );
	elseif (d1vec.z ~= 0.0) then
		d2vec.z = ( (d1vec.x * d2vec.x) + (d1vec.y * d2vec.y) ) / ( (-1) * d1vec.z );
	end
	
	--normalize 2nd direction vector
	d2vec = d2vec / dist(d2vec) * radius;
	
	--choose angle for movement on the circle
	if (radius ~= 0.0) then
		alpha = 1.0 / radius;
	else
		print ("Robot is directly on COG.")
		exit();
	end

	--debugoutput for checking whether all vectors have same length and are indeed orthogonal 
	--print(dist(d1vec), dist(d2vec), radius);
	--print(spr(d1vec, d2vec));
	
	--calculate new position
	newposition = center + math.cos(alpha) * d1vec + math.sin(alpha) * d2vec;
	
	add_position_request(newposition);
	
end

--calculates center of gravity of all robots
function cog(robots)

	center = zero;

	for i = 1, #robots do
		center = center + get_position(robots[i]);
	end

	return center / (#robots+1);

end


--pulls all the robots towards the center of gravity - movement just by unit-distance
function pull()

	newposition = cog(get_visible_robots())
	add_position_request(newposition / dist(newposition));
 
 end

--let the robot waggle a little - about 0.2 * of the unit-distance in a random direction
function waggle(distance)
	
	newposition = Vector3d(math.random(), math.random(), math.random());
	add_position_request(newposition / dist(newposition) * distance) ;
 
end 

--[[------------------------------------------------
----		MAIN PROGRAM 	----
--------------------------------------------------]]
 function main()

	--setting global nullvector
	zero = Vector3d(0.0,0.0,0.0);

	--just pull
	pull();
	
	--Choose operation by random
--[[	
	if (math.random() <= 0.5) then
		pull();
	else 
		spin();
	end
--]]

	--use robottype to switch between operations (slightly abusing this as some sort of memory)
--[[
	if (get_robot_type(get_own_identifier()) == 0) then
		pull();
		add_type_change_request(1);
	else
		spin();
		add_type_change_request(0);
	end
--]]
 
 end
 