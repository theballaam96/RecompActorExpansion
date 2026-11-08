BUILD_DIR := build

# Allow the user to specify the compiler and linker on macOS
# as Apple Clang does not support MIPS architecture
ifeq ($(OS),Windows_NT)
    CC      := clang
    LD      := ld.lld
else ifneq ($(shell uname),Darwin)
    CC      := clang
    LD      := ld.lld
else
    CC      ?= clang
    LD      ?= ld.lld
endif

TARGET  := $(BUILD_DIR)/mod.elf

LDSCRIPT := mod.ld
CFLAGS   := -target mips -mips2 -mabi=32 -O2 -G0 -mno-abicalls -mno-odd-spreg -mno-check-zero-division \
			-fomit-frame-pointer -ffast-math -fno-unsafe-math-optimizations -fno-builtin-memset \
			-Wall -Wextra -Wno-incompatible-library-redeclaration -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable \
			-Wno-missing-braces -Wno-unsupported-floating-point-opt -Werror=section
ASFLAGS  := -target mips -mips2 -mabi=32 -G0 -mno-abicalls -mno-check-zero-division -x assembler-with-cpp -modd-spreg
CPPFLAGS := -nostdinc -D_LANGUAGE_C -DMIPS -DF3DEX_GBI -I include -I include/dummy_headers \
			-I dk64_decomp/include -I dk64_decomp/include/2.0L
LDFLAGS  := -nostdlib -T $(LDSCRIPT) -Map $(BUILD_DIR)/mod.map --unresolved-symbols=ignore-all --emit-relocs -e 0 --no-nmagic

rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(if $(wildcard $d/),$(call rwildcard,$d/,$2)))
C_SRCS := $(call rwildcard,src/,*.c)
C_OBJS := $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.o))
C_DEPS := $(addprefix $(BUILD_DIR)/, $(C_DEPS:.c=.d))

S_SRCS := $(call rwildcard,src/,*.s)
S_OBJS := $(addprefix $(BUILD_DIR)/, $(S_SRCS:.s=.o))

all: $(TARGET)

$(TARGET): $(C_OBJS) $(S_OBJS) $(LDSCRIPT) | $(BUILD_DIR)
	$(LD) $(C_OBJS) $(S_OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	mkdir $(subst /,\,$@)
else
	mkdir -p $@
endif

$(C_OBJS): $(BUILD_DIR)/%.o : %.c
ifeq ($(OS),Windows_NT)
	if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
else
	mkdir -p $(dir $@)
endif
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

$(S_OBJS): $(BUILD_DIR)/%.o : %.s
ifeq ($(OS),Windows_NT)
	if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
else
	mkdir -p $(dir $@)
endif
	$(CC) $(ASFLAGS) -I src -nostdinc -D_LANGUAGE_C -DMIPS -DF3DEX_GBI -I include -I include/dummy_headers -I dk64_decomp/include -I dk64_decomp/include/2.0L $< -c -o $@

clean:
ifeq ($(OS),Windows_NT)
	rmdir /S /Q $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif

-include $(C_DEPS)

.PHONY: clean all