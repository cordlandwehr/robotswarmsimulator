# starts anm simulation

NAME=$1
DEPTH=$2
ONIOS=$3
DIR="anm_"$NAME
STEPS=$(($DEPTH * DEPTH * 1000))

# create new directory and copy project files

mkdir $DIR
#cp "adaptive_node_movement_"$NAME".swarm" ./$DIR
cp "generate_scenario.lua" ./$DIR
cp "anm_generics.lua" ./$DIR
cp *.csv ./$DIR

# change into new directory and generate script

cd ./$DIR
mkdir output
cp "../adaptive_node_movement_"$NAME"_header.lua" "adaptive_node_movement_"$NAME".lua" 
lua generate_scenario.lua $NAME $DEPTH "adaptive_node_movement_"$NAME".lua" $ONIOS "adaptive_node_movement_"$NAME".swarm"

# start the simulation
../../../../build/src/UserInterfaces/RobotSwarmSimulator --project-file "adaptive_node_movement_"$NAME".swarm" --blind --steps $STEPS --history-length 256

# run gnuplot
cd output
gnuplot *.plt
cd ..
