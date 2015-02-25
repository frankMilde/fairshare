# default value
BUILD ?=*

# directories# {{{
WORKDIR     := $(shell pwd)
SRCS_DIR    := src
LIBS_DIR    := lib
INCLUDE_DIR := include
OBJS_DIR    := obj_$(BUILD)
#BIN_DIR    := ../../bin
BIN_DIR     := bin
# }}}

# source files# {{{
SRCS_FILES +=  global-constants.cc
SRCS_FILES +=  helper-functions.cc
SRCS_FILES +=  fairshare.cc

SRCS   = $(SRCS_FILES:%.cc=$(SRCS_DIR)/%.cc)

# include files
INCLUDE += -I$(INCLUDE_DIR)/
#}}}

# ctags# {{{
#CTALL = $(INCLUDE_DIR)/*\
				$(SRCS_DIR)/*

#CTALL =  $(SRCS_DIR)/*
CTALL =  $(SRCS)
CTAGS = ctags
# }}}

# Shell commands# {{{
#SHELL  = /bin/bash
CP      := /bin/cp
RM      := /bin/rm -f
RMDIR   := /bin/rm -r
MKDIR   := /bin/mkdir -p
ECHO    := @/bin/echo
STRIP   := /usr/bin/strip
SYMLINK := /bin/ln -s
# }}}

# object files
OBJS   = $(SRCS_FILES:%.cc=$(OBJS_DIR)/%.o)

# dependency files
DEPS   = $(OBJS:.o=.d)

# target
PROG_NAME = fairshare
TARGET = $(PROGNAME)_$(BUILD)

# compiler
CXX = g++

# Libraries# {{{
# add these lines to .bash_rc
#export CPP_INCLUDE_PATH=$HOME/bin/boost_1_51_0/include
#export LD_LIBRARY_PATH=$HOME/bin/boost_1_51_0/lib:$LDLIBRARY_PATH

#
# for warnings comping out of boost libs: 
# Use -isystem instead of -I to add Boost headers to include path.
# This option means to treat headers found there as system headers,
# and suppress warnings originating there.

INCLUDE_BOOST += -isystem/usr/include/boost
LDLIBS_BOOST  += -L/usr/lib

#LDLIBS += -lm
#LDLIBS += -lboost_filesystem
#LDLIBS += -lboost_timer
#LDLIBS += -lboost_system
LDLIBS += -lboost_program_options
#LDLIBS += -lboost_chrono
#LDLIBS += -lrt
# }}}

# compiler options# {{{
# show all warnings # {{{
CXXFLAGS_WARNING +=  -W
CXXFLAGS_WARNING +=  -Wall
CXXFLAGS_WARNING +=  -Wextra
CXXFLAGS_WARNING +=  -Wcast-align
CXXFLAGS_WARNING +=  -Wcast-qual
CXXFLAGS_WARNING +=  -Wconversion
CXXFLAGS_WARNING +=  -Wdisabled-optimization
CXXFLAGS_WARNING +=  -Wfloat-equal
CXXFLAGS_WARNING +=  -Wformat-nonliteral
CXXFLAGS_WARNING +=  -Wformat-security
CXXFLAGS_WARNING +=  -Wformat-y2k
CXXFLAGS_WARNING +=  -Winit-self
CXXFLAGS_WARNING +=  -Winline
CXXFLAGS_WARNING +=  -Winvalid-pch
CXXFLAGS_WARNING +=  -Wlogical-op
CXXFLAGS_WARNING +=  -Wunsafe-loop-optimizations
CXXFLAGS_WARNING +=  -Wmissing-declarations
CXXFLAGS_WARNING +=  -Wmissing-include-dirs
CXXFLAGS_WARNING +=  -Wmissing-noreturn
CXXFLAGS_WARNING +=  -Wnon-virtual-dtor
CXXFLAGS_WARNING +=  -Wpacked
#CXXFLAGS_WARNING +=  -Wpadded
CXXFLAGS_WARNING +=  -Wparentheses
CXXFLAGS_WARNING +=  -pedantic-errors
CXXFLAGS_WARNING +=  -Wpointer-arith
CXXFLAGS_WARNING +=  -Wredundant-decls
CXXFLAGS_WARNING +=  -Wswitch-default
CXXFLAGS_WARNING +=  -Wswitch-enum
#CXXFLAGS_WARNING +=  -Wsystem-headers
CXXFLAGS_WARNING +=  -Wunreachable-code
CXXFLAGS_WARNING +=  -Wunsafe-loop-optimizations
CXXFLAGS_WARNING +=  -Wunused
CXXFLAGS_WARNING +=  -Wvariadic-macros
CXXFLAGS_WARNING +=  -Wvolatile-register-var
CXXFLAGS_WARNING +=  -Wwrite-strings
 
# do not use that option together with BOOST as this options makes objects with internal pointers a nightmare.
#OPT_WARN += -Weffc++

# treat warnings as errors
#CXXFLAGS_ERROR += -Werror
# }}}

# use for debuggin# {{{
CXXFLAGS_debug +=  -g3
CXXFLAGS_debug +=  -O0
CXXFLAGS_debug +=  -ggdb3
CXXFLAGS_debug +=  -gdwarf-2
CXXFLAGS_debug +=  -feliminate-dwarf2-dups

#OPT_DBUG +=  -ffloat-store
#http://stackoverflow.com/questions/7517588/is-this-an-g-optimization-bug
# }}}

# use for optimization, NEVER together with debug mode# {{{
# http://gcc.gnu.org/onlinedocs/gcc-3.4.4/gcc/Optimize-Options.html

# Optimize for speed# {{{
OPT_SPEED +=  -O2 # no inlining
#OPT_OPT +=  -O3 # inlining of function
OPT_SPEED += -DNDEBUG
OPT_SPEED += -fsignaling-nans
OPT_SPEED += -finline-functions
OPT_SPEED += -fipa-matrix-reorg
OPT_SPEED += -msse2
OPT_SPEED += -mfpmath=sse
OPT_SPEED += -fopenmp
#looses precisicion on float operations. http://stackoverflow.com/questions/7420665/what-does-gccs-ffast-math-actually-do
OPT_SPEED += -ffast-math 
OPT_SPEED += -fassociative-math
# use vectorizer always together with fmath options from above.
OPT_SPEED += -ftree-vectorize 
OPT_SPEED += -ftree-vectorizer-verbose=2
#===============================================================================================================
# compiler that places a  random canary between any stack allocated character buffers and the return
# pointer [5]. It then validates that the canary has not been dirtied by an
# overflowed buffer before the function returns. ProPolice can also reorder local
# variables to protect local pointers from being overwritten in a buffer overflow.
#===============================================================================================================
#OPT_SPEED += -fstack-protector-all -Wstack-protector \
# }}}

# Optimize for size# {{{
OPT_OPTSIZE += -Os
#std::vector still has code that throws exceptions and may use rtti. To suppress that use:
OPT_OPTSIZE += -fno-rtti 
OPT_OPTSIZE += -fno-exceptions 
# see also http://stackoverflow.com/questions/1512972/what-is-the-optimization-level-g-you-use-while-compairing-two-different-al
OPT_OPTSIZE += -fomit-frame-pointer
OPT_OPTSIZE += -Os
# }}}

CXXFLAGS_release += $(OPT_SPEED)
#CXXFLAGS_release += $(OPT_SIZE)
#}}}

# use as default# {{{
CXXFLAGS_DEFAULT += -std=c++0x
#CXXFLAGS_DEFAULT += -march=native # Optimize for this architecture. If you want the application to run quickly on any architecture (our condor cluster), don't specify that option.
#CXXFLAGS_DEFAULT += -mtune=native
CXXFLAGS_DEFAULT += -fshort-enums # Allocate to an enum type only as many bytes as it needs for the declared range of possible values. Specifically, the enum type will be equivalent to the smallest integer type which has enough room. 
# }}}

# }}}

# compiler flags from options# {{{
CXXFLAGS += $(INCLUDE)
CXXFLAGS += $(INCLUDE_BOOST)
CXXFLAGS += $(INCLUDE_GSL)

CXXFLAGS += $(CXXFLAGS_DEFAULT)
CXXFLAGS += $(CXXFLAGS_ERROR)
CXXFLAGS += $(CXXFLAGS_WARNING)
CXXFLAGS += $(CXXFLAGS_$(BUILD))

# flags to create dependency files
DEPFLAGS += -MMD
DEPFLAGS += -MF

# linker flags 
LDFLAGS += -static
# }}}

# These are only Makefile targets and do not refer to files
# with the same name
.PHONY : clean veryclean all debug release target ctags

###########################################
# RULES
###########################################
# {{{
#all: debug

all: release

debug:
	$(MAKE) BUILD=debug target ctags symlink

release:
	$(MAKE) BUILD=release target strip symlink

-include $(DEPS)

#ifneq ($(MAKECMDGOALS),clean)
#-include $(DEPS)
#endif

strip: 
	$(ECHO) 
	$(ECHO) "Stripping executable..."
	$(STRIP) $(BIN_DIR)/$(TARGET)

target: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(ECHO) 
	$(ECHO) Linking $^ ...
	$(MKDIR) $(BIN_DIR)
	$(CXX)  $(LDFLAGS) -o $@  $^ $(LDLIBS_BOOST) $(LDLIBS)

# compile and create dependency files
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cc
	$(MKDIR) $(OBJS_DIR)
	$(ECHO) 
	$(ECHO) Compiling $< and create dependency for $(patsubst %.o,%.d,$@)...
	LC_ALL=en_US $(CXX) $(CXXFLAGS) -o $@ -c $< $(DEPFLAGS)  $(patsubst %.o,%.d,$@)
#$(CXX) $(CXXFLAGS) $< -c -o $@ 


# Rules for generating the tags.
ctags: $(CTALL)
	$(ECHO) 
	$(ECHO) "Creating ctags..."
	$(CTAGS) $(CTALL)

# Create symlink to binary in root project folder
symlink: $(PROG_NAME)

$(PROG_NAME):
	$(ECHO) 
	$(ECHO) "Creating symlink..."
	$(RM) $(PROG_NAME)
	$(SYMLINK) $(BIN_DIR)/$(TARGET) $(PROG_NAME)

clean:
	$(ECHO) 
	$(ECHO) "Cleaning ..."
	$(RM) $(PROG_NAME)
	$(RM) $(BIN_DIR)/$(TARGET)

veryclean:
	$(ECHO) 
	$(ECHO) "Cleaning all ..."
	$(RM) $(OBJS) $(DEPS) $(BIN_DIR)/$(TARGET) 
	$(RMDIR) $(OBJS_DIR)
	$(RMDIR) $(BIN_DIR)
	$(RM) tags
	$(RM) $(PROG_NAME)

show:
	$(ECHO) 
	$(ECHO) target.......$(TARGET)
	$(ECHO) source files.$(SRCS)
	$(ECHO) source dir...$(SRCS_DIR)
	$(ECHO) obj files....$(OBJS)
	$(ECHO) obj dir......$(OBJS_DIR)
	$(ECHO) dep files....$(DEPS)
# }}}
