cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# System Defines
set(SYMS_DEFINE 
    __MSPM0G3519__
    __USE_SYSCONFIG__
    # DeviceFamily_MSPM0G350X
    $<$<CONFIG:Debug>:DEBUG>
)

# MSPM Library
set(MSPM0G_DL_EASY_FRAME_INC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source
)

# CMSIS Library
set(CMSIS_INC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/third_party/CMSIS/Core/Include
)

# Core
set(Core_INC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Inc
)

set(Core_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/startup_mspm0g351x_gcc.c
)

# MSPM source
set(MSPM0G_DL_EASY_FRAME_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_aes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_aesadv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_adc12.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_comp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_crcp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_dac12.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_flashctl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_gpamp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_i2s.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_iwdt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_keystorectl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_lcd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_lfss.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_mathacl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_mcan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_npu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_opa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_rtc_a.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_rtc_b.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_rtc_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_scratchpad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_spgss.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_tamperio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_timer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_timera.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_timerb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_timerg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_trng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_unicomm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_unicommi2cc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_unicommi2ct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_unicommspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_unicommuart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_vref.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/dl_wwdt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/dl_factoryregion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/dl_interrupt.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0c1105_c1106.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0c110x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g1x0x_g3x0x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g352x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g511x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g518x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0gx218_gx207.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0gx51x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0h321x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l111x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l122x_l222x.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l211x_l112x.c
)

# IQ MATH
set(IQMATH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNversion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNtoF.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNtoa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNtables.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNsqrt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNsin_cos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNrsmpy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNrmpy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNrepeat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNmpyIQX.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNmpy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNlog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNfrac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNexp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNdiv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNatan2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_IQNasin_acos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../source/ti/iqmath/_IQNfunctions/_atoIQN.c
)

# Link directories setup
set(EASY_FRAME_LINK_DIRS

)
# Project static libraries
set(EASY_FRAME_LINK_LIBS
    EASY_FRAME_Drivers 
    ${TOOLCHAIN_LINK_LIBRARIES}
	
)

# Interface library for includes and symbols
add_library(mspm0g_easy_frame INTERFACE)
target_include_directories(mspm0g_easy_frame INTERFACE ${MSPM0G_DL_EASY_FRAME_INC})
target_include_directories(mspm0g_easy_frame INTERFACE ${CMSIS_INC})
target_include_directories(mspm0g_easy_frame INTERFACE ${Core_INC})
target_compile_definitions(mspm0g_easy_frame INTERFACE ${SYMS_DEFINE})

# Create EasyFrame_Drivers static library
add_library(EASY_FRAME_Drivers OBJECT)
target_sources(EASY_FRAME_Drivers PRIVATE ${MSPM0G_DL_EASY_FRAME_SRC})
target_sources(EASY_FRAME_Drivers PRIVATE ${IQMATH_SRC})
target_link_libraries(EASY_FRAME_Drivers PUBLIC mspm0g_easy_frame)

# target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MSPM0G_DL_EASY_FRAME_SRC})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Core_SRC})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${EASY_FRAME_LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${EASY_FRAME_LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
