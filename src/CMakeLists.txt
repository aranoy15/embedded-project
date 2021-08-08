add_subdirectory(csp)
#add_subdirectory(bsp)
add_subdirectory(lib)
add_subdirectory(os)
add_subdirectory(app)

if (HAVE_BOOT)
      add_subdirectory(boot)
endif()