add_library(lib_lock INTERFACE)

target_sources(lib_lock INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/lock.cpp
        )

add_library(LIB::LOCK ALIAS lib_lock)
