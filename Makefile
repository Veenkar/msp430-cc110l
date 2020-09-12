########################################################################################################################
# ACC_RCV MAIN MAKEFILE
########################################################################################################################
MAKEFILE_NAME=$(firstword $(MAKEFILE_LIST))

LIBS_DIRS=$(wildcard *_lib)
LIBS=$(foreach D,$(LIBS_DIRS),$D/$D.a)
$(info LIBS_DIRS: $(LIBS_DIRS))
$(info LIBS: $(LIBS))

all: libs info

info:
	@echo ....:::: $(MAKEFILE_NAME) ::::....

clean: info
	cd acc_lib && $(MAKE) clean

cleanx:
	cd acc_lib && $(MAKE) clean

libs: $(LIBS)

%.a:
	cd $(@D) && $(MAKE)



.PHONY: all clean subsystem info cleanx

########################################################################################################################
########################################################################################################################
