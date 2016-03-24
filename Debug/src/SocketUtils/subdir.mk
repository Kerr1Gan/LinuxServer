################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SocketUtils/ServerSocket.cpp \
../src/SocketUtils/Socket.cpp \
../src/SocketUtils/SocketProtocol.cpp \
../src/SocketUtils/SocketUtil.cpp 

OBJS += \
./src/SocketUtils/ServerSocket.o \
./src/SocketUtils/Socket.o \
./src/SocketUtils/SocketProtocol.o \
./src/SocketUtils/SocketUtil.o 

CPP_DEPS += \
./src/SocketUtils/ServerSocket.d \
./src/SocketUtils/Socket.d \
./src/SocketUtils/SocketProtocol.d \
./src/SocketUtils/SocketUtil.d 


# Each subdirectory must supply rules for building sources it contributes
src/SocketUtils/%.o: ../src/SocketUtils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/c++ -I"/home/xiang/workspace/Service/src/SocketUtils" -I"/home/xiang/workspace/Service/src" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


