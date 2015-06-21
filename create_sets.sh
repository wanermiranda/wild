#!/bin/sh
lines=$(wc -l < $1)
lines=$((lines / 5))
split -da 1 -l $lines $1 part --additional-suffix=".part"

cat part1.part part2.part part3.part part4.part > train0.set
cat part0.part > test0.set

cat part0.part part2.part part3.part part4.part > train1.set
cat part1.part > test1.set

cat part0.part part1.part part3.part part4.part > train2.set
cat part2.part > test2.set

cat part0.part part1.part part2.part part4.part > train3.set
cat part3.part > test3.set

cat part0.part part1.part part2.part part3.part > train4.set
cat part4.part > test4.set