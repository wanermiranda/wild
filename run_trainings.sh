#!/bin/sh
./tp3 eoac svm_states result TRAIN train0.set
./tp3 eoac svm_states result TRAIN train1.set
./tp3 eoac svm_states result TRAIN train2.set
./tp3 eoac svm_states result TRAIN train3.set
./tp3 eoac svm_states result TRAIN train4.set


./tp3 bic svm_states result TRAIN train0.set
./tp3 bic svm_states result TRAIN train1.set
./tp3 bic svm_states result TRAIN train2.set
./tp3 bic svm_states result TRAIN train3.set
./tp3 bic svm_states result TRAIN train4.set

./tp3 sasi svm_states result TRAIN train0.set
./tp3 sasi svm_states result TRAIN train1.set
./tp3 sasi svm_states result TRAIN train2.set
./tp3 sasi svm_states result TRAIN train3.set
./tp3 sasi svm_states result TRAIN train4.set



./tp3 eoac svm_states result valid test0.set
./tp3 eoac svm_states result valid test1.set
./tp3 eoac svm_states result valid test2.set
./tp3 eoac svm_states result valid test3.set
./tp3 eoac svm_states result valid test4.set


./tp3 bic svm_states result valid test0.set
./tp3 bic svm_states result valid test1.set
./tp3 bic svm_states result valid test2.set
./tp3 bic svm_states result valid test3.set
./tp3 bic svm_states result valid test4.set

./tp3 sasi svm_states result valid test0.set
./tp3 sasi svm_states result valid test1.set
./tp3 sasi svm_states result valid test2.set
./tp3 sasi svm_states result valid test3.set
./tp3 sasi svm_states result valid test4.set