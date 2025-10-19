add_subdirectory(raylib-modern)
find_package(Boost REQUIRED)
set_target_properties(Boost::boost PROPERTIES IMPORTED_GLOBAL TRUE)
