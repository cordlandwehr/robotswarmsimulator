#!/bin/bash
# config

PROJECTS=(
	'DirectlyToOtherNode' ' ' 
	'DirectlyToOtherNodeWithFixedThreshold' '3'
	'DirectlyToOtherNodeWithFixedThreshold' '4' 
	'DirectlyToOtherNodeWithFixedThreshold' '5'
	'DirectlyToOtherNodeWithFixedThreshold' '6'
	'DirectlyToOtherNodeWithFixedThreshold' '7'
	'DirectlyToOtherNodeWithFixedThreshold' '8'
	'DirectlyToOtherNodeWithFixedThreshold' '9'
	'DirectlyToOtherNodeWithFixedThreshold' '10'
	'DirectlyToOtherNodeWithOneWayThreshold' '3' 
	'DirectlyToOtherNodeWithOneWayThreshold' '4' 
	'DirectlyToOtherNodeWithOneWayThreshold' '5' 
	'DirectlyToOtherNodeWithOneWayThreshold' '6' 
	'DirectlyToOtherNodeWithOneWayThreshold' '7' 
	'DirectlyToOtherNodeWithOneWayThreshold' '8' 
	'DirectlyToOtherNodeWithOneWayThreshold2' '3'
	'DirectlyToOtherNodeWithOneWayThreshold2' '4'
	'DirectlyToOtherNodeWithOneWayThreshold2' '5'
	'DirectlyToOtherNodeWithOneWayThreshold2' '6'
	'DirectlyToOtherNodeWithOneWayThreshold2' '7'
	'DirectlyToOtherNodeWithOneWayThreshold2' '8'
	'DirectlyToOtherNodeWithOneWayThreshold2' '9'
	'MoveOnePos' ' '
	'ToOtherNodeWithProbability' '0.25'
	'ToOtherNodeWithProbability' '0.1'
	'ToOtherNodeWithProbability' '0.2'
	'ToOtherNodeWithProbability' '0.3'
	'ToOtherNodeWithProbability' '0.4'
	'ToOtherNodeWithProbability' '0.5'
	'ToOtherNodeWithProbability' '0.6'
	'ToOtherNodeWithProbability' '0.7'
	'ToOtherNodeWithProbability' '0.75'
	'ToOtherNodeWithProbability2' '0.25 1' 
	'ToOtherNodeWithProbability2' '0.5 1' 
	'ToOtherNodeWithProbability2' '0.75 1' 
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

# the actual seeds are no longer used, so the number don't matter
SEEDS=(
	  '42'
	  '23'
	  '1337'
	  '3.14'
	  'mutterBeimer'
	  'karltoffel'
      )


n=${#PROJECTS[@]}
nS=${#SEEDS[@]}
maxp=3
depth=7
ONIOS=true

# vars
running=0
indexS=0

while [ $indexS -lt $nS ]
do
  echo "Initiating with seed no #"$indexS" ..."
  # check as long as there are jobs left
  
  index=0
  while [ $index -lt $n ]
  do
	# count current simulator processes
	running=`ps -A | grep [R]obotSwarmSimul | wc -l`

	if [ $running -lt $maxp ]
	then
		# start a new job
		DATE=`date +%s`
		#sh anm.sh $DATE ${PROJECTS[$index]} $depth $ONIOS ${PROJECTS[(($index + 1))]} &
		index=$(($index + 2))
		echo "Running job #"$(($index/2))" ... with seed " $DATE "."
	fi
	
	# sleep for a second
	sleep 3
  done
  indexS=$(($indexS + 1))

done
