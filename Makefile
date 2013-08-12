export TARGET        = libnrgraph.a
export CC            = g++
export AR            = ar
export COMMON_FLAGS  = -Wall -Werror
export COMMON_FLAGS  = -I/usr/include/cairo 
export COMMON_FLAGS += -I/usr/include/glib-2.0 
export COMMON_FLAGS += -I/usr/lib/i386-linux-gnu/glib-2.0/include
export COMMON_FLAGS += -I/usr/include/pixman-1
export COMMON_FLAGS += -I/usr/include/freetype2
export COMMON_FLAGS += -I/usr/include/libpng12 
export CFLAGS        = $(COMMON_FLAGS)
export CXXFLAGS      = $(COMMON_FLAGS) -std=c++11
export LDFLAGS       = -lcairo
export SOURCES      := $(wildcard $(CURDIR)/sources/*.cpp)
export OBJECTS      := $(notdir $(patsubst %.cpp,%.o,$(SOURCES)))
export VPATH        := $(CURDIR)/sources
export DEPS         := $(CURDIR)/Makefile.depends

.PHONY: all clean Debug depends realclean Release tags

all: Debug Release

Debug Release:
	@mkdir -p $@
	$(MAKE) --no-print-directory -C $@ -f ../$@.mk $(TARGET)

tags:
	$(CC) -M $(INCLUDE_DIRECTORIES) $(SOURCES) \
		| sed -e 's/[\\ ]/\n/g' \
		| sed -e '/^$$/d' -e '/\.o:[ \t]*$$/d' \
		| ctags -L - --c++-kinds=+p --fields=+iaS --extra=+q

depends: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDE_DIRECTORIES) -MM $(SOURCES) > $(DEPS)

realclean: clean
	rm -fr log
	rm -fr tags
	rm -fr Debug
	rm -fr Release

clean:
	rm -fr *~
	rm -fr $(DEPS)
