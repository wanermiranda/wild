# Compile/Link
G++ = g++ 
CFLAGS= -g
LIBS = `pkg-config opencv --libs`

# App name
APPNAME = tp3

#Object
OBJS = `pkg-config opencv --cflags` $(APPNAME).cpp

#Parameters
DIR = ../unser/
ARG1 =  UNSER
ARG2 =  $(DIR)svm.xml
ARG3 =  $(DIR)result.txt

#Train parameters
ARG4 =  TRAIN
ARG5 =  $(DIR)train/

#Validation parameters
#ARG4 =  VALID
#ARG5 =  $(DIR)valid/


release : ; $(G++) $(OBJS) -o $(APPNAME) $(LIBS) $(CFLAGS)

clean :
	rm -f $(APPNAME) *.o

run :
	make 
	./$(APPNAME) $(ARG1) $(ARG2) $(ARG3) $(ARG4) $(ARG5)

