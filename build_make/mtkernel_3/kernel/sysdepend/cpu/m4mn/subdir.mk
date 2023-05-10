################################################################################
# micro T-Kernel 3.00.06  makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/m4mn/vector_tbl.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/m4mn/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/m4mn/%.o: ../kernel/sysdepend/cpu/m4mn/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


