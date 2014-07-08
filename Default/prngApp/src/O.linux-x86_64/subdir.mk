################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../prngApp/src/O.linux-x86_64/bpmPosCalc.o \
../prngApp/src/O.linux-x86_64/commonDev.o \
../prngApp/src/O.linux-x86_64/devBPMai.o \
../prngApp/src/O.linux-x86_64/devBPMao.o \
../prngApp/src/O.linux-x86_64/devBPMbo.o \
../prngApp/src/O.linux-x86_64/devBPMmbbo.o \
../prngApp/src/O.linux-x86_64/devBPMwf.o \
../prngApp/src/O.linux-x86_64/devprng.o \
../prngApp/src/O.linux-x86_64/drvBPM.o \
../prngApp/src/O.linux-x86_64/prngMain.o \
../prngApp/src/O.linux-x86_64/prng_registerRecordDeviceDriver.o 

CPP_SRCS += \
../prngApp/src/O.linux-x86_64/prng_registerRecordDeviceDriver.cpp 

OBJS += \
./prngApp/src/O.linux-x86_64/prng_registerRecordDeviceDriver.o 

CPP_DEPS += \
./prngApp/src/O.linux-x86_64/prng_registerRecordDeviceDriver.d 


# Each subdirectory must supply rules for building sources it contributes
prngApp/src/O.linux-x86_64/%.o: ../prngApp/src/O.linux-x86_64/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


