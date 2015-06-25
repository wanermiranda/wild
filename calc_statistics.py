#!/bin/python
import matplotlib.pyplot as plt
import os
import glob
import numpy as np




# classes apple,car,cow,cup,dog,horse,pear,tomato
          
classes = ["apple", "car", "cow", "cup", "dog", "horse", "pear","tomato"]
methods = ["bic", "sasi", "eoac"]

path = 'result/'
for method in methods:
    method_cfm = np.zeros((8,8), dtype=np.float)
    method_tp  = np.zeros(8, dtype=np.float)
    method_fp  = np.zeros(8, dtype=np.float)
    for result_file in glob.glob( os.path.join(path, '*' + method + '.res') ):
        print result_file    
        true_pos = np.zeros(8, dtype=np.float)
        false_pos = np.zeros(8, dtype=np.float)
        totals_clas = np.zeros(8, dtype=np.float)
        confusion_matrix = np.zeros((8,8), dtype=np.float)
        corrects = 0.0
        total = 0.0
        lines = [line.rstrip('\n') for line in open(result_file)]
        for line in lines:
            class_predict = line.split(' = ')
            class_name  =  class_predict[0]
            predict_name  =  class_predict[1]
            total += 1
            totals_clas[classes.index(class_name)] += 1
            confusion_matrix[classes.index(class_name), classes.index(predict_name)] += 1
            if classes.index(class_name) == classes.index(predict_name):
                corrects += 1
                true_pos[classes.index(class_name)] += 1
            else:
                false_pos[classes.index(class_name)] += 1

        print str(corrects) + " / " + str(total) + " = " + str(corrects/total)
        print "True Positives: " + str(true_pos / totals_clas)
        print "False Positives: " + str(false_pos / totals_clas)
        print confusion_matrix
        method_cfm += confusion_matrix
        method_fp += false_pos / total
        method_tp += true_pos / total



    plt.imshow(method_cfm, interpolation='nearest')
    plt.title(method)
    plt.xticks(np.arange(0,8), classes)
    plt.yticks(np.arange(0,8), classes)
    plt.savefig("result/" + method + "_cfm.png")    
