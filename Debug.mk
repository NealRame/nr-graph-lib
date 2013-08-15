CFLAGS   += -O0 -g
CXXFLAGS += -O0 -g

$(TARGET): $(OBJECTS)
	$(AR) -q $@ $(OBJECTS)

include $(DEPS)
