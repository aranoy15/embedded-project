message(STATUS "Find USB handler")

set(DEVICE_FOLDER_NAME "STM32_USB_Device_Library")

set(DEVICE_USB_INCLUDE_DIRS "")
set(DEVICE_USB_SOURCES "")

if (EXISTS ${STM32_CUBE_${FAMILY}_PATH})
    set(DEVICE_BASE_FOLDER "${STM32_CUBE_${FAMILY}_PATH}/Middlewares/ST/${DEVICE_FOLDER_NAME}")

    foreach(COMP ${DeviceUSB_FIND_COMPONENTS})
        string(TOLOWER ${COMP} COMP_L)

        set(DEVICE_USB_CORE_FOLDER "${DEVICE_BASE_FOLDER}/Core")

        if (NOT (TARGET USB::DEVICE::CORE))
            message(STATUS "Configure Device USB Core library")

            set(DEVICE_CORE_SOURCE_LIST "")
            set(DEVICE_CORE_INCLUDE "${DEVICE_USB_CORE_FOLDER}/Inc")

            list(APPEND DEVICE_CORE_SOURCE_LIST "${DEVICE_USB_CORE_FOLDER}/Src/usbd_core.c")
            list(APPEND DEVICE_CORE_SOURCE_LIST "${DEVICE_USB_CORE_FOLDER}/Src/usbd_ctlreq.c")
            list(APPEND DEVICE_CORE_SOURCE_LIST "${DEVICE_USB_CORE_FOLDER}/Src/usbd_ioreq.c")

            list(APPEND DEVICE_USB_SOURCES ${DEVICE_CORE_SOURCE_LIST})
            list(APPEND DEVICE_USB_INCLUDE_DIRS ${DEVICE_CORE_INCLUDE})

            add_library(USB::DEVICE::CORE INTERFACE IMPORTED)
            target_include_directories(USB::DEVICE::CORE INTERFACE "${DEVICE_CORE_INCLUDE}")
            target_sources(USB::DEVICE::CORE INTERFACE "${DEVICE_CORE_SOURCE_LIST}")
        endif()

        if (NOT (TARGET USB::DEVICE::${COMP}))
            set(DEVICE_USB_${COMP}_FOLDER "${DEVICE_BASE_FOLDER}/Class/${COMP}")

            set(DEVICE_${COMP}_SOURCE_LIST "")
            set(DEVICE_${COMP}_INCLUDE "${DEVICE_USB_${COMP}_FOLDER}/Inc")

            list(APPEND DEVICE_${COMP}_SOURCE_LIST "${DEVICE_USB_${COMP}_FOLDER}/Src/usbd_${COMP_L}.c")

            add_library(USB::DEVICE::${COMP} INTERFACE IMPORTED)
            target_link_libraries(USB::DEVICE::${COMP} INTERFACE USB::DEVICE::CORE)
            target_include_directories(USB::DEVICE::${COMP} INTERFACE "${DEVICE_${COMP}_INCLUDE}")
            target_sources(USB::DEVICE::${COMP} INTERFACE "${DEVICE_${COMP}_SOURCE_LIST}")

            list(APPEND DEVICE_USB_SOURCES ${DEVICE_${COMP}_SOURCE_LIST})
            list(APPEND DEVICE_USB_INCLUDE_DIRS ${DEVICE_${COMP}_INCLUDE})

            set(DeviceUSB_${COMP}_FOUND TRUE)
        endif()

    endforeach()
endif()

list(REMOVE_DUPLICATES DEVICE_USB_INCLUDE_DIRS)
list(REMOVE_DUPLICATES DEVICE_USB_SOURCES)

#message(STATUS "Device USB Includes: ${DEVICE_USB_INCLUDE_DIRS}")
#message(STATUS "Device USB sources: ${DEVICE_USB_SOURCES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DeviceUSB
    REQUIRED_VARS DEVICE_USB_INCLUDE_DIRS DEVICE_USB_SOURCES
    FOUND_VAR DEVICEUSB_FOUND
    HANDLE_COMPONENTS
)