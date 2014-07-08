################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prngApp/src/prngMain.cpp 

C_SRCS += \
../prngApp/src/bpmPosCalc.c \
../prngApp/src/commonDev.c \
../prngApp/src/devBPMai.c \
../prngApp/src/devBPMao.c \
../prngApp/src/devBPMbo.c \
../prngApp/src/devBPMmbbo.c \
../prngApp/src/devBPMwf.c \
../prngApp/src/devprng.c \
../prngApp/src/drvBPM.c \
../prngApp/src/prngdrv.c 

OBJS += \
./prngApp/src/bpmPosCalc.o \
./prngApp/src/commonDev.o \
./prngApp/src/devBPMai.o \
./prngApp/src/devBPMao.o \
./prngApp/src/devBPMbo.o \
./prngApp/src/devBPMmbbo.o \
./prngApp/src/devBPMwf.o \
./prngApp/src/devprng.o \
./prngApp/src/drvBPM.o \
./prngApp/src/prngMain.o \
./prngApp/src/prngdrv.o 

C_DEPS += \
./prngApp/src/bpmPosCalc.d \
./prngApp/src/commonDev.d \
./prngApp/src/devBPMai.d \
./prngApp/src/devBPMao.d \
./prngApp/src/devBPMbo.d \
./prngApp/src/devBPMmbbo.d \
./prngApp/src/devBPMwf.d \
./prngApp/src/devprng.d \
./prngApp/src/drvBPM.d \
./prngApp/src/prngdrv.d 

CPP_DEPS += \
./prngApp/src/prngMain.d 


# Each subdirectory must supply rules for building sources it contributes
prngApp/src/%.o: ../prngApp/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

prngApp/src/%.o: ../prngApp/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


