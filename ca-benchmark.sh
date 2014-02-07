#!/bin/bash

COUNT=0
NUM_LOOP_ITERATIONS=$2

while [ $COUNT -lt $NUM_LOOP_ITERATIONS ]; do
	./lz77-sort <$1 >temp.out
	COUNT=$[$COUNT+1]
done