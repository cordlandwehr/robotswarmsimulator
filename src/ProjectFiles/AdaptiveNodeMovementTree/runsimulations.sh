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
        )


SEEDS=(
	  '42'
	  '23'
	  '1337'
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
		sh anm.sh ${SEEDS[$indexS]} ${PROJECTS[$index]} $depth $ONIOS ${PROJECTS[(($index + 1))]} &
		index=$(($index + 2))
		echo "Running job #"$(($index/2))" ..."
	fi
	
	# sleep for a second
	sleep 3
  done
  indexS=$(($indexS + 1))

done
