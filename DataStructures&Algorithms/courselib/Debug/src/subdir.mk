################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/driver.cpp \
../src/ics46goody.cpp \
../src/ics_exceptions.cpp \
../src/test_map.cpp \
../src/test_priority_queue.cpp \
../src/test_queue.cpp \
../src/test_set.cpp 

OBJS += \
./src/driver.o \
./src/ics46goody.o \
./src/ics_exceptions.o \
./src/test_map.o \
./src/test_priority_queue.o \
./src/test_queue.o \
./src/test_set.o 

CPP_DEPS += \
./src/driver.d \
./src/ics46goody.d \
./src/ics_exceptions.d \
./src/test_map.d \
./src/test_priority_queue.d \
./src/test_queue.d \
./src/test_set.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/local/bin/g++-5 -std=c++11 -O0 -g3 -gdwarf-3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


