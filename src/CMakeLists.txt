add_subdirectory(csp)
add_subdirectory(bsp)
add_subdirectory(lib)

if (HAVE_BOOT)
    add_subdirectory(boot)
endif()