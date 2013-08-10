CFLAGS   += -O0 -g
CPPFLAGS += -O0 -g

$(TARGET): $(OBJECTS)
	$(AR) -q $@ $(OBJECTS)

include $(DEPS)
