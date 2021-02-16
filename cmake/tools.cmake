message("Is mingw " MSYS)

function (flash NAME ADDRESS)
    if (MSVC OR MSYS OR MINGW) 
        add_custom_target(${NAME}.flash
	    	ST-LINK_CLI -P ${CMAKE_BINARY_DIR}/${NAME}.bin ${ADDRESS}
	    	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	    	USES_TERMINAL
            )
    else()
	    add_custom_target(${NAME}.flash
	    	st-flash write ${CMAKE_BINARY_DIR}/${NAME}.bin ${ADDRESS}
	    	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	    	USES_TERMINAL
            )
    endif()
endfunction ()
