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
plot_path = "./summary/%s_%s#%s.plt"
seeds = {} 

for strategy in strategies:
	for metric in metrics:	    
		print "Strategy: %s, metric: %s" % (strategy, metric)
		
		files = glob(data_path % (strategy, metric))
		
		experiments = {} 
		
		for file in files:
			prefix = file.split("#")[0]				# zerteile String bei "#" und waehle ersten Teilstring
			if not experiments.has_key(prefix):
				experiments[prefix] = {}			# lege ggf. neue Liste an
			seed = file.split("#")[1].split(".")[0]
			experiments[prefix][seed] = file		# fuege Datei zu der Gruppe dieses Praefixes hinzu			
		
		for key in experiments.keys():
			#print "key: %s" % key
			sims = experiments[key]				# sims ist unsere Gruppe von Simulationen 
			for sim in sims:
				#seed = file.split("#")[0].split(".")[0]
				#print "sim: %s" % sim
				
				seed = sim.split(metric)[0]
				#print "seed: %s" % seed
				
				handler = open(plot_path % (strategy, metric, seed), "w")
				if metric == "phi":
					handler.write("set terminal pdf\n")
					handler.write("set output '%s_phi#%s.pdf'\n" % (strategy, seed))
					handler.write("set xlabel 'rounds (in 1000)'\n")
					#handler.write("set ylabel 'value of phi'\n")
					handler.write("set yrange [0:1000]\n")
					handler.write("plot ")
					first = True
					
					for i in range(len(files)):					      
						if files[i].count("#%s%s.dat" % (seed, metric)) > 0:
							if not first:
								handler.write(", \\\n")
							else:
								first = False
							handler.write("'%s%s' using ($1/1000):2 with lines title '%s'" % ("../", files[i], files[i].split("/")[-1]))
								
					handler.close()						
				else:
					handler.write("set terminal pdf\n")
					handler.write("set output '%s_cost#%s.pdf'\n" % (strategy, seed))
					handler.write("set xlabel 'rounds (in 1000)'\n")
					handler.write("plot ")					
					first = True
					
					for i in range(len(files)):
						if files[i].count("#%s%s.dat" % (seed, metric)) > 0:
							if not first:
								handler.write(", \\\n")
							else:
								first = False						  
							handler.write("'%s%s' using ($1/1000):2 with lines title '%s'" % ("../", files[i], files[i].split("/")[-1]))
					handler.close()
			