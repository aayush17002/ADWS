SUBDIRS =

# Includes folders
INCLUDES_DIR = -I$(top_srcdir)/inc -I$(top_srcdir)/src/inc

# cflags: important to define that otherwise we inherit default values too
CXXFLAGS = -Wall -g -O3 -std=c++11
LDFLAGS = -lpthread

if PRODUCTION_SETTINGS
PRODUCTION_SETTINGS_FLAGS =
else
PRODUCTION_SETTINGS_FLAGS = -DHC_ASSERTION_CHECK -DHC_COMM_WORKER_STATS
endif

if PERFCOUNTER
PERFCOUNTER_FLAGS = -DPERFCOUNTER
else
PERFCOUNTER_FLAGS =
endif

# build the hclib library
lib_LTLIBRARIES = libhclib.la

# These are populated in sub-folder's Makefile.inc
noinst_LTLIBRARIES =
libhclib_la_LIBADD =

AM_CXXFLAGS = $(PRODUCTION_SETTINGS_FLAGS) $(PERFCOUNTER_FLAGS) -I$(LIBXML2_INCLUDE)
libhclib_la_SOURCES = hclib-runtime.cpp hclib-deque.cpp hclib-mm.cpp hclib-hpt.cpp hclib-thread-bind.cpp hclib-ddf.cpp hclib-timer.cpp hclib-perfcounter.cpp hclib-fpp-project.cpp

# build hclib
libhclib_la_CXXFLAGS= $(CXXFLAGS) $(INCLUDES_DIR) $(AM_CXXFLAGS)

MAINTAINERCLEANFILES = Makefile.in
