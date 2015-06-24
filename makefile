# Compile/Link
G++ = g++ 
CFLAGS= -g
LIBS = `pkg-config opencv --libs`

# App name
APPNAME = tp3

#Object
OBJS = `pkg-config opencv --cflags` main.cpp src/descriptors_helper.cpp


#Validation parameters
#ARG4 =  VALID
#ARG5 =  $(DIR)valid/


release : ; $(G++) $(OBJS) -o $(APPNAME) $(LIBS) $(CFLAGS)

clean :
	rm -f $(APPNAME) *.o

