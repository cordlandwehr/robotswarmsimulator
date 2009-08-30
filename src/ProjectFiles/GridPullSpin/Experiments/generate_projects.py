import os
import os.path
import random
import sys


# some script parameters
radii = [10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]
robot_numbers = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]



def main():
	if (len(sys.argv) != 4):
		print("Usage: " + sys.argv[0] + " <algorithm> <seed> <nr_of_runs>")
		sys.exit(1)
	
	# generate output directories
	robot_parent_dirname = "robot_files_seed" + str(sys.argv[2])
	swarm_parent_dirname = "swarm_files_seed" + str(sys.argv[2])
	os.mkdir(robot_parent_dirname)
	os.mkdir(swarm_parent_dirname)
	
	# generate parameter lists
	num_runs = int(sys.argv[3])
	robot_parameters_list = [(sys.argv[1], os.path.join(robot_parent_dirname, "radius" + str(radius) + "_rnum" + str(robot_number)), radius, robot_number)
	                         for radius in radii for robot_number in robot_numbers for i in range(num_runs)
	                        ]
	swarm_parameters_list = [(os.path.join(swarm_parent_dirname, "radius" + str(radius) + "_rnum" + str(robot_number)), radius, robot_number)
	                         for radius in radii for robot_number in robot_numbers for i in range(num_runs)
	                        ]
	
	# generate seeds using the input seed
	random.seed(int(sys.argv[2]))
	seeds = [random.randint(0, 1000000) for i in range(len(radii)*len(robot_numbers)*num_runs)]
	
	# generate .robot and .swarm files
	for (robot_parameters, swarm_parameters) in zip(robot_parameters_list, swarm_parameters_list):
		generate_robot_file(*robot_parameters, seed=seeds[-1])
		generate_swarm_file(*swarm_parameters, seed=seeds.pop())



def random_grid_point_in_radius(radius):
	"Returns a random, uniformly distributed grid point (as a tuple) with maximum L1 norm 'radius'."
	
	x, y = random.randint(-radius, radius), random.randint(-radius, radius)
	while (abs(x)+abs(y) > radius):
		x, y = random.randint(-radius, radius), random.randint(-radius, radius)
	return x, y

def random_grid_point_with_l1norm(norm):
	"Returns a random, uniformly distributed grid point having L1 norm 'norm'."
	
	x = random.randint(-norm, norm)
	y = (norm - abs(x)) * random.choice([-1,1])
	return x, y
	



def generate_robot_file(algorithm, output_directory_name, max_radius, robot_number, seed):
	"""
	Generates a robot file with the given file name and robot number. The seed is used to position the robots uniformly
	at random in the L1 sphere of radius max_radius. The robots will use the given algorithm. It is guaranteed that
	there is at least one robot having L1 norm exactly max_radius and no two robots share the same position.
	"""
	
	# some constants/templates
	header = '"ID","x-position","y-position","z-position","type","x-velocity","y-velocity","z-velocity","x-acceleration","y-acceleration","z-acceleration","status","marker-info","algorithm","color","x-axis-1","x-axis-2","x-axis-3","y-axis-1","y-axis-2","y-axis-3","z-axis-1","z-axis-2","z-axis-3"'
	robot_line_template = '{ID},{POS_X},{POS_Y},0,"SLAVE",0,0,0,0,0,0,"READY",0,"{ALGORITHM}",0,1,0,0,0,1,0,0,0,1'
	
	# create output directory (if it does not already exist)
	if (not os.path.exists(output_directory_name)):
		os.mkdir(output_directory_name)
	
	# create robot file and write header
	random.seed(seed)
	robot_file = open(os.path.join(output_directory_name, "seed" + str(seed) + ".robot"), 'w')
	robot_file.write(header + '\n')
	
	# used to store positions that are already occupied
	occupied_positions = dict((i,[]) for i in range(-max_radius, max_radius+1))
	
	# begin with a random robot having exactly L1 norm max_radius
	robot_pos_x, robot_pos_y = random_grid_point_with_l1norm(max_radius)
	occupied_positions[robot_pos_x].append(robot_pos_y)
	robot_line = robot_line_template.format(ID=0, POS_X=robot_pos_x, POS_Y=robot_pos_y, ALGORITHM=algorithm)
	robot_file.write(robot_line + '\n')
	
	for robot_id in range(1, robot_number):
		robot_pos_x, robot_pos_y = random_grid_point_in_radius(max_radius)
		while robot_pos_y in occupied_positions[robot_pos_x]:
			robot_pos_x, robot_pos_y = random_grid_point_in_radius(max_radius)
		occupied_positions[robot_pos_x].append(robot_pos_y)
		robot_line = robot_line_template.format(ID=robot_id, POS_X=robot_pos_x, POS_Y=robot_pos_y, ALGORITHM=algorithm)
		robot_file.write(robot_line + '\n')
	robot_file.write('\n')
	robot_file.close()



def generate_swarm_file(output_directory_name, max_radius, robot_number, seed):
	swarm_template_file = open("swarmfile.template")
	swarm_template = swarm_template_file.read()
	swarm_template_file.close()
	
	if (not os.path.exists(output_directory_name)):
		os.mkdir(output_directory_name)
	swarm_file = open(os.path.join(output_directory_name, "seed" + str(seed) + ".swarm"), 'w')
	swarm_file.write(swarm_template.format(SEEDSEED=sys.argv[2], RADIUS=max_radius, ROBOT_NUMBER=robot_number, SEED=seed))
	swarm_file.close()



if (__name__ == "__main__"):
	main()
