################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/driver_quiz1.cpp \
../src/test_quiz1.cpp 

OBJS += \
./src/driver_quiz1.o \
./src/test_quiz1.o 

CPP_DEPS += \
./src/driver_quiz1.d \
./src/test_quiz1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/local/bin/g++-5 -std=c++11 -I"/Users/HungryAdi/Documents/workspace/courselib/src" -I"/Users/HungryAdi/Documents/workspace/googletestlib/include" -O0 -g3 -gdwarf-3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


