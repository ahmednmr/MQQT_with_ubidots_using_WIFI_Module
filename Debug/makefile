################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/subdir.mk
-include HAL/subdir.mk
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
MQQT_with_ubidots_using_WIFI.lss \

FLASH_IMAGE += \
MQQT_with_ubidots_using_WIFI.hex \

EEPROM_IMAGE += \
MQQT_with_ubidots_using_WIFI.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: MQQT_with_ubidots_using_WIFI.elf secondary-outputs

# Tool invocations
MQQT_with_ubidots_using_WIFI.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,MQQT_with_ubidots_using_WIFI.map -mmcu=atmega32 -o "MQQT_with_ubidots_using_WIFI.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

MQQT_with_ubidots_using_WIFI.lss: MQQT_with_ubidots_using_WIFI.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S MQQT_with_ubidots_using_WIFI.elf  >"MQQT_with_ubidots_using_WIFI.lss"
	@echo 'Finished building: $@'
	@echo ' '

MQQT_with_ubidots_using_WIFI.hex: MQQT_with_ubidots_using_WIFI.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex MQQT_with_ubidots_using_WIFI.elf  "MQQT_with_ubidots_using_WIFI.hex"
	@echo 'Finished building: $@'
	@echo ' '

MQQT_with_ubidots_using_WIFI.eep: MQQT_with_ubidots_using_WIFI.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex MQQT_with_ubidots_using_WIFI.elf  "MQQT_with_ubidots_using_WIFI.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: MQQT_with_ubidots_using_WIFI.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 MQQT_with_ubidots_using_WIFI.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) MQQT_with_ubidots_using_WIFI.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
