################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/actuator.c \
../src/ax.c \
../src/debug.c \
../src/gpio.c \
../src/i2c.c \
../src/sensors.c \
../src/system.c \
../src/usart.c 

OBJS += \
./src/actuator.o \
./src/ax.o \
./src/debug.o \
./src/gpio.o \
./src/i2c.o \
./src/sensors.o \
./src/system.o \
./src/usart.o 

C_DEPS += \
./src/actuator.d \
./src/ax.d \
./src/debug.d \
./src/gpio.d \
./src/i2c.d \
./src/sensors.d \
./src/system.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/silard_g/workspace/EP_Framework_Atmel/inc" -DF_CPU=10000000 -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=10000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


