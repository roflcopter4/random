#Compiler and Linker
CC          := gcc

#The Target Binary Program
TARGET      := prog
#SOURCES     := something.c

#The Directories, Source, Includes, Objects, Binary and Resources
INCDIR      := inc/
OBJDIR      := obj/
#SRCDIR      := src/
#BINDIR      := bin/
#RESDIR      := res/
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -O3 -g
LIB         :=
INC         := -I $(INCDIR)
INCDEP      := -I $(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

ifndef $(SOURCES)
    ifdef $(BINDIR)
        SOURCES := $(shell find $(SRCDIR)*.$(SRCEXT))
    else
        SOURCES := $(shell find *.$(SRCEXT))
    endif
endif

#OBJECTS     := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
OBJ     := $(SOURCES:.$(SRCEXT)=.$(OBJEXT))
OBJECTS := $(patsubst %,$(OBJDIR)%,$(OBJ))


#Defauilt Make
all: resources $(TARGET)

#Remake
remake: cleaner all

#Copy Resources from Resources Directory to Target Directory
resources: directories
ifdef $(RESDIR)
    ifdef $(BINDIR)
	@cp $(RESDIR)* $(BINDIR)
    else
	@cp $(RESDIR*) .
    endif
endif

#Make the Directories
directories:
ifdef $(BINDIR)
	@mkdir -p $(BINDIR)
endif
ifdef $(OBJDIR)
	@mkdir -p $(OBJDIR)
endif

#Clean only Objecst
clean:
	@$(RM) -rf $(OBJDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -f $(TARGET)
ifdef $(BINDIR)
	@$(RM) -rf $(BINDIR)
endif

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(BINDIR)$(TARGET) $^ $(LIB)

#Compile
$(OBJDIR)%.$(OBJEXT): $(SRCDIR)%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)$*.$(SRCEXT) > $(OBJDIR)$*.$(DEPEXT)
	@cp -f $(OBJDIR)$*.$(DEPEXT) $(OBJDIR)$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OBJDIR)$*.$(OBJEXT):|' < $(OBJDIR)$*.$(DEPEXT).tmp > $(OBJDIR)$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)$*.$(DEPEXT)
	@rm -f $(OBJDIR)$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources
