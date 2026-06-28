include(CheckCXXCompilerFlag)
function(CompileOptions target_name)
    if (MSVC)
        target_compile_options(${target_name} PRIVATE /W4)
        return()
    endif ()

    set(options
            -Wall
            -Wextra
            -Wpedantic
            -Wno-missing-field-initializers
    )

    foreach (option IN LISTS options)
        string(MAKE_C_IDENTIFIER "has_cxx_flag_${option}" flag_variable)
        check_cxx_compiler_flag("${option}" ${flag_variable})
        if (${flag_variable})
            target_compile_options(${target_name} PRIVATE "${option}")
        endif ()
    endforeach ()
endfunction()
