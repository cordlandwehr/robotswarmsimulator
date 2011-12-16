#!/bin/bash
# config

PROJECTS=(

	'DirectlyToOtherNodeWithFixedThreshold' '11'
	'DirectlyToOtherNodeWithFixedThreshold' '12'
	'DirectlyToOtherNodeWithFixedThreshold' '14'
	'DirectlyToOtherNodeWithFixedThreshold' '15'
	'DirectlyToOtherNodeWithFixedThreshold' '16'
	'DirectlyToOtherNodeWithFixedThreshold' '17'
	'DirectlyToOtherNodeWithOneWayThreshold' '9' 
	'DirectlyToOtherNodeWithOneWayThreshold' '10' 
	'DirectlyToOtherNodeWithOneWayThreshold' '11' 
	'DirectlyToOtherNodeWithOneWayThreshold' '12' 
	'DirectlyToOtherNodeWithOneWayThreshold' '13' 
	'DirectlyToOtherNodeWithOneWayThreshold' '14' 
	'DirectlyToOtherNodeWithOneWayThreshold2' '9'
	'DirectlyToOtherNodeWithOneWayThreshold2' '10'
	'DirectlyToOtherNodeWithOneWayThreshold2' '11'
	'DirectlyToOtherNodeWithOneWayThreshold2' '12'
	'DirectlyToOtherNodeWithOneWayThreshold2' '13'
	'DirectlyToOtherNodeWithOneWayThreshold2' '14'
	'DirectlyToOtherNodeWithOneWayThreshold2' '15'
	'ToOtherNodeWithProbability' '0.25'
	'ToOtherNodeWithProbability' '0.1'
	'ToOtherNodeWithProbability' '0.2'
	'ToOtherNodeWithProbability' '0.3'
	'ToOtherNodeWithProbability' '0.4'
	'ToOtherNodeWithProbability' '0.5'
	'ToOtherNodeWithProbability' '0.6'
	'ToOtherNodeWithProbability' '0.7'
	'ToOtherNodeWithProbability' '0.75'
	'DirectlyToOtherNodeWithFixedThreshold2' '2'
	'DirectlyToOtherNodeWithFixedThreshold2' '3'
	'DirectlyToOtherNodeWithFixedThreshold2' '4'
	'DirectlyToOtherNodeWithFixedThreshold2' '5'
	'DirectlyToOtherNodeWithFixedThreshold2' '6'
	'DirectlyToOtherNodeWithFixedThreshold2' '7'
	'DirectlyToOtherNodeWithFixedThreshold2' '8'
	'DirectlyToOtherNodeWithFixedThreshold2' '9'
	'DirectlyToOtherNodeWithFixedThreshold2' '10'
	'DirectlyToOtherNodeWithFixedThreshold2' '11'
	'DirectlyToOtherNodeWithFixedThreshold2' '12'
	'DirectlyToOtherNodeWithFixedThreshold2' '13'
	'DirectlyToOtherNodeWithFixedThreshold2' '14'
	'DirectlyToOtherNodeWithFixedThreshold2' '15'
	'ToOtherNodeWithDynamicThreshold' '2'
	'ToOtherNodeWithDynamicThreshold' '3'
	'ToOtherNodeWithDynamicThreshold' '4'
	'ToOtherNodeWithDynamicThreshold' '5'
	'ToOtherNodeWithDynamicThreshold' '6'
	'ToOtherNodeWithDynamicThreshold' '7'
	'ToOtherNodeWithDynamicThreshold' '8'
	'ToOtherNodeWithDynamicThreshold' '9'
	'ToOtherNodeWithDynamicThreshold' '10'
	'ToOtherNodeWithDynamicThreshold' '11'
	'ToOtherNodeWithDynamicThreshold' '12'
	'ToOtherNodeWithDynamicThreshold' '13'
	'ToOtherNodeWithDynamicThreshold' '14'
	'ToOtherNodeWithDynamicThreshold' '15'
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
