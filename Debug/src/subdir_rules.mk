################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --include_path="C:/Users/hungv/Downloads/MC_Lab_LCD-master" --include_path="C:/Users/hungv/Downloads/MC_Lab_LCD-master/include" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --advice:power="9,10.1" --define=ccs="ccs" --define=PART_TM4C1294NCPDT --gcc --abi=eabi --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/init_peripherals.obj: ../src/init_peripherals.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --include_path="C:/Users/hungv/Downloads/MC_Lab_LCD-master" --include_path="C:/Users/hungv/Downloads/MC_Lab_LCD-master/include" --include_path="C:/ti/TivaWare_C_Series-2.1.4.178" --advice:power="9,10.1" --define=ccs="ccs" --define=PART_TM4C1294NCPDT --gcc --abi=eabi --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


