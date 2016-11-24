################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/error.c \
../src/micro.c \
../src/parser.c \
../src/scanner.c \
../src/semantic.c \
../src/symbol.c 

OBJS += \
./src/error.o \
./src/micro.o \
./src/parser.o \
./src/scanner.o \
./src/semantic.o \
./src/symbol.o 

C_DEPS += \
./src/error.d \
./src/micro.d \
./src/parser.d \
./src/scanner.d \
./src/semantic.d \
./src/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


