################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/m4kn/vector_tbl.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/m4kn/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/m4kn/%.o: ../kernel/sysdepend/cpu/m4kn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


