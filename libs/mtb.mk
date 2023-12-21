#
# This file is generated by ModusToolbox during the 'make getlibs' operation
# Any edits to this file will be lost the next time the library manager is run or
# the next time 'make getlibs' is run.
#
# List of local libraries


# Path to the current BSP
SEARCH_TARGET_APP_KIT_XMC72_EVK=bsps/TARGET_APP_KIT_XMC72_EVK

# The search paths for the included middleware
SEARCH_cat1cm0p=../mtb_shared/cat1cm0p/release-v1.2.0
SEARCH_cmsis=../mtb_shared/cmsis/release-v5.8.0
SEARCH_core-lib=../mtb_shared/core-lib/release-v1.4.1
SEARCH_core-make=../mtb_shared/core-make/release-v3.2.2
SEARCH_mtb-hal-cat1=../mtb_shared/mtb-hal-cat1/release-v2.4.3
SEARCH_mtb-pdl-cat1=../mtb_shared/mtb-pdl-cat1/release-v3.7.0
SEARCH_recipe-make-cat1c=../mtb_shared/recipe-make-cat1c/release-v1.1.1

# Search libraries added to build
SEARCH_MTB_MK+=$(SEARCH_cat1cm0p)
SEARCH_MTB_MK+=$(SEARCH_cmsis)
SEARCH_MTB_MK+=$(SEARCH_core-lib)
SEARCH_MTB_MK+=$(SEARCH_core-make)
SEARCH_MTB_MK+=$(SEARCH_mtb-hal-cat1)
SEARCH_MTB_MK+=$(SEARCH_mtb-pdl-cat1)
SEARCH_MTB_MK+=$(SEARCH_recipe-make-cat1c)

-include $(CY_INTERNAL_APP_PATH)/importedbsp.mk
COMPONENTS += MW_CAT1CM0P
COMPONENTS += MW_CMSIS
COMPONENTS += MW_CORE_LIB
COMPONENTS += MW_CORE_MAKE
COMPONENTS += MW_MTB_HAL_CAT1
COMPONENTS += MW_MTB_PDL_CAT1
COMPONENTS += MW_RECIPE_MAKE_CAT1C

# Register map file
DEVICE_XMC7200D-E272K8384_SVD=$(SEARCH_mtb-pdl-cat1)/devices/COMPONENT_CAT1C/svd/cat1c8m.svd


#
# generate make targets for the graphical editors that are specific to this project
#

bsp-assistant:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name bsp-assistant
.PHONY: bsp-assistant

CY_HELP_bsp-assistant=Launches the BSP Assistant 1.10 GUI
CY_HELP_bsp-assistant_VERBOSE=Launches the BSP Assistant 1.10 GUI. Check the BSP Assistant 1.10 User Guide for more information.
mtb_help_tool_bsp-assistant:
	@:
	$(info $(MTB__SPACE)bsp-assistant       $(CY_HELP_bsp-assistant))

mtb_help_tools_end: mtb_help_tool_bsp-assistant
mtb_help_tool_bsp-assistant: mtb_help_tools_start
.PHONY: mtb_help_tool_bsp-assistant

config:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name device-configurator
.PHONY: config

CY_HELP_config=Launches the Device Configurator 4.10 GUI for the target's modus file
CY_HELP_config_VERBOSE=Launches the Device Configurator 4.10 GUI. Check the Device Configurator 4.10 User Guide for more information.
mtb_help_tool_config:
	@:
	$(info $(MTB__SPACE)config              $(CY_HELP_config))

mtb_help_tools_end: mtb_help_tool_config
mtb_help_tool_config: mtb_help_tools_start
.PHONY: mtb_help_tool_config

device-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name device-configurator
.PHONY: device-configurator

CY_HELP_device-configurator=Launches the Device Configurator 4.10 GUI for the target's modus file
CY_HELP_device-configurator_VERBOSE=Launches the Device Configurator 4.10 GUI. Check the Device Configurator 4.10 User Guide for more information.
mtb_help_tool_device-configurator:
	@:
	$(info $(MTB__SPACE)device-configurator $(CY_HELP_device-configurator))

mtb_help_tools_end: mtb_help_tool_device-configurator
mtb_help_tool_device-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_device-configurator

modlibs:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name library-manager
.PHONY: modlibs

CY_HELP_modlibs=Launches the Library Manager 2.10 GUI
CY_HELP_modlibs_VERBOSE=Launches the Library Manager 2.10 GUI. Check the Library Manager 2.10 User Guide for more information.
mtb_help_tool_modlibs:
	@:
	$(info $(MTB__SPACE)modlibs             $(CY_HELP_modlibs))

mtb_help_tools_end: mtb_help_tool_modlibs
mtb_help_tool_modlibs: mtb_help_tools_start
.PHONY: mtb_help_tool_modlibs

library-manager:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name library-manager
.PHONY: library-manager

CY_HELP_library-manager=Launches the Library Manager 2.10 GUI
CY_HELP_library-manager_VERBOSE=Launches the Library Manager 2.10 GUI. Check the Library Manager 2.10 User Guide for more information.
mtb_help_tool_library-manager:
	@:
	$(info $(MTB__SPACE)library-manager     $(CY_HELP_library-manager))

mtb_help_tools_end: mtb_help_tool_library-manager
mtb_help_tool_library-manager: mtb_help_tools_start
.PHONY: mtb_help_tool_library-manager

qspi-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name qspi-configurator
.PHONY: qspi-configurator

CY_HELP_qspi-configurator=Launches the QSPI Configurator 4.10 GUI for the target's cyqspi file
CY_HELP_qspi-configurator_VERBOSE=Launches the QSPI Configurator 4.10 GUI. Check the QSPI Configurator 4.10 User Guide for more information.
mtb_help_tool_qspi-configurator:
	@:
	$(info $(MTB__SPACE)qspi-configurator   $(CY_HELP_qspi-configurator))

mtb_help_tools_end: mtb_help_tool_qspi-configurator
mtb_help_tool_qspi-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_qspi-configurator

smartio-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name smartio-configurator
.PHONY: smartio-configurator

CY_HELP_smartio-configurator=Launches the Smart I/O Configurator 4.10 GUI for the target's modus file
CY_HELP_smartio-configurator_VERBOSE=Launches the Smart I/O Configurator 4.10 GUI. Check the Smart I/O Configurator 4.10 User Guide for more information.
mtb_help_tool_smartio-configurator:
	@:
	$(info $(MTB__SPACE)smartio-configurator $(CY_HELP_smartio-configurator))

mtb_help_tools_end: mtb_help_tool_smartio-configurator
mtb_help_tool_smartio-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_smartio-configurator

.PHONY: mtb_help_tools_start mtb_help_tools_end
