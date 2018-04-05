# Extremely simple Makefile.

TARGET      := calc
OBJECTS     := calculator.o stack.o mystrlib2.o

OPTFLAGS     = -march=native -pipe
DEBUGFLAGS   = -g -O0
RELEASEFLAGS = -Ofast
WFLAGS       = -Wall -Wpedantic -Wextra -fdiagnostics-color=always
DEFINES      = -D_GNU_SOURCE
LIBS         =

CC          ?= /usr/bin/gcc
LDFLAGS     += 
BACKUP_CC   := /usr/bin/gcc
OBJDIR      := obj

###############################################################################

COMMONFLAGS  = $(OPTFLAGS) $(WFLAGS) $(DEFINES)
OBJ          = $(addprefix $(OBJDIR)/, $(OBJECTS))

ifeq ($(CC),cc)
    CC := $(BACKUP_CC)
endif

ifeq ($(cfg),release)
    CFLAGS := $(RELEASEFLAGS) $(COMMONFLAGS)
else
    CFLAGS := $(DEBUGFLAGS) $(COMMONFLAGS)
endif

LD_CMD = $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $^ $(LIBS)
CC_CMD = $(CC) $(CFLAGS) -o $@ -c $< 

###############################################################################

all: $(OBJDIR) $(TARGET)

ifdef v
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

clean:
	@rm -rf *.o $(TARGET) $(OBJDIR)

remake: clean
	@$(MAKE) --no-print-directory

.PHONY: clean remake
