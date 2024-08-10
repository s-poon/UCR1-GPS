################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/User/app/app.c 

OBJS += \
./Core/User/app/app.o 

C_DEPS += \
./Core/User/app/app.d 


# Each subdirectory must supply rules for building sources it contributes
Core/User/app/%.o Core/User/app/%.su Core/User/app/%.cyclo: ../Core/User/app/%.c Core/User/app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Core/User/app -I../Core/User/gpsParse -I/GPS_IMU/Core/vendor_generated/can_tools -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-User-2f-app

clean-Core-2f-User-2f-app:
	-$(RM) ./Core/User/app/app.cyclo ./Core/User/app/app.d ./Core/User/app/app.o ./Core/User/app/app.su

.PHONY: clean-Core-2f-User-2f-app

