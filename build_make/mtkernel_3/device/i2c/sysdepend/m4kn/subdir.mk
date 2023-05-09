################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/device/i2c/sysdepend/m4kn/i2c_m4kn.o 

C_DEPS += \
./mtkernel_3/device/i2c/sysdepend/m4kn/i2c_m4kn.d 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/i2c/sysdepend/m4kn/%.o: ../device/i2c/sysdepend/m4kn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
