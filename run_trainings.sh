#!/bin/sh
echo "Starting resources/trainings\n"

./tp3 $1 svm_states result TRAIN resources/train0.set &
./tp3 $1 svm_states result TRAIN resources/train1.set &
./tp3 $1 svm_states result TRAIN resources/train2.set &
./tp3 $1 svm_states result TRAIN resources/train3.set &
./tp3 $1 svm_states result TRAIN resources/train4.set 


