# Makefile3 in SoftWare directory  
  
# Define the subdirectories containing Makefiles  
SUBDIRS := STM32G431_GCC   STM32F405RG_GCC
  
# The default target is 'all'  
all: $(SUBDIRS)  

# Define a rule for each subdirectory  
# '@' prevents echoing the command itself  
$(SUBDIRS):  
	@echo "Building in $@"  
	$(MAKE) -C $@  

# Clean target, defined to clean both subdirectories  
clean:  
	@for dir in $(SUBDIRS); do \  
	$(MAKE) -C 
	$$
	dir clean; \  
		done  
.PHONY: all $(SUBDIRS) clean