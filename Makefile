
#name of the target
TARGET = projet
# Build path
BUILD_DIR = build


#----------------------------------------------------------------------
# TOOL DEFINITIONS
#----------------------------------------------------------------------
PREFIX=arm-none-eabi-#arm-atollic-eabi-
AS      = $(PREFIX)gcc -x assembler-with-cpp
CC      = $(PREFIX)gcc
CPP     = $(PREFIX)g++
LD      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE 	= $(PREFIX)size

#----------------------------------------------------------------------
# sources files
#----------------------------------------------------------------------
# C files
ROOT_DIR = .
SRC_DIR = ./Src
CSRCS = $(SRC_DIR)/main.c $(SRC_DIR)/syscalls.c $(SRC_DIR)/sysmem.c 
CSRCS += $(SRC_DIR)/system_stm32f4xx.c 

# asm files (startup)
ASRCS = $(ROOT_DIR)/Startup/startup_stm32f401xe.s

#----------------------------------------------------------------------
# headers files
#----------------------------------------------------------------------
#PATH to STM32Cube library (here integrated in the project for headers)
STM_LIBRARY = $(ROOT_DIR)/Drivers/CMSIS

INCLUDE_PATHS = -I$(ROOT_DIR) -I$(ROOT_DIR)/Inc 

#CMSIs library 
INCLUDE_PATHS += -I$(STM_LIBRARY)/Include \
					-I$(STM_LIBRARY)/Device/ST/STM32F4xx/Include

#----------------------------------------------------------------------
#link script
#----------------------------------------------------------------------

LINK_SCRIPT = $(ROOT_DIR)/STM32F401RETX_FLASH.ld

#----------------------------------------------------------------------
# COMPILER AND ASSEMBLER OPTIONS
#----------------------------------------------------------------------
#compiler options
CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 
CC_FLAGS  = $(CPU) -c -ffunction-sections -fdata-sections -Wall -fstack-usage
AS_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16


#debug flags 
  CC_FLAGS +=  -O0 -g
  AS_FLAGS += -gdwarf-2
  CC_SYMBOLS += -DDEBUG -DUSE_FULL_ASSERT
#no debug
# CC_FLAGS +=  -Os


#define the target version (needed in some include file) :
CC_SYMBOLS += -DSTM32F401xE 

#----------------------------------------------------------------------
# linker options and library to link
#----------------------------------------------------------------------

LD_FLAGS = $(CPU) -specs=nano.specs -T$(LINK_SCRIPT) $(LIB) $(LD_SYS_LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LD_SYS_LIBS = -lm -lc -lgcc -lnosys 
#LD_SYS_LIBS += -lstdc++ -lsupc++ 


#first target to avoid writting 'make all' each time
#$(TARGET):$(TARGET).elf

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).lst $(MOVE)
#----------------------------------------------------------------------
# BUILD OBJECTS
#----------------------------------------------------------------------

# list all object files (in build dir) 
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CSRCS:.c=.o))) 
vpath %.c $(sort $(dir $(CSRCS)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASRCS:.s=.o)))
vpath %.s $(sort $(dir $(ASRCS)))
#OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPPSRCS:.cpp=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CSRCS_LIB:.c=.o)))

#to display a var
#$(info $(OBJECTS) is ${OBJECTS})

#dependencies
dep = $(OBJECTS:.o=.d)  # one dependency file for each source
su=$(OBJECTS:.o=.su)


#compile all object files
#compile all object files
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(AS_FLAGS) $< -o $@

#%.o:%.cpp
#	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98  $(INCLUDE_PATHS) -o $@ $< 

$(BUILD_DIR):
	mkdir $@	


#dependencies
-include $(dep)   # include all dep files in the makefile
#dependencies files .d
#%.d:%.c
%.d:%.o
	@$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(CC_SYMBOLS) $< -MM -MT $(@:.d=.o) >$@

#----------------------------------------------------------------------
# BUILD PROJECT (link)
#----------------------------------------------------------------------

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(LD) $(OBJECTS) $(LD_FLAGS) -o $@
	$(SIZE) $@

# other binary files il needeed

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	@$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/$(TARGET).lst: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	@$(OBJDUMP) -Sdh $< > $@

lst: $(BUILD_DIR)/$(TARGET).lst

size:
	$(SIZE) $(PROJECT).elf

#move elf file to root directory
MOVE: $(BUILD_DIR)/$(TARGET).elf
	cp $(BUILD_DIR)/$(TARGET).elf .

#size: $(BUILD_DIR)/$(TARGET).elf
#	$(SIZE) $(BUILD_DIR)/$(TARGET).elf


#inclusion du contenu des fichiers .d
#-include $(CDEP)

#all: $(PROJECT).bin $(PROJECT).hex 




.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

cleandep:
	rm -f $(dep)



