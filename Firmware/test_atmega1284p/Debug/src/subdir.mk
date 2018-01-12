################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FSM.cpp \
../src/Usart.cpp \
../src/adc_calib.cpp \
../src/esp826612.cpp \
../src/main.cpp \
../src/operator.cpp 

OBJS += \
./src/FSM.o \
./src/Usart.o \
./src/adc_calib.o \
./src/esp826612.o \
./src/main.o \
./src/operator.o 

CPP_DEPS += \
./src/FSM.d \
./src/Usart.d \
./src/adc_calib.d \
./src/esp826612.d \
./src/main.d \
./src/operator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega1284p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


