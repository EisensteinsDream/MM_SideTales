# COMPILER #

CC=clang -O3 -fsanitize=address,undefined

# FLAGS #

FLAGS=-Wall -Wextra -Wno-sign-compare -Werror -Wno-strict-prototypes -pedantic -std=c18 -march=native -I$(INC) -I$(HINC) -I$(HINC2) -Wno-unused-parameter
OBJFLAGS=-c
LOBJFLAGS=-fPIC
LIBFLAGS=-shared
LLIBS=-pthread -lrt -lm

# DIRECTORIES #

INC=./include/
HINC=./include/plex
HINC2=./include/mm_common
SRC=./src

OBJD=./obj
BIND=./bin
LIBD=./lib

ROBJD=$(OBJD)/release

RBIND=$(BIND)/release

RLIBD=$(LIBD)/release

# OBJECTS TO INCLUDE #

ROBJ=$(ROBJD)/game.o $(ROBJD)/minigame0.o $(ROBJD)/mms0.o $(ROBJD)/minigame1.o $(ROBJD)/mms1.o

# PHONIES #

.PHONY: all release debug clean rclean dclean rebuild rrebuild drebuild

all: release
clean: 
	rm $(ROBJD)/*; rm $(RBIND)/*; rm $(RLIBD)/libMMS.so;

rebuild:
	make clean; make release;

release: $(ROBJ) $(RLIBD)/libMMS.so $(RBIND)/main

# HEADER RULES #

HRPLEX=$(HINC)/plex.h
HRMMCOMMON=$(HINC2)/mmcommon.h

HRLANGUAGE=$(INC)/meta/mms_language.h $(INC)/meta/language/mms_language_en.h

HRFILEPATH=$(INC)/meta/mms_filepaths.h

HRMINIGAME0=$(INC)/minigame/minigame0.h $(SRC)/minigame/minigame0.c $(HRPLEX) $(HRMMCOMMON)

HRMINIGAME1=$(INC)/minigame/minigame1.h $(SRC)/minigame/minigame1.c $(HRPLEX) $(HRMMCOMMON)

HRMMS0=$(INC)/chapters/mms0.h $(SRC)/chapters/mms0.c $(HRMINIGAME0)

HRMMS1=$(INC)/chapters/mms1.h $(SRC)/chapters/mms1.c $(HRMINIGAME1)

HRGAME=$(INC)/game.h $(SRC)/game.c $(HRPLEX) $(HRMMCOMMON) $(HRLANGUAGE) $(HRFILEPATH) $(HRMMS0) $(HRMMS1)

# OBJ COMPILES #

## RELEASE ##

$(ROBJD)/game.o : $(HRGAME)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/game.c -o $(ROBJD)/game.o

$(ROBJD)/minigame0.o : $(HRMINIGAME0)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/minigame/minigame0.c -o $(ROBJD)/minigame0.o

$(ROBJD)/mms0.o : $(HRMMS0)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/chapters/mms0.c -o $(ROBJD)/mms0.o

$(ROBJD)/minigame1.o : $(HRMINIGAME1)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/minigame/minigame1.c -o $(ROBJD)/minigame1.o

$(ROBJD)/mms1.o : $(HRMMS1)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/chapters/mms1.c -o $(ROBJD)/mms1.o

# LIBRARY COMPILES #

$(RLIBD)/libMMS.so : $(ROBJ)
	$(CC) $(LIBFLAGS) $(ROBJ) -o $(RLIBD)/libMMS.so -L$(RLIBD) -lPLEX -lMM_COMMON  $(LLIBS)

# BINARY COMPILES #

$(RBIND)/main : $(SRC)/main.c $(HRGAME)
	$(CC) $(FLAGS) -Wno-unused-parameter $(SRC)/main.c -o $(RBIND)/main -L$(RLIBD) -lPLEX -lMM_COMMON -lMMS $(LLIBS)
