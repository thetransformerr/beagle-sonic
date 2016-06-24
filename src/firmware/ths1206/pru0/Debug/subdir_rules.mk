################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: PRU Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-pru_2.1.2/bin/clpru" -v3 --include_path="C:/ti/ccsv6/ccs_base/pru/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-pru_2.1.2/include" -g --define=am3359 --define=pru0 --diag_wrap=off --diag_warning=225 --display_error_number --endian=little --hardware_mac=on --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


