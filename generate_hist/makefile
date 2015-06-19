# Compile/Link
GCC = gcc
#LIBS     = -LDescriptors/Color/lib
#INCLUDE  = -IDescriptors/Color/include
#DESCRIPT = -lcolordescriptors -ljpeg -lm
DESCRIPT = -ltexturedescriptors -ljpeg -lm
LIBS     = -LDescriptors/Texture/lib
INCLUDE  = -IDescriptors/Texture/include

# Nome do aplicativo
APPNAME = extrai_unser

#Objetos
OBJS = -O2 -Wall $(APPNAME).c 

#Parametros
ARG1 = images/pgm/apple1.pgm
ARG2 = images/apple1/apple1.pgm
ARG3 = unser.bin

release : ; $(GCC) $(OBJS) -o $(APPNAME) $(INCLUDE) $(LIBS) $(DESCRIPT)

clean :
	rm -f $(APPNAME) *.o

run :
	make 
	./$(APPNAME) $(ARG1) $(ARG2) $(ARG3) 

