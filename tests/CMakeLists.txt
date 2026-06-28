add_executable(fourier_core_tests
        fourier_core_tests.cpp
)
target_compile_features(fourier_core_tests PRIVATE cxx_std_23)
target_link_libraries(fourier_core_tests PRIVATE fourier_core)
CompileOptions(fourier_core_tests)
add_test(NAME fourier_core_tests COMMAND fourier_core_tests)
