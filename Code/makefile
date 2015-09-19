CC       =  gcc
CFLAGS   = -Wall -O2 -g
LIB      = -lSDL -lGLU -lGL -lm -lSDL_image -lSDL_mixer
INCLUDES = 

OBJ      = main.o fonctionnement.o obstacles_persos.o textures_son.o 
RM       = rm -f
BIN      = ThomasIsDead-InAnExplosion-
DIRNAME  = $(shell basename $$PWD)
BACKUP   = $(shell date +`basename $$PWD`-%m.%d.%H.%M.tgz)
STDNAME  = $(DIRNAME).tgz

all : $(BIN)


$(BIN) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) $(INCLUDES)  -o $(BIN)
	@echo "--------------------------------------------------------------"
	@echo "                 to execute type: ./$(BIN) "
	@echo "--------------------------------------------------------------"

main.o : main.c fonctionnement.c
	@echo "compile main"
	$(CC) $(CFLAGS) $(LIB) $(INCLUDES) -c $<  
	@echo "done..."

fonctionnement.o : fonctionnement.c textures_son.c obstacles_persos.c 
	@echo "compile fonctionnement"
	$(CC) $(CFLAGS) $(LIB) $(INCLUDES) -c $<  
	@echo "done..."

textures_son.o : textures_son.c 
	@echo "compile textures_son"
	$(CC) $(CFLAGS) $(LIB) $(INCLUDES) -c $<  
	@echo "done..."

obstacles_persos.o : obstacles_persos.c
	@echo "compile obstacles_persos"
	$(CC) $(CFLAGS) $(LIB) $(INCLUDES) -c $<  
	@echo "done..."



clean :	
	@echo "**************************"
	@echo "CLEAN"
	@echo "**************************"
	$(RM) *~ $(OBJ) $(BIN) 

tar : clean 
	@echo "**************************"
	@echo "TAR"
	@echo "**************************"
	cd .. && tar cvfz $(BACKUP) $(DIRNAME)

