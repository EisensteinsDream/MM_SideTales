# COMPILER #

CC=gcc -O3 -DPLEX_RECDIRECTORY=\"../resources/\" -DPLEX_HEADERLESSDIRECTORY=\"../headerlesstex/\"

# FLAGS #

FLAGS=-Wall -Wextra -Wno-sign-compare -Werror -Wno-strict-prototypes -Wno-unused-but-set-parameter -Wno-enum-compare -pedantic -std=c18 -march=native -I$(INC) -Wno-unused-parameter
OBJFLAGS=-c
LOBJFLAGS=-fPIC
LIBFLAGS=-shared
LIBS=-lsndfile -lSDL2 -lSDL2_image
LLIBS=-pthread -lm -lrt -lportaudio

# DIRECTORIES #

INC=./include
SRC=./src

OBJD=./obj
LIBD=./lib

ROBJD=$(OBJD)/release
RLIBD=$(LIBD)/release

# OBJECTS TO INCLUDE #

ROBJ=$(ROBJD)/terminate.o $(ROBJD)/memprofile.o $(ROBJD)/protoexpandarray.o $(ROBJD)/mem.o $(ROBJD)/expandarray.o $(ROBJD)/fakestrings.o $(ROBJD)/math.o $(ROBJD)/error.o $(ROBJD)/timer.o $(ROBJD)/defaults.o $(ROBJD)/graphics_low.o $(ROBJD)/texture_low.o $(ROBJD)/window.o $(ROBJD)/input.o $(ROBJD)/texture.o $(ROBJD)/graphics.o $(ROBJD)/animation.o $(ROBJD)/screeneffects.o $(ROBJD)/sound.o $(ROBJD)/generator.o $(ROBJD)/prominencemachine.o $(ROBJD)/fileparse.o $(ROBJD)/directory.o $(ROBJD)/datetime.o $(ROBJD)/spritetheater.o $(ROBJD)/plex.o

# PHONIES #

.PHONY: all release clean rclean rebuild rrebuild

