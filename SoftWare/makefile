# Makefile3 in SoftWare directory

# Define the subdirectories containing Makefiles
SUBDIRS := STM32G431CBUx STM32H723_GCC STM32G4_MCBV06 STM32G431_GCC

# The default target is 'all'
all: $(SUBDIRS)
# Define the make command
MAKE_CMD := make

# Define a rule for each subdirectory
$(SUBDIRS):
	@echo "Building in $@"
	"D:/Program Files (x86)/GnuWin32/bin/$(MAKE_CMD)" -C $@
# Clean target, defined to clean both subdirectories
clean:
	@for dir in $(SUBDIRS); do \
		"D:/Program Files (x86)/GnuWin32/bin/$(MAKE_CMD)" -C $$dir clean; \
	done
.PHONY: all $(SUBDIRS) clean