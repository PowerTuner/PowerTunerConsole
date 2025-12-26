cmake_minimum_required(VERSION 3.23)

project(PowerTunerConsole VERSION 1.0 DESCRIPTION "Official PowerTuner Console Client" LANGUAGES CXX)

option(WITH_INTEL "Enable support for Intel CPUs" ON)
option(WITH_AMD "Enable support for AMD CPUs" ON)

set(PROJECT_AUTHOR "kylon")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PRIV_DEFS "")

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets Network TextToSpeech)

qt_standard_project_setup()

configure_file(src/Resources/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

add_subdirectory(src/external/libPWTShared)
add_subdirectory(src/external/libPWTClientCommon)
add_subdirectory(src/external/libPWTClientService)

qt_add_resources(QT_RESOURCE
	src/Resources/resources.qrc
	src/external/commonResources/commonResource.qrc
)

set(PROJECT_SOURCES
	src/ConsoleStyle.h
	src/ConsoleStyle.cpp
	src/Include/Daemon.h
	src/Include/GamepadButton.h
	src/Include/GamepadMap.h
	src/Include/AppQuitBehaviour.h
	src/Include/FloatIconSize.h
	src/Include/TTSID.h
	src/Classes/ConsoleSettings.h
	src/Classes/ConsoleSettings.cpp
	src/Classes/GamepadWorker.h
	src/Classes/GamepadWorker.cpp
	src/Classes/TTSWorker.h
	src/Classes/TTSWorker.cpp

	src/Tabs/ConsoleTab.h
	src/Tabs/ConsoleTab.cpp
	src/Tabs/Include/AddonTab.h

	src/Tabs/Widgets/ConsoleBaseWidget.h
	src/Tabs/Widgets/ConsoleWidget.h
	src/Tabs/Widgets/ConsoleWidget.cpp
	src/Tabs/Widgets/ConsoleLabel.h
	src/Tabs/Widgets/ConsoleLabel.cpp
	src/Tabs/Widgets/ConsoleSelect.h
	src/Tabs/Widgets/ConsoleSelect.cpp
	src/Tabs/Widgets/ConsoleSliderBase.h
	src/Tabs/Widgets/ConsoleSliderBase.cpp
	src/Tabs/Widgets/ConsoleSlider.h
	src/Tabs/Widgets/ConsoleSlider.cpp
	src/Tabs/Widgets/ConsoleSliderUnit.h
	src/Tabs/Widgets/ConsoleSliderUnit.cpp
	src/Tabs/Widgets/ConsoleSliderLevels.h
	src/Tabs/Widgets/ConsoleSliderLevels.cpp
	src/Tabs/Widgets/ConsoleCheckbox.h
	src/Tabs/Widgets/ConsoleCheckbox.cpp
	src/Tabs/Widgets/ConsoleButton.h
	src/Tabs/Widgets/ConsoleButton.cpp
	src/Tabs/Widgets/ConsoleLineEdit.h
	src/Tabs/Widgets/ConsoleLineEdit.cpp
	src/Tabs/Widgets/ConsoleSpinBox.h
	src/Tabs/Widgets/ConsoleSpinBox.cpp
	src/Tabs/Widgets/SectionWidget.h
	src/Tabs/Widgets/SectionWidget.cpp
	src/Tabs/Widgets/SectionItemWidget.h
	src/Tabs/Widgets/SectionItemWidget.cpp

	src/Tabs/Home/Widgets/SystemInfoWidget.h
	src/Tabs/Home/Widgets/SystemInfoWidget.cpp
	src/Tabs/Home/Widgets/CPUInfoWidget.h
	src/Tabs/Home/Widgets/CPUInfoWidget.cpp
	src/Tabs/Home/Widgets/GPUInfoWidget.h
	src/Tabs/Home/Widgets/GPUInfoWidget.cpp
	src/Tabs/Home/Widgets/PowerTunerWidget.h
	src/Tabs/Home/Widgets/PowerTunerWidget.cpp
	src/Tabs/Home/HomeTab.h
	src/Tabs/Home/HomeTab.cpp

	src/Tabs/Log/Widgets/ConsoleLogText.h
	src/Tabs/Log/Widgets/ConsoleLogText.cpp
	src/Tabs/Log/LogTab.h
	src/Tabs/Log/LogTab.cpp

	src/Tabs/Daemons/DaemonsTab.h
	src/Tabs/Daemons/DaemonsTab.cpp
	src/Tabs/Daemons/Widgets/DaemonWidget.h
	src/Tabs/Daemons/Widgets/DaemonWidget.cpp

	src/Tabs/Settings/Widgets/ClientOptions.h
	src/Tabs/Settings/Widgets/ClientOptions.cpp
	src/Tabs/Settings/Widgets/AddDaemonOptions.h
	src/Tabs/Settings/Widgets/AddDaemonOptions.cpp
	src/Tabs/Settings/Widgets/DaemonOptions.h
	src/Tabs/Settings/Widgets/DaemonOptions.cpp
	src/Tabs/Settings/SettingsTab.h
	src/Tabs/Settings/SettingsTab.cpp

	src/Tabs/Profiles/ProfilesTab.h
	src/Tabs/Profiles/ProfilesTab.cpp

	src/Tabs/OS/Linux/SysFs/SysFsTab.h
	src/Tabs/OS/Linux/SysFs/SysFsTab.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUParkWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUSMTWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUSMTWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUScalingGovernorWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUFrequencyWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUFrequencyWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/CPUIdleGovernorWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/CPUIdleGovernorWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerContainer.h
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerContainer.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/BlockQueueSchedulerWidget.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementContainer.h
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementContainer.cpp
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementWidget.h
	src/Tabs/OS/Linux/SysFs/Widgets/MiscPowerManagementWidget.cpp
	src/Tabs/OS/Linux/IntelGPU/IntelGPUTab.h
	src/Tabs/OS/Linux/IntelGPU/IntelGPUTab.cpp
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUBoostWidget.h
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUBoostWidget.cpp
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUFrequencyWidget.h
	src/Tabs/OS/Linux/IntelGPU/Widgets/GPUFrequencyWidget.cpp
	src/Tabs/OS/Linux/AMDGPU/AMDGPUTab.h
	src/Tabs/OS/Linux/AMDGPU/AMDGPUTab.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/DPMForcePerformanceLevelWidget.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/DPMForcePerformanceLevelWidget.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/EngineClockSCLKWidget.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/EngineClockSCLKWidget.cpp
	src/Tabs/OS/Linux/AMDGPU/Widgets/PowerDPMStateWidget.h
	src/Tabs/OS/Linux/AMDGPU/Widgets/PowerDPMStateWidget.cpp

	src/Tabs/OS/Windows/PowerScheme/PowerSchemeTab.h
	src/Tabs/OS/Windows/PowerScheme/PowerSchemeTab.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/PowerSettingWidget.h
	src/Tabs/OS/Windows/PowerScheme/Include/PowerSettingWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/OptionsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Include/OptionsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Include/MinMaxSliderWidget.h
	src/Tabs/OS/Windows/PowerScheme/Include/MinMaxSliderWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSettingsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSettingsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/CopySchemeSettingsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/CopySchemeSettingsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/DuplicateSchemeWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/DuplicateSchemeWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/MiscGlobalOptionsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/MiscGlobalOptionsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/SchemeActionsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeOptionsWidgets/SchemeActionsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemesOptionsWidget.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemesOptionsWidget.cpp
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSelectInput.h
	src/Tabs/OS/Windows/PowerScheme/Widgets/SchemeSelectInput.cpp

	src/Tabs/FanControl/FanControlTab.h
	src/Tabs/FanControl/FanControlTab.cpp
	src/Tabs/FanControl/Widgets/FanCurveWidget.h
	src/Tabs/FanControl/Widgets/FanCurveWidget.cpp
	src/Tabs/FanControl/Widgets/FanControlWidget.h
	src/Tabs/FanControl/Widgets/FanControlWidget.cpp

	src/Widgets/StatusBar.h
	src/Widgets/StatusBar.cpp
	src/Widgets/TrayIcon.h
	src/Widgets/TrayIcon.cpp
	src/Widgets/FloatingIcon.h
	src/Widgets/FloatingIcon.cpp
	src/Widgets/Modal.h
	src/Widgets/ModalYesNo.h
	src/Widgets/ModalYesNo.cpp
	src/Widgets/FloatMicroKeyboard.h
	src/Widgets/FloatMicroKeyboard.cpp

	src/main.cpp
	src/mainwindow.cpp
	src/mainwindow.h
	src/mainwindow.ui
)

if (WITH_INTEL)
	message(STATUS "${PROJECT_NAME}: Intel CPU support is enabled")
	list(APPEND PRIV_DEFS WITH_INTEL)

	list(APPEND PROJECT_SOURCES
		src/Tabs/Vendor/Intel/CPU/CPUTab.h
		src/Tabs/Vendor/Intel/CPU/CPUTab.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/CPUPowerBalanceWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/CPUPowerBalanceWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/EnergyPerformanceBiasWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/EnergyPerformanceBiasWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/FIVRControlWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/GPUPowerBalanceWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/GPUPowerBalanceWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPowerManagement/MiscPowerManagementWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPowerManagement/MiscPowerManagementNHLMWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscPowerManagement/MiscPowerManagementNHLMWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscProcessorFeaturesWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/MiscProcessorFeaturesWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlNHLMWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlNHLMWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlSBWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlSBWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlCU1Widget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerCtl/PowerCtlCU1Widget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerLimitWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PowerLimitWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PP1ConfigWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PP1ConfigWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboPowerCurrentWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboPowerCurrentWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/TurboRatioLimitWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/VRCurrentConfigWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/VRCurrentConfigWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlCoreWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlSBCoreWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlSBCoreWidget.cpp
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlCU1CoreWidget.h
		src/Tabs/Vendor/Intel/CPU/Widgets/PKGCSTConfigControl/PKGCSTConfigControlCU1CoreWidget.cpp

		src/Tabs/Vendor/Intel/HWP/HWPTab.h
		src/Tabs/Vendor/Intel/HWP/HWPTab.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestPkgWidget.h
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestPkgWidget.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestThreadWidget.h
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestThreadWidget.cpp
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestWidget.h
		src/Tabs/Vendor/Intel/HWP/Widgets/HWPRequestWidget.cpp

		src/Tabs/Vendor/Intel/MCHBAR/MCHBARTab.h
		src/Tabs/Vendor/Intel/MCHBAR/MCHBARTab.cpp
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitWidget.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitIVBWidget.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitIVBWidget.cpp
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitTGLWidget.h
		src/Tabs/Vendor/Intel/MCHBAR/Widgets/PkgRaplLimit/PkgRaplLimitTGLWidget.cpp
	)
endif ()

if (WITH_AMD)
	message(STATUS "${PROJECT_NAME}: AMD CPU support is enabled")
	list(APPEND PRIV_DEFS WITH_AMD)

	list(APPEND PROJECT_SOURCES
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/AMDPStateWidget.h
		src/Tabs/OS/Linux/SysFs/Widgets/AMD/AMDPStateWidget.cpp

		src/Tabs/Vendor/AMD/RYCPU/RYCPUTab.h
		src/Tabs/Vendor/AMD/RYCPU/RYCPUTab.cpp
		src/Tabs/Vendor/AMD/RYCPU/Include/RADJSliderWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Include/RADJSliderWidget.cpp
		src/Tabs/Vendor/AMD/RYCPU/Widgets/APUSkinTempWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/APUSlowLimitWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/DGPUSkinTempWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/FastLimitWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/MaxGfxClockWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/MinGfxClockWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/PowerProfileWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/PowerProfileWidget.cpp
		src/Tabs/Vendor/AMD/RYCPU/Widgets/SlowLimitWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/StapmLimitWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/StaticGfxClkWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/TctlTempWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMCurrentWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMMaxCurrentWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMSocCurrentWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/VRMSocMaxCurrentWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerAllWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerCoreWidget.h
		src/Tabs/Vendor/AMD/RYCPU/Widgets/CurveOptimizerCoreWidget.cpp

		src/Tabs/Vendor/AMD/CPU/CPUTab.h
		src/Tabs/Vendor/AMD/CPU/CPUTab.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CorePerformanceBoostWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/PStateWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCWidget.cpp
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCThreadWidget.h
		src/Tabs/Vendor/AMD/CPU/Widgets/CPPCThreadWidget.cpp
	)
endif ()

set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_EXAMPLES OFF)
set(SDL_DISABLE_INSTALL ON)
set(SDL_DISABLE_INSTALL_DOCS ON)
set(SDL_INSTALL_TESTS OFF)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL_SHARED_DEFAULT OFF)
set(SDL_SHARED_AVAILABLE OFF)
set(SDL_AUDIO OFF)
set(SDL_VIDEO OFF)
set(SDL_GPU OFF)
set(SDL_RENDER OFF)
set(SDL_CAMERA OFF)
set(SDL_HAPTIC OFF)
set(SDL_POWER OFF)
set(SDL_JOYSTICK ON)
set(SDL_VIRTUAL_JOYSTICK OFF)
set(SDL_SENSOR OFF)
set(SDL_DIALOG OFF)
set(SDL_UNIX_CONSOLE_BUILD ON)
add_subdirectory(src/external/SDL EXCLUDE_FROM_ALL)
set_property(TARGET SDL3-static PROPERTY POSITION_INDEPENDENT_CODE ON)

if (WIN32)
	add_subdirectory(src/external/libPWTWin32)
	configure_file(src/Resources/Windows/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

	list(APPEND PROJECT_SOURCES
		win.rc
	)
endif ()

qt_add_executable(${PROJECT_NAME} ${QT_RESOURCE} ${PROJECT_SOURCES})

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
	message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PRIV_DEFS})

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt::Core
	Qt::Widgets
	Qt::Network
	Qt::TextToSpeech
	PWT::ClientCommon
	PWT::Shared
	PWT::ClientService
	SDL3::SDL3
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER "kylon.pwtconsole.com"
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
	BUNDLE  DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
	TARGET ${PROJECT_NAME}
	OUTPUT_SCRIPT deploy_script
	NO_TRANSLATIONS
	NO_COMPILER_RUNTIME
	NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
