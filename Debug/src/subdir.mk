################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ARConstant.cpp \
../src/bebop.cpp \
../src/bebopControl.cpp \
../src/bebopVideo.cpp \
../src/h264Decoder.cpp \
../src/streamWindow.cpp 

OBJS += \
./src/ARConstant.o \
./src/bebop.o \
./src/bebopControl.o \
./src/bebopVideo.o \
./src/h264Decoder.o \
./src/streamWindow.o 

CPP_DEPS += \
./src/ARConstant.d \
./src/bebop.d \
./src/bebopControl.d \
./src/bebopVideo.d \
./src/h264Decoder.d \
./src/streamWindow.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/SDL2 -I../include -include/usr/include/SDL2/SDL.h -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


