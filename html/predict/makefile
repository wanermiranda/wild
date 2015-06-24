# Compile/Link
G++ = g++
LIBS = `pkg-config opencv --libs`

# App name
APPNAME = predict

#Object
OBJS = `pkg-config opencv --static --cflags` predict.cpp ../src/descriptors_helper.cpp

#Parameters
DIR = ../unser/
ARG1 =  UNSER
ARG2 =  $(DIR)svm.xml
ARG3 =  $(DIR)valid/001.bin

release : ; $(G++) $(OBJS) -o $(APPNAME) $(LIBS)

clean :
	rm -f $(APPNAME) *.o

run :
	make 
	./$(APPNAME) $(ARG1) $(ARG2) $(ARG3) 

