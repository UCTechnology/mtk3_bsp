################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/device/adc/sysdepend/m4kn/adc_m4kn.o 

C_DEPS += \
./mtkernel_3/device/adc/sysdepend/m4kn/adc_m4kn.d 


mtkernel_3/device/adc/sysdepend/m4kn/%.o: ../device/adc/sysdepend/m4kn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


