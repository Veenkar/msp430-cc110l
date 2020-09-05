################################################################
# PATHS
################################################################
ODIR=obj
LDIR=lib
IDIR=include
SRC=src
DEP=dep

################################################################
# FILES
################################################################
BUILDINFO=$(ODIR)/build.info

################################################################
# CONFIG
################################################################
PROJ_NAME=acc_rcv
CC=msp430-gcc
CFLAGS=-I$(IDIR) -mmcu=msp430g2553 --vv -g
#LIBS=-lm
LIBS=


################################################################
# FILE LISTS
################################################################
SOURCES := $(wildcard $(SRC)/*.c)
_SOURCES = $(patsubst $(SRC)/%,%,$(SOURCES))

_OBJ = $(patsubst %.c,%.o,$(_SOURCES))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

################################################################
# ALL
################################################################
all: $(PROJ_NAME)

################################################################
# FLAGS CHANGE DETECTION
################################################################
PREV_CFLAGS := $(shell if [ -f $(BUILDINFO) ]; then cat "${BUILDINFO}"; fi)
$(info curr flags: $(CFLAGS))
#$(info prev flags: $(PREV_CFLAGS))
$(shell if [ "$(PREV_CFLAGS)" != "$(CFLAGS)" ]; then echo "$(CFLAGS)" > $(BUILDINFO); fi)

################################################################
# MAIN
################################################################
$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

################################################################
# PHONIES CONFIG
################################################################
.PHONY: clean all

################################################################
# DEPENDENCIES
################################################################
$(DEP)/%.d: $(SRC)/%.c
	@set -e; rm -f $@; \
	$(CC) -M $(CPPFLAGS) -I $(IDIR) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(ODIR)/\1.o $@ : $(BUILDINFO) ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

################################################################
# INCLUDE DEPENDENCIES TO THIS MAKEFILE
################################################################
_DEPS = $(patsubst %.c,%.d,$(_SOURCES))
DEPS = $(patsubst %,$(DEP)/%,$(_DEPS))
-include $(DEPS)

################################################################
# OBJECTS
################################################################
$(ODIR)/%.o: $(SRC)/%.c $(DEP)/%.d
	$(CC) -c -o $@ $< $(CFLAGS)

################################################################
# CLEAN
################################################################
clean:
	rm -f $(PROJ_NAME) $(ODIR)/*.o $(BUILDINFO) $(DEP)/*.d $(ODIR)/*.d.* *~ core $(INCDIR)/*~
