# This makefile can build and install the magmadnn library

# incluse any user-defined compile options (pre-sets)
include make.inc

# compilers for c++ and cu
CXX ?= g++
NVCC ?= nvcc

# locations of cuda and magma installations
CUDADIR ?= /usr/local/cuda
MAGMADIR ?= /usr/local/magma
BLASDIR ?= /usr/local/openblas
BLASLIB ?= openblas

# where to install magmadnn (make must have sudo access if prefix is root privileged)
prefix ?= /usr/local/magmadnn

# headers needed for library compilation
INC := -I./include -I$(BLASDIR)/include

# libs to link with
LIBDIRS := -L$(BLASDIR)/lib
LIBS = -l$(BLASLIB)

# use nvcc to determine if we should compile for gpu or not
USE_CUDA = 0
GPU_TARGET ?= Kepler
ifneq ($(shell which nvcc),)
include make.device
CUDA_MACRO = -D_HAS_CUDA_
INC += -I$(CUDADIR)/include -I$(MAGMADIR)/include
LIBDIRS += -L$(CUDADIR)/lib64 -L$(MAGMADIR)/lib
LIBS += -lcudart -lmagma
USE_CUDA=1
endif


# individual flags for compilation
OPTIMIZATION_LEVEL ?= -O3
WARNINGS ?= -Wall
FPIC ?= -fPIC
CXX_VERSION ?= -std=c++11
DEBUG ?= 0

# set optimization to Og for debugging
ifeq ($(DEBUG),1)
OPTIMIZATION_LEVEL = -O0
endif

# the entire flags for compilation
CXXFLAGS := $(OPTIMIZATION_LEVEL) $(WARNINGS) $(CXX_VERSION) $(CUDA_MACRO) $(FPIC) -MMD
NVCCFLAGS := $(CXX_VERSION) $(OPTIMIZATION_LEVEL) -Xcompiler "$(CXXFLAGS)" $(NV_SM) $(NV_COMP)
LD_FLAGS := $(LIBDIRS) $(LIBS)

# include -g for debugging
ifeq ($(DEBUG),1)
CXXFLAGS += -g -DDEBUG
NVCCFLAGS += -g -DDEBUG
endif

# make these available to child makefiles
export CXX
export NVCC
export INC
export LD_FLAGS
export prefix
export CUDADIR
export MAGMADIR
export OPTIMIZATION_LEVEL
export WARNINGS
export CXX_VERSION
export CUDA_MACRO
export USE_CUDA
export CXXFLAGS
export NVCCFLAGS


# default extension for object files
o_ext ?= o

# where are the source files (files that need to be compiled) found
TARGET_DIRS ?= src

all: $(TARGET_DIRS)

# step into source directories and use their makefiles
$(TARGET_DIRS):
	@echo "==== Building Sources ===="
ifeq ($(USE_CUDA),1)
	@echo "CUDA installation found. Building GPU/CPU."
else
	@echo "(X) CUDA installation not found. Building CPU-only."
endif
	$(MAKE) -C $@
	@echo

# collect all the object files from the source directories (deepest: src/compute/*/*.cpp)
OBJ_FILES = $(wildcard $(TARGET_DIRS)/*.$(o_ext) $(TARGET_DIRS)/*/*.$(o_ext) $(TARGET_DIRS)/*/*/*.$(o_ext))
# collect dependency files (.d)
DEP_FILES = $(wildcard $(TARGET_DIRS)/*.d $(TARGET_DIRS)/*/*.d $(TARGET_DIRS)/*/*/*.d)


# MAKE THE LIB FILES

LIB_DIR ?= lib

# archiver and flags
ARCH ?= ar
ARCH_FLAGS ?= cr
RANLIB ?= ranlib
RANLIB_FLAGS ?= 

# set EXT to .dylib and FLAG to -dynamiclib for MAC OS
LIBSHARED_EXT ?= .so
LIBSHARED_FLAG ?= -shared

libstatic := $(LIB_DIR)/libmagmadnn.a
libshared := $(LIB_DIR)/libmagmadnn$(LIBSHARED_EXT)

lib: $(TARGET_DIRS) static shared
static: $(libstatic)
shared: $(libshared)

$(libstatic): 
	@echo "==== building static lib ===="
	mkdir -p $(LIB_DIR)
	$(ARCH) $(ARCH_FLAGS) $@ $(OBJ_FILES)
	$(RANLIB) $(RANLIB_FLAGS) $@
	@echo 

$(libshared):
	@echo "==== building shared lib ===="
	mkdir -p $(LIB_DIR)
	$(CXX) $(LIBSHARED_FLAG) $(FPIC) -o $@ $(OBJ_FILES) -L./lib $(LD_FLAGS)
	@echo 


# make the testers
TESTING_DIR ?= testing
testing:
	@echo "==== building testing sources ===="
	# step into the testing directories and call their makefiles
	$(MAKE) -C $(TESTING_DIR)
	@echo

# make the examples
EXAMPLE_DIR ?= examples
examples:
	@echo "==== building examples ===="
	# step into example directory and use its makefile
	$(MAKE) -C $(EXAMPLE_DIR)
	@echo


# build the library first, then link the lib together.
# install copies the newly made libs into prefix
install: lib
	@echo "==== installing libs ===="
	mkdir -p $(prefix)
	mkdir -p $(prefix)/include
	mkdir -p $(prefix)/lib
	cp -r ./include/* $(prefix)/include
	cp $(libstatic) $(prefix)/lib
	cp $(libshared) $(prefix)/lib
	@echo


# build the docs files and the refman.pdf
DOCS_DIR ?= docs
docs:
ifneq ($(shell which doxygen),)
	doxygen doxygen.config
	$(MAKE) -C $(DOCS_DIR)/latex
endif


# TODO: change to call clean on subdirectory makefiles
clean:
	rm $(OBJ_FILES) $(DEP_FILES)


.PHONY: $(TARGET_DIRS) $(libstatic) $(libshared) $(TESTING_DIR) $(EXAMPLE_DIR) $(DOCS_DIR)


