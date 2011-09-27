# starts Move & Forget simulation

NUM=$1
DIR="maf_with_"$NUM"_robots"
STEPS=$(($NUM * 96 * 48))

# create new directory and copy project files

mkdir $DIR
cp *.swarm ./$DIR
cp *.lua ./$DIR

# change into new directory and generate world

cd ./$DIR
lua generate_robots.lua $NUM move_and_forget.lua robot_circle.csv edge_circle.csv 

# start the simulation
../../../../build/src/UserInterfaces/RobotSwarmSimulator --project-file move_and_forget --blind --steps $STEPS --history-length 64

# run gnuplot
gnuplot *.plt
