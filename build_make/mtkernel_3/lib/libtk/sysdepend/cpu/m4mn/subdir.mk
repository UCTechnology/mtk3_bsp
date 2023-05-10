################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4mn/int_m4mn.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4mn/ptimer_m4mn.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4mn/int_m4mn.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/m4mn/ptimer_m4mn.d 

mtkernel_3/lib/libtk/sysdepend/cpu/m4mn/%.o: ../lib/libtk/sysdepend/cpu/m4mn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
