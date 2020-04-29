cmake_minimum_required(VERSION 3.13)
project(ssphh)


if(APPLE OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD")
    message("adding /usr/local/include because Apple or BSD detected")
    message("adding /usr/local/lib     because Apple or BSD detected")
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()


include_directories(~/github/fluxions/include)
include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME}
	src/AuroraVisualization.cpp
	src/camera_animation.cpp
	src/Damselfish.cpp
	src/DragDrop.cpp
	src/GLFW_template.cpp
	src/GLUT_template.cpp
	src/help_window.cpp
	src/jellyfish.cpp
	src/main.cpp
	src/PbskyViewController.cpp
	src/pch.cpp
	src/renderer_config_window.cpp
	src/renderer_window.cpp
	src/scene_editor_window.cpp
	src/scene_graph_window.cpp
	src/sphhapp_ssphh.cpp
	src/SphlJob.cpp
	src/ssphhapp.cpp
	src/ssphhapp_pathanim.cpp
	src/ssphhapp_pbsky.cpp
	src/ssphhapp_renderconfigs.cpp
	src/ssphhapp_render.cpp
	src/ssphhapp_render_gl11.cpp
	src/ssphhapp_render_gles30.cpp
	src/ssphhapp_render_shadows.cpp
	src/ssphhapp_scenegraph.cpp
	src/ssphhapp_unicornfish.cpp
	src/ssphhapp_update.cpp
	src/ssphhapp_usercontrols.cpp
	src/ssphh_corona.cpp
	src/ssphh_imgui.cpp
	src/ssphh_imgui_tests.cpp
	src/ssphh_menu.cpp
	src/ssphh_ppmcompare.cpp
	src/ssphh_shaders.cpp
	src/ssphh_tests.cpp
	src/ssphh_uf_broker.cpp
	src/ssphh_uf_client.cpp
	src/ssphh_uf.cpp
	src/ssphh_uf_imgui.cpp
	src/ssphh_uf_worker.cpp
	src/ssphh_window.cpp
	src/tests.cpp
	src/tool_window.cpp
	src/unicornfish_window.cpp
    )

target_link_libraries(${PROJECT_NAME} fluxions)


message("Found " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER_VERSION})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(LLVM_EXPECTED_VERSION 10.0.0)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS LLVM_EXPECTED_VERSION)
        message("Using stdc++fs because Clang 10 not detected")
        target_link_libraries(${PROJECT_NAME} c++fs)
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(GCC_EXPECTED_VERSION 9.0.0)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_EXPECTED_VERSION)
        message("Using stdc++fs because G++ 9 not detected")
        target_link_libraries(${PROJECT_NAME} stdc++fs)
    endif()
endif()


find_library(SDL2_LIB NAMES SDL2)
find_library(SDL2_IMAGE_LIB NAMES SDL2_image)
find_library(SDL2_MIXER_LIB NAMES SDL2_mixer)
find_library(SDL2_TTF_LIB NAMES SDL2_ttf)
find_library(CZMQ_LIB NAMES czmq)
find_library(OPENGL_LIB NAMES GL)
find_library(GLEW_LIB NAMES GLEW)
find_library(GLFW_LIB NAMES glfw)
find_library(CURL_LIB NAMES curl)
find_library(PTHREAD_LIB NAMES pthread)
find_library(ILMIMF_LIB NAMES IlmImf)
find_library(ILMHALF_LIB NAMES Half)
find_package(glfw3 3.2 REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL2_LIB}
    ${SDL2_IMAGE_LIB}
    ${SDL2_MIXER_LIB}
    ${SDL2_TTF_LIB}
    ${CZMQ_LIB}
    ${OPENGL_LIB}
    ${GLEW_LIB}
    ${GLFW_LIB}
    ${CURL_LIB}
    ${PTHREAD_LIB}
    ${ILMIMF_LIB}
    ${ILMHALF_LIB}
    )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

