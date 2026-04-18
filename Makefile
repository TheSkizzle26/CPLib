# modified makefile originally from SnailMath's hollyhock-2 example makefile.

APP_NAME := cplib_test

FIXPOINT_DEFS := -DFIXMATH_NO_CACHE -DFIXMATH_NO_CTYPE -DFIXMATH_NO_HARD_DIVISION -DFIXMATH_NO_64BIT # -DFIXMATH_FAST_SIN (excluded to not make 3d camera jitter)

C_VERSION := c23
OPTIMIZATION_LEVEL := O3 # don't go lower than O1 (compilation errors!)

# enable some features here (for example -DCPLIB_ENABLE_MATRIX4)
CPLIB_FEATURES := -DCPLIB_ENABLE_3D -DCPLIB_ENABLE_FONT -DCPLIB_ENABLE_FILE

# maximum number of threads
# MAKEFLAGS := --jobs=$(shell nproc)

C_PC := gcc
C_PC_FLAGS := -W -Wall -DTARGET_PC -lraylib -std=$(C_VERSION) -$(OPTIMIZATION_LEVEL) $(FIXPOINT_DEFS) $(CPLIB_FEATURES)

#The sh4 assembler, compiler and linker:
AS := sh4-elf-as
AS_FLAGS :=

CC := sh4-elf-gcc
CC_FLAGS := -ffreestanding -fshort-wchar -Wall -Wextra -fno-builtin-memcpy -std=$(C_VERSION) -$(OPTIMIZATION_LEVEL) $(FIXPOINT_DEFS) $(CPLIB_FEATURES)

CXX := sh4-elf-g++
CXX_FLAGS := -ffreestanding -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -fno-builtin-memcpy -std=$(C_VERSION) -$(OPTIMIZATION_LEVEL) $(FIXPOINT_DEFS) $(CPLIB_FEATURES)

LD := sh4-elf-gcc
LD_FLAGS := -nostartfiles -nostdlib

READELF := sh4-elf-readelf
OBJCOPY := sh4-elf-objcopy

AS_SOURCES := $(shell find $(SOURCEDIR) -name '*.s')
CC_SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
CXX_SOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')
H_INC := $(shell find $(SOURCEDIR) -name '*.h')
HPP_INC := $(shell find $(SOURCEDIR) -name '*.hpp')
OBJECTS := $(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

APP_PC := _$(APP_NAME).elf

APP_ELF := $(APP_NAME).hhk

all: pc hhk

pc: $(APP_PC) Makefile

hhk: $(APP_ELF) Makefile

clean:
	rm -f $(OBJECTS) $(APP_ELF) $(APP_PC)

$(APP_PC):  $(CC_SOURCES) $(CXX_SOURCES) $(H_INC) $(HPP_INC)
	$(C_PC) $(CC_SOURCES) $(CXX_SOURCES) -o $(APP_PC) $(C_PC_FLAGS)

# used to be $(SDK_DIR)/sdk.o before linker.ld and after objects
$(APP_ELF): $(OBJECTS) linker.ld
	$(LD) -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS)
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(APP_ELF) $(APP_ELF)

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
# $(SDK_DIR)/sdk.o:
# 	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md.md in the SDK directory for more information)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.c $(H_INC)
	$(CC) -c $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
%.o: %.cpp $(HPP_INC)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

#tell make that 'all' 'clean' 'hhk' and 'pc' are not actually files.
.PHONY: all clean hhk pc
