cmake_minimum_required( VERSION 2.8.11 )

file( GLOB QUICK_LAYER_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    [^.]*.cpp
    [^.]*.mm
    [^.]*.h
)

find_package( Qt5Quick )

include_directories( ${_qt5Core_install_prefix}/include )

add_library( QuickLayer ${QUICK_LAYER_SRC} )

qt5_use_modules( QuickLayer Quick )
