CFLAGS   += -O3
CPPFLAGS += -O3

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) -q $@ $(OBJECTS)

include $(DEPS)
