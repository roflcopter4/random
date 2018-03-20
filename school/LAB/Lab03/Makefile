# Extremely simple Makefile.

#ifeq ($(CC),cc)
    #CC := /usr/lib/ccache/bin/gcc
#endif

TARGET     = tictactoe
OBJ        = tictac.o

OPTFLAGS   = -pipe -march=native
DEBUGFLAGS = -Og -g
RELFLAGS   = -O2
WFLAGS     = -Wall -Wpedantic

ifeq ($(cfg),release)
    ALLFLAGS = $(OPTFLAGS) $(RELFLAGS) $(WFLAGS) $(CFLAGS)
else
    ALLFLAGS = $(OPTFLAGS) $(DEBUGFLAGS) $(WFLAGS) $(CFLAGS)
endif

###############################################################################

all: report

$(TARGET): $(OBJ)
	@$(CC) $(ALLFLAGS) $(LDFLAGS) $^ -o $(TARGET)
	@printf " LD\t%s\n" "$@"

%.o: %.c
	@$(CC) $(ALLFLAGS) -c $< -o $@
	@printf " CC\t%s\n" $@

###############################################################################

report:
	@echo "CC:     $(CC)"
	@echo "ALLFLAGS: $(ALLFLAGS)"
	@echo
	@$(MAKE) $(TARGET) --no-print-directory

clean:
	rm -f *.o $(TARGET)

remake: clean
	@$(MAKE) --no-print-directory

.PHONY: clean nuke remake report
