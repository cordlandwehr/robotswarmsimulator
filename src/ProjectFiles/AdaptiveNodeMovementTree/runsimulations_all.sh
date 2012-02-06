#!/bin/bash
# config

PROJECTS=(
	'DirectlyToOtherNodeWithOneWayThreshold2' '4'
	'DirectlyToOtherNodeWithOneWayThreshold2' '8'
	'DirectlyToOtherNodeWithOneWayThreshold2' '16'
	'DirectlyToOtherNodeWithOneWayThreshold2' '24'
	'DirectlyToOtherNodeWithOneWayThreshold2' '32'
        )

# the actual seeds are no longer used, so the number don't matter
SEEDS=(
	  '5'
'6'
'7'
'8'
'9'
      )


n=${#PROJECTS[@]}
nS=${#SEEDS[@]}
maxp=4
depth=8
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
		sh anm.sh $DATE ${PROJECTS[$index]} $depth $ONIOS ${PROJECTS[(($index + 1))]} &
		index=$(($index + 2))
		echo "Running job #"$(($index/2))" ... with seed " $DATE "."
	fi
	
	# sleep for a second
	sleep 3
  done
  indexS=$(($indexS + 1))

done
