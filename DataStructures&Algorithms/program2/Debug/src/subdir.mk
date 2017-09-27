################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/driver.cpp \
../src/test_priority_queue.cpp \
../src/test_queue.cpp \
../src/test_set.cpp 

OBJS += \
./src/driver.o \
./src/test_priority_queue.o \
./src/test_queue.o \
./src/test_set.o 

CPP_DEPS += \
./src/driver.d \
./src/test_priority_queue.d \
./src/test_queue.d \
./src/test_set.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/local/bin/g++-5 -std=c++11 -I"/Users/HungryAdi/Documents/workspace/courselib/src" -I"/Users/HungryAdi/Documents/workspace/googletestlib/include" -O0 -g3 -gdwarf-3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


