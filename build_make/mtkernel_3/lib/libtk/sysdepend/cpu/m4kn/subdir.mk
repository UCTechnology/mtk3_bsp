################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4kn/int_m4kn.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4kn/ptimer_m4kn.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4kn/int_m4kn.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4kn/ptimer_m4kn.d 

mtkernel_3/lib/libtk/sysdepend/cpu/m4kn/%.o: ../lib/libtk/sysdepend/cpu/m4kn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
