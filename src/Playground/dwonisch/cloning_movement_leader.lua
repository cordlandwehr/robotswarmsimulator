last_activation = nil;
i = 0;

function main()
	local movement_vectors = { 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(0,1,0), 
		Vector3d(0,1,0), 
		Vector3d(0,1,0), 
		Vector3d(0,1,0), 
		Vector3d(0,1,0), 
		Vector3d(0,0,1), 
		Vector3d(0,1,0), 
		Vector3d(1,0,0),
		Vector3d(0,0,1),
		Vector3d(0,1,0),
		Vector3d(0,0,1),
		Vector3d(0,0,-1),
		Vector3d(0,-1,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0), 
		Vector3d(1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0),
		Vector3d(-1,0,0)				
	}
		
	if(last_activation == nil or View.get_time() - last_activation >= 9) then
		last_activation = View.get_time();
		local factor = 1;
		if(i > #movement_vectors) then
			factor = 2;
		end
		if(i > 2*#movement_vectors) then
			factor = 3;
		end
		if(i > 3*#movement_vectors) then
			factor = 4;
		end
		if(i > 4*#movement_vectors) then
			factor = 5;
		end
		View.add_position_request(factor * movement_vectors[(i % #movement_vectors) + 1]);
		i = i + 1;
	end	
end
