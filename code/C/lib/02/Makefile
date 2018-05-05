ifeq ($(CC),cc)
    CC := /usr/lib/ccache/bin/gcc
endif
CFLAGS = -Og -pipe -march=native -Wall -Wpedantic
TARGET = assignment1pt2
OBJ    = myStrLib.o main.o


all: report


$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)
	@printf " LD\t%s\n" "$^"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf " CC\t%s\n" $@


report:
	@echo "CC:     $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo
	@$(MAKE) $(TARGET) --no-print-directory

clean:
	@rm -f *.o

nuke:
	@rm -f *.o $(TARGET)

remake: nuke
	@$(MAKE) --no-print-directory

.PHONY: clean nuke remake report
