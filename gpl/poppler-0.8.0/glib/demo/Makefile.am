INCLUDES = 					\
	-I$(top_srcdir)/glib			\
	-I$(top_builddir)/glib			\
	$(GTK_TEST_CFLAGS)

if BUILD_GTK_TEST
noinst_PROGRAMS = poppler-glib-demo
endif

poppler_glib_demo_SOURCES = 			\
	main.c					\
	annots.h				\
	annots.c				\
	fonts.h					\
	fonts.c					\
	forms.h					\
	forms.c					\
	images.h				\
	images.c				\
	info.h					\
	info.cc					\
	links.h					\
	links.c					\
	outline.h				\
	outline.c				\
	page.h					\
	page.c					\
	render.h				\
	render.c				\
	transitions.h				\
	transitions.c				\
	utils.h					\
	utils.c

LDADD =						\
	$(top_builddir)/glib/libpoppler-glib.la	\
	$(top_builddir)/poppler/libpoppler.la	\
	$(GTK_TEST_LIBS)
