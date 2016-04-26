PROGNAME=trazer

CC=g++

TRZSRCPATH=$(PWD)/../../source
RKHPATH=$(PWD)/../../rkh/source

DEFINE= -D__LINUX__ -DLINUX_DEBUG -D__TRAZER__ -DN__cplusplus -DN_DEBUG -D_CONSOLE -D__LNXGNU__
CFLAGS = -ansi -Wall -I$(TRZSRCPATH)/include -I$(RKHPATH)/include -I$(PWD)
LDFLAGS	= -lc -pthread

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
