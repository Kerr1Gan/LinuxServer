################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Server/ServerCore.cpp 

OBJS += \
./src/Server/ServerCore.o 

CPP_DEPS += \
./src/Server/ServerCore.d 


# Each subdirectory must supply rules for building sources it contributes
src/Server/%.o: ../src/Server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/c++ -I"/home/xiang/workspace/Service/src/SocketUtils" -I"/home/xiang/workspace/Service/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