all: release
clean: 
	rm $(ROBJD)/*; rm $(RLIBD)/*;

rebuild:
	make rclean; make release;

release: $(ROBJ) $(RLIBD)/libPLEX.so

rclean:
	rm $(ROBJD)/*; rm $(RLIBD)/*;

rrebuild:
	make rclean; make release;

# HEADER RULES #

HROSADAPT=$(INC)/meta/osadapt.h
HRHARDLIMITS=$(INC)/meta/hardlimits.h
HRSTANDARDNUMBERS=$(INC)/meta/standardnumbers.h
HRLANGUAGE=$(INC)/meta/language.h $(INC)/meta/language/language_en.h
HRERRORMESS=$(INC)/meta/errormess/errormess_en.h $(INC)/meta/errormess.h $(HRLANGUAGE)

HRCINCLUDE=$(INC)/meta/cinclude.h $(HROSADAPT) $(HRHARDLIMITS) $(HRERRORMESS) $(HRFILELOC) $(HRSTANDARDNUMBERS)

HRTERMINATE=$(INC)/terminate.h $(SRC)/terminate.c $(HRCINCLUDE)

HRMEMPROFILE=$(INC)/mem/memprofile.h $(SRC)/mem/memprofile.c $(HRCINCLUDE)
HRPROTOEXPANDARRAY=$(INC)/mem/protoexpandarray.h $(SRC)/mem/protoexpandarray.c $(HRCINCLUDE)
HRMEM=$(INC)/mem/mem.h $(SRC)/mem/mem.c $(HRPROTOEXPANDARRAY) $(HRMEMPROFILE)
HREXPANDARRAY=$(INC)/mem/expandarray.h $(SRC)/mem/expandarray.c $(HRMEM)

HRFAKESTRINGS=$(INC)/utility/fakestrings.h $(SRC)/utility/fakestrings.c $(HRCINCLUDE)
HRMATH=$(INC)/utility/math.h $(SRC)/utility/math.c $(HRCINCLUDE)

HRTIMER=$(INC)/utility/timer.h $(SRC)/utility/timer.c $(HRFAKESTRINGS)

HRDEFAULTS=$(INC)/meta/defaultnames.h $(INC)/meta/defaultnames/defaultnames_en.h $(INC)/utility/defaults.h $(SRC)/utility/defaults.c $(HRCINCLUDE) $(HRPROTOEXPANDARRAY) $(HRMEMPROFILE)

HRERROR=$(INC)/utility/error.h $(SRC)/utility/error.c $(HRTERMINATE)

HRTEXTURE_LOW=$(INC)/windowing/texture_low.h $(SRC)/windowing/texture_low.c $(SRC)/windowing/texture_low/texture_low_SDL.c.h $(HRDEFAULTS)

HRGRAPHICS_LOW=$(INC)/windowing/graphics_low.h $(SRC)/windowing/graphics_low.c $(SRC)/windowing/graphics_low/graphics_low_SDL.c.h $(HRTEXTURE_LOW)

HRWINDOW=$(INC)/windowing/window.h $(SRC)/windowing/window.c $(HRGRAPHICS_LOW)
HRINPUT=$(INC)/windowing/input.h $(SRC)/windowing/input.c $(HRWINDOW) $(HRMATH) $(HRTIMER) $(HRTERMINATE)
HRTEXTURE=$(INC)/windowing/texture.h $(SRC)/windowing/texture.c $(HRWINDOW)
HRGRAPHICS=$(INC)/windowing/graphics.h $(SRC)/windowing/graphics.c $(HRTEXTURE) $(HRINPUT) $(HRMEMPROFILE)
HRANIMATION=$(INC)/windowing/animation.h $(SRC)/windowing/animation.c $(HRGRAPHICS)

HRSCREENEFFECTS=$(INC)/windowing/screeneffects.h $(SRC)/windowing/screeneffects.c $(HRGRAPHICS)

HRSOUND=$(INC)/sound/sound.h $(SRC)/sound/sound.c $(HRCINCLUDE)

HRGENERATOR=$(INC)/utility/generator.h $(SRC)/utility/generator.c $(HRMATH) $(HRMEMPROFILE)

HRPROMINENCEMACHINE=$(INC)/utility/prominencemachine.h $(SRC)/utility/prominencemachine.c $(HRMATH) $(HRMEMPROFILE)

HRFILEPARSE=$(INC)/utility/fileparse.h $(SRC)/utility/fileparse.c $(HRFAKESTRINGS) $(HRMEMPROFILE)

HRDIRECTORY=$(INC)/utility/directory.h $(SRC)/utility/directory.c $(SRC)/utility/directory_unix.c.h $(HRFAKESTRINGS)

HRDATETIME=$(INC)/utility/datetime.h $(SRC)/utility/datetime.c $(HRCINCLUDE)

HRSPRITETHEATER=$(INC)/utility/spritetheater.h $(SRC)/utility/spritetheater.c $(HRGRAPHICS) $(HRTIMER) $(HRMEMPROFILE)

HRPLEX=$(INC)/plex.h $(SRC)/plex.c $(HRANIMATION) $(HRERROR) $(HRGENERATOR) $(HRFILEPARSE)

# OBJ COMPILES #

$(ROBJD)/terminate.o : $(HRTERMINATE)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/terminate.c -o $(ROBJD)/terminate.o

$(ROBJD)/memprofile.o : $(HRMEMPROFILE)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mem/memprofile.c -o $(ROBJD)/memprofile.o

$(ROBJD)/protoexpandarray.o : $(HRPROTOEXPANDARRAY)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mem/protoexpandarray.c -o $(ROBJD)/protoexpandarray.o

$(ROBJD)/mem.o : $(HRMEM)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mem/mem.c -o $(ROBJD)/mem.o

$(ROBJD)/expandarray.o : $(HREXPANDARRAY)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/mem/expandarray.c -o $(ROBJD)/expandarray.o

$(ROBJD)/fakestrings.o : $(HRFAKESTRINGS)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/fakestrings.c -o $(ROBJD)/fakestrings.o

$(ROBJD)/math.o : $(HRMATH)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/math.c -o $(ROBJD)/math.o

$(ROBJD)/error.o : $(HRERROR)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/error.c -o $(ROBJD)/error.o

$(ROBJD)/timer.o : $(HRTIMER)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/timer.c -o $(ROBJD)/timer.o

$(ROBJD)/defaults.o : $(HRDEFAULTS)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/defaults.c -o $(ROBJD)/defaults.o

$(ROBJD)/graphics_low.o : $(HRWINDOW)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/graphics_low.c -o $(ROBJD)/graphics_low.o

$(ROBJD)/texture_low.o : $(HRWINDOW)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/texture_low.c -o $(ROBJD)/texture_low.o

$(ROBJD)/window.o : $(HRWINDOW)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/window.c -o $(ROBJD)/window.o

$(ROBJD)/input.o : $(HRINPUT)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/input.c -o $(ROBJD)/input.o

$(ROBJD)/texture.o : $(HRTEXTURE)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/texture.c -o $(ROBJD)/texture.o

$(ROBJD)/graphics.o : $(HRGRAPHICS)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/graphics.c -o $(ROBJD)/graphics.o

$(ROBJD)/animation.o : $(HRANIMATION)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/animation.c -o $(ROBJD)/animation.o

$(ROBJD)/screeneffects.o : $(HRSCREENEFFECTS)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/windowing/screeneffects.c -o $(ROBJD)/screeneffects.o

$(ROBJD)/sound.o : $(HRSOUND)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/sound/sound.c -o $(ROBJD)/sound.o

$(ROBJD)/generator.o : $(HRGENERATOR)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/generator.c -o $(ROBJD)/generator.o

$(ROBJD)/prominencemachine.o : $(HRPROMINENCEMACHINE)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/prominencemachine.c -o $(ROBJD)/prominencemachine.o

$(ROBJD)/fileparse.o : $(HRFILEPARSE)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/fileparse.c -o $(ROBJD)/fileparse.o

$(ROBJD)/directory.o : $(HRDIRECTORY)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/directory.c -o $(ROBJD)/directory.o

$(ROBJD)/datetime.o : $(HRDATETIME)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/datetime.c -o $(ROBJD)/datetime.o

$(ROBJD)/spritetheater.o : $(HRSPRITETHEATER)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/utility/spritetheater.c -o $(ROBJD)/spritetheater.o

$(ROBJD)/plex.o : $(HRPLEX)
	$(CC) $(FLAGS) $(OBJFLAGS) $(LOBJFLAGS) $(SRC)/plex.c -o $(ROBJD)/plex.o

# LIBRARY COMPILES #

$(RLIBD)/libPLEX.so : $(ROBJ) $(HRPLEX)
	$(CC) $(LIBFLAGS) $(ROBJ) -o $(RLIBD)/libPLEX.so $(LIBS) $(LLIBS)
