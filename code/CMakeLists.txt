cmake_minimum_required( VERSION 3.8 )
project(
    ardo
    VERSION 1.0.0
    DESCRIPTION "ARDO - An spatial audio ORDO remake"
    LANGUAGES CXX
)

find_package( SFML 2.5 COMPONENTS system window graphics REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLUT REQUIRED )

add_library( ardolib STATIC
    src/Board.cpp
    src/Game.cpp
    src/Hud.cpp
    src/HudMessage.cpp
    src/NotifciationCenter.cpp
    src/Object.cpp
    src/Renderer.cpp
    src/Sound.cpp
    src/SoundRenderer.cpp
)

target_compile_features( ardolib
    PUBLIC
        cxx_std_17  #  std::optional, nested namespaces
        cxx_auto_type
        cxx_defaulted_functions
        cxx_lambdas
        cxx_range_for
        cxx_trailing_return_types
        cxx_unicode_literals
        cxx_inline_namespaces
)

target_include_directories( ardolib
    PUBLIC
        src
        libs/fmod/api/lowlevel/inc
        libs/fmod/api/studio/inc
)

target_link_libraries( ardolib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api/lowlevel/lib/libfmod.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api/lowlevel/lib/libfmodL.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api/studio/lib/libfmodstudio.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api/studio/lib/libfmodstudioL.dylib
        sfml-system
        sfml-window
        sfml-network
        sfml-graphics
        ${OPENGL_LIBRARIES} 
        ${GLUT_LIBRARY}
)

add_executable ( ardo src/main.cpp )
target_link_libraries( ardo PRIVATE ardolib )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR} )