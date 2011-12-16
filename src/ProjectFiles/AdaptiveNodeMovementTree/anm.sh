# starts anm simulation

SEED=$1
NAME=$2
DEPTH=$3
ONIOS=$4
DIR="anm_"$NAME
STEPS=$(($DEPTH * $DEPTH * 10000))
#STEPS=$(($DEPTH * $DEPTH * 1))

shift; shift; shift; shift

params=""
paramsLine=""
while [ $# -gt 0 ]
do
  params=$params" "$1
  paramsLine=$paramsLine"_"$1
  shift
done

DIR="results/"$DIR"_"$DEPTH$paramsLine"_"$SEED

# create new directory and copy project files

mkdir results

mkdir $DIR
#cp "adaptive_node_movement_"$NAME".swarm" ./$DIR
cp "generate_scenario.lua" ./$DIR
cp "anm_generics.lua" ./$DIR
cp *.csv ./$DIR

# change into new directory and generate script

cd ./$DIR
mkdir output
cp "../../adaptive_node_movement_"$NAME"_header.lua" "adaptive_node_movement_"$NAME".lua" 
lua generate_scenario.lua $SEED $NAME $DEPTH "adaptive_node_movement_"$NAME".lua" $ONIOS "adaptive_node_movement_"$NAME".swarm" $paramsLine $params

# start the simulation
../../../../../build/src/UserInterfaces/RobotSwarmSimulator --project-file "adaptive_node_movement_"$NAME".swarm" --blind --steps $STEPS --history-length 256

# run gnuplot
cd output
gnuplot *.plt
cd ..

# todo: move files one level higher and remove garbage

