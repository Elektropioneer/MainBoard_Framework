################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../darkside.c \
../greenside.c \
../main.c \
../orangeside.c 

OBJS += \
./darkside.o \
./greenside.o \
./main.o \
./orangeside.o 

C_DEPS += \
./darkside.d \
./greenside.d \
./main.d \
./orangeside.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/silardg/Development/Robotics/Software/MainBoard/inc" -DF_CPU=10000000 -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=10000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


