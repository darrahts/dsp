################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/fourierTransform.cpp \
../src/main.cpp \
../src/waveforms.cpp 

OBJS += \
./src/fourierTransform.o \
./src/main.o \
./src/waveforms.o 

CPP_DEPS += \
./src/fourierTransform.d \
./src/main.d \
./src/waveforms.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


