CC = gcc
CFLAGS = -Wall -fstack-protector -Wextra -Wunreachable-code -O3 -std=c99
TARGET = graphexec
SRCDIR = src
OBJDIR = obj
BINDIR = bin
OUTPUTDIR = output
SOURCES = $(wildcard $(SRCDIR)/*.c)
INCLUDES = $(wildcard $(SRCDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS)
	@echo "Building complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONEY: clean
clean:
	@rm -rf $(OBJECTS) $(OUTPUTDIR)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@rm -rf $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

.PHONEY: test
test:
	@rm -rf $(OUTPUTDIR)
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph0.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph1.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph2.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph3.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph4.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testgraph5.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testCycle0.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testCycle1.txt
	$(BINDIR)/$(TARGET) res/pa1-release/testfail0.txt
