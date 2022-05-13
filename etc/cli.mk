##############################################################################
##                                 sli-tool                                 ##
##                                                                          ##
##                     Copyright (C) 2021-2022 Aquefir.                     ##
##                Released under Mozilla Public License 2.0.                ##
##############################################################################

include $(AQ)/lib/slick/base.mk

# name of project. used in output binary naming
PROJECT := sli-tool

# put a ‘1’ for the desired target types to compile
EXEFILE := 1
SOFILE  :=
AFILE   :=

# space-separated path list for #includes
# <system> includes
INCLUDES := include $(TROOT)/include/glib-2.0 $(TROOT)/lib/glib-2.0/include
# "local" includes
INCLUDEL := src

# space-separated library name list
LIBS      := glib-2.0
LIBDIRS   := $(TROOT)/lib

# ‘3P’ are in-tree 3rd-party dependencies
# 3PLIBDIR is the base directory
# 3PLIBS is the folder names in the base directory for each library
3PLIBDIR := 3rdparty
3PLIBS   :=

# frameworks (macOS target only)
FWORKS :=

# sources
CFILES    := \
	src/main.c \
	src/sli.c
CPPFILES  :=
PUBHFILES := \
	include/sli.h
PRVHFILES := \
	src/sli.h

# test suite sources
TES_CFILES    :=
TES_HFILES :=
TES_CPPFILES  :=
TES_HFILES :=

# force C++ linker because of 3rd-party library
CCLD := $(CXX)

# this defines all our usual targets
include $(AQ)/lib/slick/targets.mk
