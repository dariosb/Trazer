PROGNAME=trazer

CC=g++

TRZSRCPATH=$(PWD)/../../source

DEFINE= -D__LINUX__ -DLINUX_DEBUG -DN__cplusplus -DN_DEBUG -D_CONSOLE -D__W32STVC08__
CFLAGS = -ansi -Wall -I$(TRZSRCPATH)/include -I$(PWD)
LDFLAGS	= -lc

trzobjects:=$(patsubst %.cpp,%.o,$(wildcard $(TRZSRCPATH)/*.cpp)) 
trazer:=$(patsubst $(TRZSRCPATH)/%, %,$(trzobjects)) 

objects:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

$(PROGNAME): $(objects) $(trzobjects)
	@$(CC) -o $@ $(OBJ) $(objects) $(trazer) $(LDFLAGS)


%.o: %.cpp
	@$(CC) $(CFLAGS) -c $(DEFINE) $<

clean: 
	rm -rf *.o $(PROGNAME) 

tags:
	ctags -R $(TRZSRCPATH) $(PWD)
