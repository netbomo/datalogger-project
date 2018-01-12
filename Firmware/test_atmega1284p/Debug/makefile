################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
test_atmega1284p.lss \

FLASH_IMAGE += \
test_atmega1284p.hex \

EEPROM_IMAGE += \
test_atmega1284p.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: test_atmega1284p.elf secondary-outputs

# Tool invocations
test_atmega1284p.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,test_atmega1284p.map,--cref -mrelax -Wl,--gc-sections -mmcu=atmega1284p -o "test_atmega1284p.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

test_atmega1284p.lss: test_atmega1284p.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S test_atmega1284p.elf  >"test_atmega1284p.lss"
	@echo 'Finished building: $@'
	@echo ' '

test_atmega1284p.hex: test_atmega1284p.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex test_atmega1284p.elf  "test_atmega1284p.hex"
	@echo 'Finished building: $@'
	@echo ' '

test_atmega1284p.eep: test_atmega1284p.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex test_atmega1284p.elf  "test_atmega1284p.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: test_atmega1284p.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega1284p test_atmega1284p.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(S_DEPS)$(LSS)$(C_DEPS)$(FLASH_IMAGE)$(CC_DEPS)$(ELFS)$(C++_DEPS)$(OBJS)$(CXX_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(SIZEDUMMY)$(S_UPPER_DEPS)$(CPP_DEPS) test_atmega1284p.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
