# DeviceNet Project 0.2 Makefile
# Written by Georgi D. Sotirov, gdsotirov@gmail.com
# $Id: Makefile,v 1.5 2014/10/25 09:33:19 gsotirov Exp $

DEBUG = 1 # comment this line to disable debug bilds

CC = g++
AR = ar
LN = ln
RM = rm
RANLIB = ranlib
MKDIR = mkdir
INSTALL = install
LDCONFIG = ldconfig

DEBUG_FLAGS = -ggdb
ifeq ($(DEBUG), 1)
CFLAGS = -Wall $(DEBUG_FLAGS)
else
CFLAGS = -Wall
endif
ARFLAGS = rc
LNFLAGS = -sf
RMFLAGS = -f
MKDIRFLAGS = -p
INSTALLFLAGS = -m 0755

PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib

MAJOR = 0
MINOR = 2
PATCH = 0

LIBNAME = dnetmod
ANAME = lib$(LIBNAME).a
SONAME = lib$(LIBNAME).so.$(MAJOR).$(MINOR).$(PATCH)
SOVERSION = lib$(LIBNAME).so.$(MAJOR).$(MINOR)
TESTNAME = dnmtest

OBJS = cid.o cnode.o cintf.o cdevice.o ccifintf.o ccifdevice.o dnetmod.o
OBJSDLL = $(OBJS:.o=.pic.o)
CIFDIR = ../lib/cif3.000
CIFINC = $(CIFDIR)/usr-inc
CIFHDRS = $(CIFINC)/cif_user.h $(CIFINC)/rcs_user.h $(CIFINC)/dnm_user.h
CIFAPI = $(CIFDIR)/api/cif_api.o

STATIC_COMPILE_CMD = $(CC) $(CFLAGS) -I$(CIFINC) -o $@ -c $<
SHARED_COMPILE_CMD = $(CC) $(CFLAGS) -I$(CIFINC) -fPIC -o $@ -c $<

# Build static and shared libraries and test programm
all: static shared test

# Build static library file
$(ANAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS) $(CIFAPI)
	$(RANLIB) $@

# Build shared library file and make links
$(SONAME): $(OBJSDLL)
	$(CC) -shared $(OBJSDLL) $(CIFAPI) -Wl,-soname -Wl,$(SOVERSION) -o $@
	$(LN) $(LNFLAGS) $(SONAME) $(SOVERSION)
	$(LN) $(LNFLAGS) $(SONAME) lib$(LIBNAME).so

$(TESTNAME): shared $(TESTNAME).o
	$(CC) $(DEBUG_FLAGS) $(TESTNAME).o -L. -l$(LIBNAME) -o $(TESTNAME)

cid.o: cid.cpp dnmdefs.h cid.h
	$(STATIC_COMPILE_CMD)

cnode.o: cnode.cpp dnmdefs.h cid.h cnode.h
	$(STATIC_COMPILE_CMD)

cintf.o: cintf.cpp dnmdefs.h cid.h cnode.h cintf.h
	$(STATIC_COMPILE_CMD)

cdevice.o: cdevice.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h cdevice.h
	$(STATIC_COMPILE_CMD)

ccifintf.o: ccifintf.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h $(CIFHDRS) ccifintf.h
	$(STATIC_COMPILE_CMD)

ccifdevice.o: ccifdevice.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h ccifintf.h $(CIFHDRS) ccifdevice.h ccifdevice.cpp
	$(STATIC_COMPILE_CMD)

dnetmod.o: dnetmod.cpp dnmdefs.h dnmerrs.h dnmsd.h cid.h cnode.h cintf.h cdevice.h ccifintf.h ccifdevice.h $(CIFHDRS) dnetmod.h
	$(STATIC_COMPILE_CMD)

cid.pic.o: cid.cpp dnmdefs.h cid.h
	$(SHARED_COMPILE_CMD)

cnode.pic.o: cnode.cpp dnmdefs.h cid.h cnode.h
	$(SHARED_COMPILE_CMD)

cintf.pic.o: cintf.cpp dnmdefs.h cid.h cnode.h cintf.h
	$(SHARED_COMPILE_CMD)

cdevice.pic.o: cdevice.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h cdevice.h
	$(SHARED_COMPILE_CMD)

ccifintf.pic.o: ccifintf.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h $(CIFHDRS) ccifintf.h
	$(SHARED_COMPILE_CMD)

ccifdevice.pic.o: ccifdevice.cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h ccifintf.h $(CIFHDRS) ccifdevice.h ccifdevice.cpp
	$(SHARED_COMPILE_CMD)

dnetmod.pic.o: dnetmod.cpp dnmdefs.h dnmerrs.h dnmsd.h cid.h cnode.h cintf.h cdevice.h ccifintf.h ccifdevice.h $(CIFHDRS) dnetmod.h
	$(SHARED_COMPILE_CMD)

$(TESTNAME).o: $(TESTNAME).cpp dnmdefs.h dnmerrs.h cid.h cnode.h cintf.h cdevice.h ccifintf.h ccifdevice.h
	$(CC) $(CFLAGS) -o $@ -c $<

# Build static library
static: $(ANAME)

# Build shared library
shared: $(SONAME)

# Build test programm
test: $(TESTNAME)

# Install static and shared libraries
install: all
	$(MKDIR) $(MKDIRFLAGS) $(LIBDIR)
	$(INSTALL) $(INSTALLFLAGS) $(ANAME) $(LIBDIR)
	$(INSTALL) $(INSTALLFLAGS) $(SONAME) $(LIBDIR)
	$(LN) $(LNFLAGS) $(LIBDIR)/$(SONAME) $(LIBDIR)/$(SOVERSION)
	$(LN) $(LNFLAGS) $(LIBDIR)/$(SONAME) $(LIBDIR)/lib$(LIBNAME).so
	$(LDCONFIG)

# Uninstall static and shared libraries
uninstall:
	$(RM) $(RMFLAGS) $(LIBDIR)/lib$(LIBNAME).so
	$(RM) $(RMFLAGS) $(LIBDIR)/$(SOVERSION)
	$(RM) $(RMFLAGS) $(LIBDIR)/$(SONAME)
	$(RM) $(RMFLAGS) $(LIBDIR)/$(ANAME)
	$(LDCONFIG)

# Clean objects and intermediate files
clean:
	$(RM) $(RMFLAGS) $(OBJS) $(OBJSDLL) $(TESTNAME).o

# Clean objects, intermediate files and binaries
distclean: clean
	$(RM) $(RMFLAGS) $(ANAME) $(SONAME) $(SOVERSION) lib$(LIBNAME).so
	$(RM) $(RMFLAGS) $(TESTNAME)

