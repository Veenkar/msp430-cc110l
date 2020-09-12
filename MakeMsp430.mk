########################################################################################################################
########################################################################################################################
##____/\\\\____________/\\\\_____/\\\\\\\\\_____/\\\________/\\\__/\\\\\\\\\\\\\\\____________________________________##
##____\/\\\\\\________/\\\\\\___/\\\\\\\\\\\\\__\/\\\_____/\\\//__\/\\\///////////____________________________________##
##_____\/\\\//\\\____/\\\//\\\__/\\\/////////\\\_\/\\\__/\\\//_____\/\\\______________________________________________##
##______\/\\\\///\\\/\\\/_\/\\\_\/\\\_______\/\\\_\/\\\\\\//\\\_____\/\\\\\\\\\\\_____________________________________##
##_______\/\\\__\///\\\/___\/\\\_\/\\\\\\\\\\\\\\\_\/\\\//_\//\\\____\/\\\///////_____________________________________##
##________\/\\\____\///_____\/\\\_\/\\\/////////\\\_\/\\\____\//\\\___\/\\\___________________________________________##
##_________\/\\\_____________\/\\\_\/\\\_______\/\\\_\/\\\_____\//\\\__\/\\\__________________________________________##
##__________\/\\\_____________\/\\\_\/\\\_______\/\\\_\/\\\______\//\\\_\/\\\\\\\\\\\\\\\_____________________________##
##___________\///______________\///__\///________\///__\///________\///__\///////////////_____________________________##
##__________________/\\\\\\\\\\\______________________________________________________________________________________##
##_________________/\\\/////////\\\___________________________________________________________________________________##
##_________________\//\\\______\///___________________________________________________________________________________##
##___________________\////\\\_________________________________________________________________________________________##
##_______________________\////\\\______________________veenkar@gmail.com______________________________________________##
##___________________________\////\\\_________________________________________________________________________________##
##_____________________/\\\______\//\\\_______________________________________________________________________________##
##_____________________\///\\\\\\\\\\\/_______________________________________________________________________________##
##________________________\///////////________________________________________________________________________________##
##_________________________/\\\\\\\\\\\\\________________________/\\\________/\\\\\\\\\\______/\\\\\\\________________##
##_________________________\/\\\/////////\\\____________________/\\\\\______/\\\///////\\\___/\\\/////\\\_____________##
##__________________________\/\\\_______\/\\\__________________/\\\/\\\_____\///______/\\\___/\\\____\//\\\___________##
##___________________________\/\\\\\\\\\\\\\/_________________/\\\/\/\\\____________/\\\//___\/\\\_____\/\\\__________##
##____________________________\/\\\/////////_________________/\\\/__\/\\\___________\////\\\__\/\\\_____\/\\\_________##
##_____make_MSP________________\/\\\________________________/\\\\\\\\\\\\\\\\___________\//\\\_\/\\\_____\/\\\________##
##_______not war________________\/\\\_______________________\///////////\\\//___/\\\______/\\\__\//\\\____/\\\________##
##_______________________________\/\\\_________________________________\/\\\____\///\\\\\\\\\/____\///\\\\\\\/________##
##________________________________\///__________________________________\///_______\/////////________\///////_.mk_____##
########################################################################################################################
## EXAMPLE MINIMAL CONFIG (other variables default)
########################################################################################################################
## VARIANT=msp430g2553  # launchpad
##
##
########################################################################################################################
## EXAMPLE FULL CONFIG (all config values used, but same as defaults, so equivalent to minimal)
########################################################################################################################
## # PATHS# build files
## ODIR=obj
## LDIR=lib
## IDIR=include
## SRC=src
## DEP=dep
##
## # FILES
## BUILDINFO=$(ODIR)/build.info
##
## # CONFIG
## PROJ_NAME=acc_rcv
## BUILD_TYPE=app
## CC=msp430-gcc
## VARIANT=msp430g2553
## DEBUG_FLAGS=-mmcu=msp430g2553 -O0 -g3 -ggdb -gdwarf-2
## WARN_FLAGS=-Wall -Wextra -Wshadow
## CFLAGS=-I$(IDIR) $(DEBUG_FLAGS) $(WARN_FLAGS) -mmcu=$(VARIANT) --vv -std=gnu90 -MMD
## #LIBS=-lm
## LIBS=
##
########################################################################################################################
########################################################################################################################
## DEFAULT FLAGS
########################################################################################################################
DEFAULT_BUILD_TYPE=app
DEFAULT_CC=msp430-gcc
DEFAULT_DEBUG_FLAGS=-O0 -g3 -ggdb -gdwarf-2
DEFAULT_WARN_FLAGS=-Wall -Wextra -Wshadow
DEFAULT_OTHER_FLAGS=--vv
DEFAULT_STANDARD=gnu90
# VARIANT - MUST SPECIFY in main Makefile, which includes this MakeMsp430.mk! Like this:
# VARIANT=msp430g2553

########################################################################################################################
## FLAGS PROCESSING
########################################################################################################################
MAKEFILE_NAME=$(firstword $(MAKEFILE_LIST))

