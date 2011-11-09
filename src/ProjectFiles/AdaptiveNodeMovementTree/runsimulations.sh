# config
projects=( "MoveOnePos" "ToOtherNodeWithProbability" "DirectlyToOtherNode" "DirectlyToOtherNodeWithProbability" )
n=4
maxp=3
depth=10
onios=true

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
		sh anm.sh ${projects[$index]} depth onios &
		index=$(($index + 1))
		echo "Running job #"$index" ..."
	fi
	
	# sleep for a second
	sleep 1
done
