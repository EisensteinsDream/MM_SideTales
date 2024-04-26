# COMPILER #

CC=gcc -O3 -DPLEX_RECDIRECTORY=\"../resources/\" -DPLEX_HEADERLESSDIRECTORY=\"../headerlesstex/\"

# FLAGS #

FLAGS=-Wall -Wextra -Wno-sign-compare -Werror -Wno-strict-prototypes -pedantic -std=c18 -march=native -I$(INC) -I$(HINC) -Wno-unused-parameter
OBJFLAGS=-c
LOBJFLAGS=-fPIC
LIBFLAGS=-shared
LLIBS=-pthread -lrt -lm

# DIRECTORIES #

INC=./include/
HINC=./include/plex
SRC=./src

OBJD=./obj
LIBD=./lib

ROBJD=$(OBJD)/release

RLIBD=$(LIBD)/release

# OBJECTS TO INCLUDE #

ROBJ=$(ROBJD)/mmcommon.o $(ROBJD)/mmthemes.o

# PHONIES #

.PHONY: all release clean rclean rebuild rrebuild

all: release
clean: 
	rm $(ROBJD)/*; rm $(RLIBD)/*;

rebuild:
	make rclean; sh updateplex.sh; make release;

release: $(ROBJ) $(RLIBD)/libMM_COMMON.so

rclean:
	rm $(ROBJD)/*; rm $(RLIBD)/*;

rrebuild:
	make rclean; sh updateplex.sh; make release;

# HEADER RULES #

HRPLEX=$(HINC)/plex.h

HRLANGUAGE=$(INC)/meta/mmcommon_language.h $(INC)/meta/language/mmcommon_language_en.h
HRFILENAME=$(INC)/meta/mmcommon_filepaths.h $(HRLANGUAGE)

HRMMCOMMON=$(INC)/mmcommon.h $(SRC)/mmcommon.c $(HRPLEX)

HRMMTHEMES=$(INC)/mmthemes.h $(SRC)/mmthemes.c $(HRPLEX) $(HRFILENAME) $(HRFILENAME)

# OBJ COMPILES #

$(ROBJD)/mmcommon.o : $(HRMMCOMMON)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mmcommon.c -o $(ROBJD)/mmcommon.o

$(ROBJD)/mmthemes.o : $(HRMMTHEMES)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mmthemes.c -o $(ROBJD)/mmthemes.o

# LIBRARY COMPILES #

$(RLIBD)/libMM_COMMON.so : $(ROBJ)
	$(CC) $(LIBFLAGS) $(ROBJ) -o $(RLIBD)/libMM_COMMON.so -L$(RLIBD) -lPLEX $(LLIBS)
