# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  Assignment1_Part2_config = debug
endif
ifeq ($(config),release)
  Assignment1_Part2_config = release
endif

PROJECTS := Assignment1_Part2

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

Assignment1_Part2:
ifneq (,$(Assignment1_Part2_config))
	@echo "==== Building Assignment1_Part2 ($(Assignment1_Part2_config)) ===="
	@${MAKE} --no-print-directory -C . -f Assignment1_Part2.make config=$(Assignment1_Part2_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f Assignment1_Part2.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Assignment1_Part2"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"