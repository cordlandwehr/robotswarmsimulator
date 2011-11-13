#!/bin/bash
# config
PROJECTS=( 'MoveOnePos' 'DirectlyToOtherNode' 'ToOtherNodeWithProbability' 'DirectlyToOtherNodeWithOneWayThreshold' 'ToOtherNodeWithProbability2_m25_s1' )
n=${#PROJECTS[@]}
maxp=1
depth=4
ONIOS=true

# vars
running=0
index=0

# check as long as there are jobs left
while [ $index -lt $n ]
do
	# count current simulator processes
	running=`ps -A | grep [R]obotSwarmSimul | wc -l`

	if [ $running -lt $maxp ]
	then
		# start a new job
		sh anm.sh ${PROJECTS[$index]} $depth $ONIOS &
		index=$(($index + 1))
		echo "Running job #"$index" ..."
	fi
	
	# sleep for a second
	sleep 3
done
