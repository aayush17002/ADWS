
SUBDIRS = src

AUX_DIST = \
	$(ac_aux_dir)/config.guess		\
	$(ac_aux_dir)/config.sub		\
	$(ac_aux_dir)/install-sh		\
	$(ac_aux_dir)/ltmain.sh			\
	$(ac_aux_dir)/missing			\
	$(ac_aux_dir)/depcomp

library_includedir=$(includedir)

library_include_HEADERS = inc/hclib-timer.h inc/hclib-rt.h inc/hclib.h inc/hclib-mak/hclib.mak inc/hclib-async.h inc/hclib-hpt.h inc/hclib-mm.h inc/hclib-forasync.h inc/hclib-ddf.h inc/hclib-asyncAwait.h src/inc/hclib-atomics.h inc/hclib-place.h inc/hclib-asyncStruct.h inc/likwid-gcc.mk

MAINTAINERCLEANFILES = Makefile.in \
	aclocal.m4 \
	configure  \
	$(AUX_DIST)
