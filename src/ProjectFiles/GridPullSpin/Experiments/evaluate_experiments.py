import os
import os.path
import re
import sys

import matplotlib
import numpy
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D



def main():
	if (len(sys.argv) != 2):
		print("Usage: " + sys.argv[0] + " <seed>")
		sys.exit(1)
	
	results = get_results("results_seed" + sys.argv[1])
	plot_results(results)



def get_results(result_dir_name):
	"""
	Parses the given result directory and returns a dictionary keyed with a tuple of the form (radius, rob_num). For
	each key, a list of running times for the corresponding configuration represents the value.
	"""
	
	# some regular expressions used for parsing	
	output_line_regexp = re.compile("^\s*(?P<TIME>[0-9]+)(\s*\S+\s*){12}(?P<MAX_L1DIST>[0-9]+)\s*$")
	dir_name_regexp = re.compile(result_dir_name + "/radius(?P<RADIUS>[0-9]+)_rnum(?P<ROBOT_NUMBER>[0-9]+)$")
	
	# dictionary that will hold the final results
	results = dict()
	
	# find and parse output files
	directory_walk = os.walk(result_dir_name)
	directory_walk.next() # skip root element
	for root, dirs, files in directory_walk:		
		# scan for configuration type (radius, robot_num) and create empty list in results dictionary to hold data
		dir_name_match = dir_name_regexp.search(root)
		radius = int(dir_name_match.group("RADIUS"))
		robot_number = int(dir_name_match.group("ROBOT_NUMBER"))
		results[(radius, robot_number)] = list()
		
		# scan output files for this configuration
		for file in files:
			# skip gnuplot files
			if not file.startswith("output"):
				continue
			
			output_file = open(os.path.join(root, file))
			output_file_values = list()
			for output_line in output_file:
				output_line_match = output_line_regexp.match(output_line)
				if (output_line_match):
					time = int(output_line_match.group("TIME"))
					max_l1dist = int(output_line_match.group("MAX_L1DIST"))
					output_file_values.append((time, max_l1dist))
			output_file.close()
			
			finish_time = output_file_values[-1][0]
			results[(radius, robot_number)].append(finish_time)
	
	return results



def plot_results(results):
	"""
	Takes a dictionary keyed with tuples of the form (radius, rob_num) and lists of finish times as values and generates
	plots from this values.
	"""
	
	# prepare data
	radii, robot_numbers = zip(*results.keys())
	radii = list(set(radii))
	robot_numbers = list(set(robot_numbers))
	radii.sort()
	robot_numbers.sort()	
	np_radii, np_robot_numbers = numpy.meshgrid(radii, robot_numbers)
	np_max_finish_times = numpy.array([[max(results[radius, robot_number]) for radius in radii] for robot_number in robot_numbers])
	np_mean_finish_times = numpy.array([[numpy.mean(results[radius, robot_number]) for radius in radii] for robot_number in robot_numbers])
	np_min_finish_times = numpy.array([[min(results[radius, robot_number]) for radius in radii] for robot_number in robot_numbers])
	
	# prepare axes
	fig = pyplot.figure()
	subplot1 = fig.add_subplot(111)
	subplot2 = fig.add_subplot(222)
	subplot3 = fig.add_subplot(223)
	subplot4 = fig.add_subplot(224)
	ax1 = Axes3D(fig, rect=subplot1.get_position())
#	ax2 = Axes3D(fig, rect=subplot2.get_position())
#	ax3 = Axes3D(fig, rect=subplot3.get_position())
#	ax4 = Axes3D(fig, rect=subplot4.get_position())
	ax1.set_xlabel('$R$ (initial $L^1$ radius)')
	ax1.set_ylabel('$n$ (number of robots)')
	ax1.set_zlabel('$t$ (running time)')
#	ax2.set_xlabel('$R$ (initial $L^1$ radius)')
#	ax2.set_ylabel('$n$ (number of robots)')
#	ax2.set_zlabel('$t$ (running time)')
#	ax3.set_xlabel('$R$ (initial $L^1$ radius)')
#	ax3.set_ylabel('$n$ (number of robots)')
#	ax3.set_zlabel('$t$ (running time)')
#	ax4.set_xlabel('$R$ (initial $L^1$ radius)')
#	ax4.set_ylabel('$n$ (number of robots)')
#	ax4.set_zlabel('$t$ (running time)')
	
	# proxy artists for labeling (surface plots seem to to support labels by themselves)
	proxy_artist_maxft = matplotlib.patches.Rectangle((0, 0), 1, 1, facecolor='red')
	proxy_artist_meanft = matplotlib.patches.Rectangle((0, 0), 1, 1, facecolor='green')
	proxy_artist_minft = matplotlib.patches.Rectangle((0, 0), 1, 1, facecolor='blue')
	ax1.legend([proxy_artist_maxft, proxy_artist_minft, proxy_artist_meanft], ['maximum', 'mean', 'minimum'])
#	ax2.legend([proxy_artist_maxft], ['maximum'])
#	ax3.legend([proxy_artist_minft], ['mean'])
#	ax4.legend([proxy_artist_meanft], ['minimum'])

	
	# plot data
	ax1.plot_surface(np_radii, np_robot_numbers, np_max_finish_times, color='red', rstride=1, cstride=1)
	ax1.plot_surface(np_radii, np_robot_numbers, np_mean_finish_times, color='green', rstride=1, cstride=1)
	ax1.plot_surface(np_radii, np_robot_numbers, np_min_finish_times, color='blue', rstride=1, cstride=1)
#	ax2.plot_surface(np_radii, np_robot_numbers, np_max_finish_times, color='red', rstride=1, cstride=1)
#	ax3.plot_surface(np_radii, np_robot_numbers, np_min_finish_times, color='green', rstride=1, cstride=1)
#	ax4.plot_surface(np_radii, np_robot_numbers, np_mean_finish_times, color='blue', rstride=1, cstride=1)
	pyplot.show()



if (__name__ == "__main__"):
	main()
