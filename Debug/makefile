################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32F1_MQTT_SMARTHOME.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32F1_MQTT_SMARTHOME.list \

OBJCOPY_HEX += \
STM32F1_MQTT_SMARTHOME.hex \

OBJCOPY_BIN += \
STM32F1_MQTT_SMARTHOME.bin \


# All Target
all: STM32F1_MQTT_SMARTHOME.elf secondary-outputs

# Tool invocations
STM32F1_MQTT_SMARTHOME.elf: $(OBJS) $(USER_OBJS) G:\LMZ\STM32F1_MQTT_SMARTHOME\STM32F1_MQTT_SMARTHOME\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "STM32F1_MQTT_SMARTHOME.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"G:\LMZ\STM32F1_MQTT_SMARTHOME\STM32F1_MQTT_SMARTHOME\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32F1_MQTT_SMARTHOME.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32F1_MQTT_SMARTHOME.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32F1_MQTT_SMARTHOME.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32F1_MQTT_SMARTHOME.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "STM32F1_MQTT_SMARTHOME.hex"
	@echo 'Finished building: $@'
	@echo ' '

STM32F1_MQTT_SMARTHOME.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32F1_MQTT_SMARTHOME.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