ifeq ($(CC),cc)
	CC=$(DEFAULT_CC)
endif
ifeq ($(VARIANT),)
	$(error PLEASE SPECIFY VARIANT (in your main Makefile which includes this MakeMsp430.mk))
endif

IDIR_FLAGS=$(patsubst %, -I %,$(IDIR))

BUILD_TYPE:=$(or $(BUILD_TYPE),$(DEFAULT_BUILD_TYPE))
DEBUG_FLAGS:=$(or $(DEBUG_FLAGS),$(DEFAULT_DEBUG_FLAGS))
WARN_FLAGS:=$(or $(WARN_FLAGS),$(DEFAULT_WARN_FLAGS))
STANDARD:=$(or $(STANDARD),$(DEFAULT_STANDARD))
OTHER_FLAGS:=$(or $(OTHER_FLAGS),$(DEFAULT_OTHER_FLAGS))
# VARIANT - MUST SPECIFY in main Makefile, which includes this MakeMsp430.mk! Like this:
# VARIANT=msp430g2553

CFLAGS:=$(CFLAGS)		\
	$(IDIR_FLAGS)       \
	$(DEBUG_FLAGS)		\
	$(WARN_FLAGS)		\
	-mmcu=$(VARIANT)	\
	-STD$(STANDARD)		\
	$(OTHER_FLAGS)


########################################################################################################################
## FILE LISTS
########################################################################################################################
# full paths
SOURCES = $(wildcard $(SRC)/*.c)
# file names only
_SOURCES = $(patsubst $(SRC)/%,%,$(SOURCES))

#file names only
_OBJ = $(patsubst %.c,%.o,$(_SOURCES))
# full paths
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

MAKEFILES := Makefile $(wildcard *.mk)
$(info makefiles: $(MAKEFILES))

########################################################################################################################
# FLAGS CHANGE DETECTION
########################################################################################################################
# PREV_BUILDINFO_VALUE = $(shell if [ -f $(BUILDINFO) ]; then cat "${BUILDINFO}"; fi)
# CURRENT_BUILDINFO_VALUE=$(CC) $(CFLAGS)
# $(info curr compile command: $(CURRENT_BUILDINFO_VALUE))
# $(info prev flags: $(PREV_CFLAGS))
# $(shell \
# 	if [ "$(PREV_BUILDINFO_VALUE)" != "$(CURRENT_BUILDINFO_VALUE)" ]; then \
# 		mkdir -p `dirname $(BUILDINFO)` && echo "$(CURRENT_BUILDINFO_VALUE)" > $(BUILDINFO); \
# 	fi\
# )

########################################################################################################################
# BUILD TYPES
########################################################################################################################
ifeq ($(BUILD_TYPE),lib)
	BUILD_TARGET=$(PROJ_NAME).a
    BUILD_TARGET_CMD=ar rcs $@ $^
else
	BUILD_TARGET=$(PROJ_NAME)
    BUILD_TARGET_CMD=$(CC) $(BUILD_TARGET_FLAGS) $(CFLAGS) -L$(LDIR) -o $@ $^
endif
$(info BUILD_TARGET: $(BUILD_TARGET))

########################################################################################################################
# ALL - needs to be the first rule
########################################################################################################################
all: $(BUILD_TARGET)

########################################################################################################################
# CLEAN
########################################################################################################################
clean:
	rm -f $(PROJ_NAME) *.a $(ODIR)/*.o $(ODIR)/*.a $(DEP)/*.d $(ODIR)/*.d.* *~ core

########################################################################################################################
# PHONIES CONFIG
########################################################################################################################
.PHONY: clean all

########################################################################################################################
# MAIN
########################################################################################################################
$(BUILD_TARGET): $(OBJ) $(LIBS)
	@mkdir -p $(@D)
	$(BUILD_TARGET_CMD)

########################################################################################################################
# DEPENDENCIES
########################################################################################################################
DEP_CMD=$(CC) -M $(CFLAGS) $(IDIR)

$(DEP)/%.d: $(SRC)/%.c $(MAKEFILES)
	@mkdir -p $(@D);
	@set -e; rm -f $@;
	$(DEP_CMD) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,$(ODIR)/\1.o $@ : $(MAKEFILES) ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

########################################################################################################################
# OBJECTS
########################################################################################################################
OBJ_CMD=$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SRC)/%.c $(DEP)/%.d
	@mkdir -p $(@D);
	$(OBJ_CMD)

########################################################################################################################
# LIBS
########################################################################################################################
%.a:
	@echo "===================================================="
	@echo "BUILDING: LIB"
	@echo "===================================================="
	cd $(@D) && $(MAKE)

########################################################################################################################
# INCLUDE DEPENDENCIES TO THIS MAKEFILE
########################################################################################################################
_DEPS = $(patsubst %.c,%.d,$(_SOURCES))
DEPS = $(patsubst %,$(DEP)/%,$(_DEPS))
-include $(DEPS)

########################################################################################################################
########################################################################################################################

