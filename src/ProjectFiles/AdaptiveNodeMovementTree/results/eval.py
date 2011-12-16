from glob import glob

strategies = ["DirectlyToOtherNodeWithFixedThreshold",
	"DirectlyToOtherNodeWithOneWayThreshold",
	"DirectlyToOtherNodeWithOneWayThreshold2",
	"ToOtherNodeWithProbability",
	"ToOtherNodeWithProbability2",
	"DirectlyToOtherNodeWithFixedThreshold",
	"ToOtherNodeWithDynamicThreshold"]
	
metrics = ["phi", "cost"]

data_path = "./anm_%s_*/output/*%s.dat"
plot_path = "./%s_%s.plt"

for strategy in strategies:
	for metric in metrics:
		print "Strategy: %s, metric: %s" % (strategy, metric)
		files = glob(data_path % (strategy, metric))
		handler = open(plot_path % (strategy, metric), "w")
		
		if metric == "phi":
			handler.write("set terminal pdf\n")
			handler.write("set output '%s_phi.pdf'\n" % strategy)
			handler.write("set xlabel 'rounds (in 1000)'\n")
			#handler.write("set ylabel 'value of phi'\n")
			handler.write("set yrange [0:1000]\n")
			handler.write("plot ")
			for i in range(len(files)):
				handler.write("'%s' using ($1/1000):2 with lines title '%s'" % (files[i], files[i].split("/")[-1]))
				if i < len(files)-1:
					handler.write(", \\\n")
			handler.close()
				
		else:
			handler.write("set terminal pdf\n")
			handler.write("set output '%s_cost.pdf'\n" % strategy)
			handler.write("set xlabel 'rounds (in 1000)'\n")
			handler.write("plot ")
			for i in range(len(files)):
				handler.write("'%s' using ($1/1000):2 with lines title '%s'" % (files[i], files[i].split("/")[-1]))
				if i < len(files)-1:
					handler.write(", \\\n")
			handler.close()
	