#!/bin/bash
# config

# just a test file...

SEEDS=(
	  '42'
	  '23'
	  '1337'
      )

PROJECTS=(
		'DirectlyToOtherNode' ''
		'ToOtherNodeWithProbability' '0.25'
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
