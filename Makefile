########################################################################################################################
# CONFIG
########################################################################################################################
# PATHS
ODIR=obj
LDIR=lib
IDIR=include
SRC=src
DEP=dep

# FILES
BUILDINFO=$(ODIR)/build.info

# CONFIG
PROJ_NAME=acc_rcv
CC=msp430-gcc
VARIANT=msp430g2553
DEBUG_FLAGS=-mmcu=msp430g2553 -O0 -g3 -ggdb -gdwarf-2
WARN_FLAGS=-Wall -Wextra -Wshadow
CFLAGS=-I$(IDIR) $(DEBUG_FLAGS) $(WARN_FLAGS) -mmcu=$(VARIANT) --vv -std=gnu90 -MMD
#LIBS=-lm
LIBS=


########################################################################################################################
# MAKEMSP430
########################################################################################################################
include MakeMsp430.mk
