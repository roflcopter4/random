# Extremely simple Makefile.

#ifeq ($(CC),cc)
    #CC := /usr/lib/ccache/bin/gcc
#endif

TARGET       = tictactoe
OBJDIR       = obj
OBJECTS      = tictac.o

OPTFLAGS     = -pipe -march=native
DEBUGFLAGS   = -Og -g
RELEASEFLAGS = -O2
WFLAGS       = -Wall -Wpedantic -fdiagnostics-color=always

###############################################################################

OBJ = $(addprefix $(OBJDIR)/, $(OBJECTS))

ifeq ($(cfg),release)
    ALLFLAGS = $(OPTFLAGS) $(RELFLAGS) $(WFLAGS) $(CFLAGS)
else
    ALLFLAGS = $(OPTFLAGS) $(DEBUGFLAGS) $(WFLAGS) $(CFLAGS)
endif

LD_CMD = $(CC) $(ALLFLAGS) $(LDFLAGS) $^ -o $(TARGET)
CC_CMD = $(CC) $(ALLFLAGS) -c $< -o $@

###############################################################################


ifdef v
all: report $(OBJDIR) $(TARGET)
else
all: $(OBJDIR) $(TARGET)
endif


ifeq ($(v),all)
$(TARGET): $(OBJ)
	$(LD_CMD)

$(OBJDIR)/%.o: %.c
	$(CC_CMD)

else
$(TARGET): $(OBJ)
	@$(LD_CMD)
	@printf " LD\t%s\n" "$@"

$(OBJDIR)/%.o: %.c
	@$(CC_CMD)
	@printf " CC\t%s\n" $@
endif


$(OBJDIR):
	@mkdir -p $(OBJDIR)


###############################################################################

report:
	@echo "CC:       $(CC)"
	@echo "ALLFLAGS: $(ALLFLAGS)"
	@echo
	@$(MAKE) $(TARGET) --no-print-directory

clean:
	@rm -rf *.o $(TARGET) $(OBJDIR)

remake: clean
	@$(MAKE) --no-print-directory

.PHONY: clean remake report
