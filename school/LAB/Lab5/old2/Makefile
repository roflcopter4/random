# Extremely simple Makefile.

TARGET       = calc
#OBJECTS      = main.o mystrlib2_pt1.o mystrlib2_pt2.o mystrlib2_pt3.o stack.o
OBJECTS      = main.o mystrlib2_pt2.o mystrlib2_pt3.o stack.o

OBJDIR       = obj
OPTFLAGS     = -march=native -pipe
DEBUGFLAGS   = -g -O0
RELEASEFLAGS = -Ofast
WFLAGS       = -Wall -Wpedantic -fdiagnostics-color=always

###############################################################################

ifeq ($(CC),cc)
    CC := /usr/bin/gcc
endif

OBJ = $(addprefix $(OBJDIR)/, $(OBJECTS))
#ASM1 = $(OBJECTS:%.o=%.s)
#ASM = $(addprefix $(OBJDIR)/, $(ASM1))

ifeq ($(cfg),release)
    ALLFLAGS = $(OPTFLAGS) $(RELEASEFLAGS) $(WFLAGS) $(CFLAGS)
else
    ALLFLAGS = $(OPTFLAGS) $(DEBUGFLAGS) $(WFLAGS) $(CFLAGS)
endif

LD_CMD = $(CC) $(ALLFLAGS) $(LDFLAGS) -o $(TARGET) $^ 
CC_CMD = $(CC) $(ALLFLAGS) -o $@ -c $< 

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
