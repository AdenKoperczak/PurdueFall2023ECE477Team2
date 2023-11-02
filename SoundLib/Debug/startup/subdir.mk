################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -I"C:/Users/awkop/Desktop/Documents/Purdue/2023Fall/ECE477/PurdueFall2023ECE477Team2/SoundLib/StdPeriph_Driver/inc" -I"C:/Users/awkop/Desktop/Documents/Purdue/2023Fall/ECE477/PurdueFall2023ECE477Team2/SoundLib/inc" -I"C:/Users/awkop/Desktop/Documents/Purdue/2023Fall/ECE477/PurdueFall2023ECE477Team2/SoundLib/CMSIS/device" -I"C:/Users/awkop/Desktop/Documents/Purdue/2023Fall/ECE477/PurdueFall2023ECE477Team2/SoundLib/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


