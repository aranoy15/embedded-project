message(STATUS "Find USB handler")

if (EXISTS ${STM32_CUBE_${FAMILY}_PATH})
    foreach(COMP ${USB_FIND_COMPONENTS})
        message(STATUS "Component ${COMP}")
    endforeach()
    
else()
    message(STATUS "Can't find cube family path")
endif()

set(USB_INCLUDE_DIRS "")
set(USB_SOURCES "")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(USB
    REQUIRED_VARS USB_INCLUDE_DIRS USB_SOURCES
    FOUND_VAR USB_FOUND
    HANDLE_COMPONENTS
)