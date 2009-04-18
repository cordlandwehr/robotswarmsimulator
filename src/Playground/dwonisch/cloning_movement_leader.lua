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
		View.add_position_request(movement_vectors[(i % #movement_vectors) + 1]);
		i = i + 1;
	end	
end
