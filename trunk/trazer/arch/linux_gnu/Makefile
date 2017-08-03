PROGNAME=trazer

CC=g++

TRZSRCPATH=$(PWD)/../../source
RKHPATH=$(PWD)/../../rkh/source

#uncomment for trazer build
#DEFINE= -D__LINUX__ -DLINUX_DEBUG -D__TRAZER__ -DN_DEBUG -D_CONSOLE -D__LNXGNU__ -D__TEST__
#uncomment for unitrzlib.a first run "make" builds the objs, then run "make unitrzlib.a"
DEFINE= -D__LINUX__ -DLINUX_DEBUG -D__TRAZER__ -DN_DEBUG -D_CONSOLE -D__LNXGNU__ -D__TEST__ -D__UNITRAZER_LIB__ -D__TZLOG_STUB___
CFLAGS = -ansi -Wall -I$(TRZSRCPATH)/include -I$(TRZSRCPATH)/include/rkh -I$(RKHPATH)/include -I$(PWD)
LDFLAGS	= -lc -pthread

UNITRAZER_OBJS = cfgdep.o config.o error.o evexptbl.o evtbl.o \
				 messages.o rtimtbl.o seqdiag.o sigtbl.o symbtbl.o \
				 tzlog.o tzparse.o uevtbl.o unittrz.o utils.o


trzobjects:=$(patsubst %.cpp,%.o,$(wildcard $(TRZSRCPATH)/*.cpp)) 
trazer:=$(patsubst $(TRZSRCPATH)/%, %,$(trzobjects)) 

objects:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

$(PROGNAME): $(objects) $(trzobjects)
	@$(CC) -o $@ $(OBJ) $(objects) $(trazer) $(LDFLAGS)

unitrzlib.a: $(UNITRAZER_OBJS)
	ar rcs $@ $^	

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $(DEFINE) $<

clean: 
	rm -rf *.o $(PROGNAME) unitrzlib.a 

tags:
	ctags -R $(TRZSRCPATH) $(PWD)
