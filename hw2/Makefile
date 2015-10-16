CC = gcc
CFLAGS = -Wall -fstack-protector -Wextra -Wunreachable-code -O -std=c99 -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED
TARGET = codec_4061
SRCDIR = src
OBJDIR = obj
OUTPUTDIR = output
SOURCES = $(wildcard $(SRCDIR)/*.c)
INCLUDES = $(wildcard $(SRCDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS)
	@echo "Building complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(SRCDIR)/%.h
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONEY: clean
clean:
	@rm -rf $(OBJDIR) $(OUTPUTDIR) $(TARGET)
	@echo "Cleanup complete!"

.PHONEY: run-test
run-test: $(TARGET)
	@rm -rf $(OUTPUTDIR)
	./$(TARGET) -e ./res/pa2_4061/encodetest01 $(OUTPUTDIR)

