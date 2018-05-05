#Compiler and Linker
#CC          := gcc

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
OPTIMFLAGS    := -O0 -g
RELOPTIMFLAGS := -O3
CFLAGS        := -Wall -Wpedantic
#OTHER         := 
ALLCFLAGS     := $(OPTIMFLAGS) $(CFLAGS) $(FLAGS)
#LIB           := -Llib -lmyStrLib -lggets
INC           := -I $(INCDIR)
INCDEP        := -I $(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------

ifndef $(SOURCES)
    ifdef $(SRCDIR)
        SOURCES := $(shell find $(SRCDIR)*.$(SRCEXT))
    else
        SOURCES := $(shell find *.$(SRCEXT))
    endif
endif

#OBJECTS     := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
OBJ     := $(SOURCES:.$(SRCEXT)=.$(OBJEXT))
OBJECTS := $(patsubst %,$(OBJDIR)%,$(OBJ))
NUMPROC := $(shell nproc --all)


all: resources $(TARGET)

remake: cleaner
	$(MAKE) -j $(NUMPROC) all

Release:
	$(MAKE) -j $(NUMPROC) all OPTIMFLAGS=$(RELOPTIMFLAGS) OBJDIR=rel/obj/ BINDIR=rel/
	strip rel/$(TARGET)

Remake-Release: nuke Release


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
nuke: cleaner
	@$(RM) -rf rel
cleaner: clean
	@$(RM) -f $(TARGET)
ifdef $(BINDIR)
	@$(RM) -rf $(BINDIR)
endif

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) $(LINKFLAGS) -o $(BINDIR)$(TARGET) $^ $(LIB)

#Compile
$(OBJDIR)%.$(OBJEXT): $(SRCDIR)%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(ALLCFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(ALLCFLAGS) $(INCDEP) -MM $(SRCDIR)$*.$(SRCEXT) > $(OBJDIR)$*.$(DEPEXT)
	@cp -f $(OBJDIR)$*.$(DEPEXT) $(OBJDIR)$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OBJDIR)$*.$(OBJEXT):|' < $(OBJDIR)$*.$(DEPEXT).tmp > $(OBJDIR)$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)$*.$(DEPEXT)
	@rm -f $(OBJDIR)$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources

#.NOTPARALLEL: remake
