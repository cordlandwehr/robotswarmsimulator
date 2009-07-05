# statistics of the simulation
#====================================
set title  " SCHLAUE SCHWÄRME \n Modell: ..." 
set xrange []
set yrange []
set grid
set pointsize 0.5
set xlabel 'time'
set ylabel ''
plot "output_Temp_ALL.plt" using 1:2 title "num_robots" with linespoints, "output_Temp_ALL.plt" using 1:3 title "num_masters" with linespoints, "output_Temp_ALL.plt" using 1:4 title "num_slaves" with linespoints, "output_Temp_ALL.plt" using 1:5 title "miniball_center_x" with linespoints, "output_Temp_ALL.plt" using 1:6 title "miniball_center_y" with linespoints, "output_Temp_ALL.plt" using 1:7 title "miniball_center_z" with linespoints, "output_Temp_ALL.plt" using 1:8 title "miniball_radius" with linespoints, "output_Temp_ALL.plt" using 1:9 title "miniball_movedist" with linespoints, "output_Temp_ALL.plt" using 1:10 title "visgraph_connected" with linespoints
