CC = gcc
CFLAGS = -Wall -fstack-protector -Wextra -Wunreachable-code -O -std=c99 -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED
TARGET = codec_4061
SRCDIR = .
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
	./$(TARGET) -e ./res/pa2_4061/encodetest02 $(OUTPUTDIR)
	./$(TARGET) -e ./res/pa2_4061/encodetest03 $(OUTPUTDIR)
	./$(TARGET) -e ./res/pa2_4061/encodetest04 $(OUTPUTDIR)
	./$(TARGET) -e ./res/pa2_4061/extracredit_encode01 $(OUTPUTDIR)
	./$(TARGET) -d ./res/pa2_4061/decodetest01 $(OUTPUTDIR)
	./$(TARGET) -d ./res/pa2_4061/decodetest02 $(OUTPUTDIR)
	./$(TARGET) -d ./res/pa2_4061/decodetest03 $(OUTPUTDIR)
	./$(TARGET) -d ./res/pa2_4061/decodetest04 $(OUTPUTDIR)
	./$(TARGET) -d ./res/pa2_4061/extracredit_decode01 $(OUTPUTDIR)
	diff -rq ./output/encodetest01 ./res/pa2_4061/decodetest01
	diff -rq ./output/encodetest02 ./res/pa2_4061/decodetest02
	diff -rq ./output/encodetest03 ./res/pa2_4061/decodetest03
	diff -rq ./output/encodetest04 ./res/pa2_4061/decodetest04
	diff -rq ./output/extracredit_encode01/ ./res/pa2_4061/extracredit_decode01/
	diff -rq ./output/decodetest01 ./res/pa2_4061/encodetest01
	diff -rq ./output/decodetest02 ./res/pa2_4061/encodetest02
	diff -rq ./output/decodetest03 ./res/pa2_4061/encodetest03
	diff -rq ./output/decodetest04 ./res/pa2_4061/encodetest04
	diff -rq ./output/extracredit_decode01/ ./res/pa2_4061/extracredit_encode01/

