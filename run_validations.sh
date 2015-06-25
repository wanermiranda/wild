echo "Starting validation\n"
echo "./tp3 $1 svm_states/train0.$1.xml result valid resources/test0.set"

./tp3 $1 svm_states/train0.$1.xml result valid resources/test0.set &
./tp3 $1 svm_states/train1.$1.xml result valid resources/test1.set &
./tp3 $1 svm_states/train2.$1.xml result valid resources/test2.set &
./tp3 $1 svm_states/train3.$1.xml result valid resources/test3.set &
./tp3 $1 svm_states/train4.$1.xml result valid resources/test4.set

