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
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Mainboard_Framework.lss \

FLASH_IMAGE += \
Mainboard_Framework.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Mainboard_Framework.elf secondary-outputs

# Tool invocations
Mainboard_Framework.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Mainboard_Framework.map -mmcu=at90can128 -o "Mainboard_Framework.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Mainboard_Framework.lss: Mainboard_Framework.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Mainboard_Framework.elf  >"Mainboard_Framework.lss"
	@echo 'Finished building: $@'
	@echo ' '

Mainboard_Framework.hex: Mainboard_Framework.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Mainboard_Framework.elf  "Mainboard_Framework.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Mainboard_Framework.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=at90can128 Mainboard_Framework.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Mainboard_Framework.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
