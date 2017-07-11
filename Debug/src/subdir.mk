################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/actuator.c \
../src/ax.c \
../src/can.c \
../src/gpio.c \
../src/odometry.c \
../src/system.c \
../src/usart.c 

OBJS += \
./src/actuator.o \
./src/ax.o \
./src/can.o \
./src/gpio.o \
./src/odometry.o \
./src/system.o \
./src/usart.o 

C_DEPS += \
./src/actuator.d \
./src/ax.d \
./src/can.d \
./src/gpio.d \
./src/odometry.d \
./src/system.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/silard_g/workspace/EP_Framework_Atmel/inc" -DF_CPU=10000000 -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=10000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


