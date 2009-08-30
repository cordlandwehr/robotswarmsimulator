import os
import os.path
import re
import commands
import sys



RSS_EXECUTABLE = "../../../../build/src/UserInterfaces/RobotSwarmSimulator"
RSS_COMMAND_TEMPLATE = RSS_EXECUTABLE + " --project-file {PRJ_FILE} --output {OUT_DIR} {ARGS}"



def main():
	if (len(sys.argv) < 2):
		print("Usage: " + sys.argv[0] + " <seed> <further_RSS_arguments>")
		sys.exit(1)
	
	if len(sys.argv) > 2:
		arguments = " ".join(sys.argv[2:])
	else:
		arguments = ""
		
	# create result directory
	result_dir_name = "results_seed" + sys.argv[1]
	os.mkdir(result_dir_name)
	
	swarm_files_base_dir = "swarm_files_seed" + sys.argv[1]
	dir_name_regexp = re.compile(swarm_files_base_dir + "/radius(?P<RADIUS>[0-9]+)_rnum(?P<ROBOT_NUMBER>[0-9]+)$")
	file_name_regexp = re.compile("seed(?P<SEED>[0-9]+).swarm$") # currently not used
	
	
	directory_walk = os.walk(swarm_files_base_dir)
	root, dirs, files = directory_walk.next() # skip root element
	configuration_number = len(dirs)
	run_number = None
	count = 1
	for root, dirs, files in directory_walk:
		if not run_number:
			run_number = len(files)
			
		parsed_parameters = dir_name_regexp.search(root).groupdict()
		output_directory_name = os.path.join(result_dir_name, "radius" + parsed_parameters["RADIUS"] + "_rnum" + parsed_parameters["ROBOT_NUMBER"])
		
		for file in files:
			swarm_file = os.path.join(root, file)
			rss_command = RSS_COMMAND_TEMPLATE.format(PRJ_FILE=swarm_file, OUT_DIR=output_directory_name, ARGS=arguments)
			print("Running (" + str(count) + "/" + str(configuration_number*run_number) + "): " + rss_command)
			(status, cmd_output) = commands.getstatusoutput(rss_command)
			if status != 0:
				print(cmd_output)
				error_file = open("error.log", 'a')
				error_file.write(cmd_output)
				error_file.close()
			count += 1



if (__name__ == "__main__"):
	main()
