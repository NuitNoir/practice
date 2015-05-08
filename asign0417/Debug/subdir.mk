################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../boost_practice.cpp \
../cv_practice.cpp \
../ls_cv.cpp \
../ls_practice.cpp 

OBJS += \
./boost_practice.o \
./cv_practice.o \
./ls_cv.o \
./ls_practice.o 

CPP_DEPS += \
./boost_practice.d \
./cv_practice.d \
./ls_cv.d \
./ls_practice.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


