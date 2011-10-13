# config
sizes=( 320 192 288 160 256 128 224 96 64 )
n=9
maxp=3

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
		sh moveandforget.sh ${sizes[$index]} &
		index=$(($index + 1))
		echo "Running job #"$index" ..."
	fi
	
	# sleep for a second
	sleep 1
done
