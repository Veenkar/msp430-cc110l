C# PATHS
ODIR=obj
LDIR=lib
# TODO MULTIPLE IDIRS
IDIR=acc_lib/include
SRC=src
DEP=dep

# FILES
BUILDINFO=$(ODIR)/build.info

# CONFIG
PROJ_NAME=acc_rcv
BUILD_TYPE=app

#LIBS=-lm
LIBS=acc_lib/acc_lib.a


########################################################################################################################
# COMMON SETTINGS
########################################################################################################################
include CommonSettings.mk

########################################################################################################################
# MAKEMSP430
########################################################################################################################
include MakeMsp430.mk

########################################################################################################################
########################################################################################################################
