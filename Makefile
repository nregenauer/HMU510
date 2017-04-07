# library name
lib.name = submission

# input source file (class name == source file basename)
class.sources = submission.c
# all extra files to be included in binary distribution of the library


# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
