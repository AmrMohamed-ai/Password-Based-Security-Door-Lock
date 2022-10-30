################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CONTROL_ECU.c \
../Dc_Motor.c \
../external_eeprom.c \
../i2c.c \
../lcd.c \
../timer0_config.c \
../uart_config.c \
../used_functions2.c 

OBJS += \
./CONTROL_ECU.o \
./Dc_Motor.o \
./external_eeprom.o \
./i2c.o \
./lcd.o \
./timer0_config.o \
./uart_config.o \
./used_functions2.o 

C_DEPS += \
./CONTROL_ECU.d \
./Dc_Motor.d \
./external_eeprom.d \
./i2c.d \
./lcd.d \
./timer0_config.d \
./uart_config.d \
./used_functions2.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


