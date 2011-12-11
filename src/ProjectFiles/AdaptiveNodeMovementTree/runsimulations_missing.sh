#!/bin/bash
# config

PROJECTS=(
	'DirectlyToOtherNodeWithOneWayThreshold2' '3'
	'DirectlyToOtherNodeWithOneWayThreshold2' '4'
	'DirectlyToOtherNodeWithOneWayThreshold2' '5'
	'DirectlyToOtherNodeWithOneWayThreshold2' '6'
	'DirectlyToOtherNodeWithOneWayThreshold2' '7'
	'DirectlyToOtherNodeWithOneWayThreshold2' '8'
	'DirectlyToOtherNodeWithOneWayThreshold2' '9'
        )
n=${#PROJECTS[@]}
maxp=3
depth=7
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
		sh anm.sh ${PROJECTS[$index]} $depth $ONIOS ${PROJECTS[(($index + 1))]} &
		index=$(($index + 2))
		echo "Running job #"$index" ..."
	fi
	
	# sleep for a second
	sleep 3
done
