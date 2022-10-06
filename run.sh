#!/bin/bash

# ./run.sh <start_generation> <end_generation> <num_creatures>
# <num_creatures> must match number of .gene files in active_creatures folder

# run sim with creatures in active folder
# sim outputs a script that culls creatures
# run that script
# repeat
# every N generations, zip active folder to checkpoint progress


# sim <boarsize> <population> <steps> <simfile.sim> <genome01> <genome02> ...

echo "Running generations $1 through $2 with $3 creatures per generation"

#for i in {$1..$2}
for (( i=$1; i<=$2; i++ ))
do
  if (( $i % 20 == 0 ))
  then
    zip -q creatures/gen$i.zip active_creatures/*.gene
  fi
  ./sim 128 $3 2000 sims/g$i.sim active_creatures/*.gene > cull.sh
  zip -q sims/g$i.sim.zip sims/g$i.sim && rm sims/g$i.sim &
  ./cull.sh
  for file in active_creatures/c*.gene
  do
    name=${file##*/}
    mv $file active_creatures/g$i$name
  done
done
