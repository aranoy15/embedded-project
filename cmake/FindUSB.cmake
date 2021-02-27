message(STATUS "Find USB handler")

set(USB_INCLUDE_DIRS "")
set(USB_SOURCES "")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(USB
    REQUIRED_VARS USB_INCLUDE_DIRS USB_SOURCES
    FOUND_VAR USB_FOUND
    HANDLE_COMPONENTS
)