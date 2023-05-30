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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: AirConditioner_main.axf

# Tool invocations
AirConditioner_main.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\student\Desktop\Wbudy\C03_AirConditioner\Lib_CMSISv1p30_LPC17xx\Debug" -L"C:\Users\student\Desktop\Wbudy\C03_AirConditioner\Lib_EaBaseBoard\Debug" -L"C:\Users\student\Desktop\Wbudy\C03_AirConditioner\Lib_MCU\Debug" -Xlinker --gc-sections -Xlinker -Map=AirConditioner_main.map -mcpu=cortex-m3 -mthumb -T "AirConditioner_main_Debug.ld" -o "AirConditioner_main.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) AirConditioner_main.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size AirConditioner_main.axf; # arm-none-eabi-objdump -h -S AirConditioner_main.axf >AirConditioner_main.lss
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
