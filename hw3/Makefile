CC = gcc
CFLAGS = -Wall -fstack-protector -Wextra -Wunreachable-code -O -std=c99 -D_XOPEN_SOURCE=700 -D_XOPEN_SOURCE_EXTENDED -lpthread
TARGET = twitterTrend
SRCDIR = .
OBJDIR = obj
OUTPUTDIR = ./res/4061_PA03_to_be_released/
SOURCES = $(wildcard $(SRCDIR)/*.c)
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
	@rm -rf $(OBJDIR) $(TARGET)
	@rm -rf $(wildcard $(OUTPUTDIR)/*.result)
	@echo "Cleanup complete!"

.PHONEY: run-test
run-test: $(TARGET)
	./twitterTrend ./res/4061_PA03_to_be_released/client1.in 1
	./twitterTrend ./res/4061_PA03_to_be_released/client2.in 1
	./twitterTrend ./res/4061_PA03_to_be_released/client2.in 2
	./twitterTrend ./res/4061_PA03_to_be_released/client2.in 3
	./twitterTrend ./res/4061_PA03_to_be_released/client3.in 2
	./twitterTrend ./res/4061_PA03_to_be_released/client4.in 3
