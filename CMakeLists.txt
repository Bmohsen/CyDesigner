cmake_minimum_required(VERSION 3.20)
project(CyDesigner LANGUAGES C CXX)

# 1. C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. CEF Paths
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/external/cef")
set(CEF_RESOURCE_DIR "${CEF_ROOT}/Resources")
set(ASSETS_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")

# Automatically choose between Release/Debug lib folder
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CEF_LIB_DIR "${CEF_ROOT}/Debug")
else()
    set(CEF_LIB_DIR "${CEF_ROOT}/Release")
endif()

# 3. Dependencies
find_package(raylib CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG docking)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

FetchContent_Declare(rlImGui GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git GIT_TAG main)
FetchContent_MakeAvailable(rlImGui)
add_library(rlImGui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_include_directories(rlImGui PUBLIC ${rlimgui_SOURCE_DIR})
target_link_libraries(rlImGui PUBLIC raylib imgui)

# =========================================================
# 4. CEF DLL WRAPPER (Updated for 'base' folder)
# =========================================================
# This glob now explicitly includes the 'base' folder where your logging live
file(GLOB_RECURSE CEF_WRAPPER_SOURCES
    "${CEF_ROOT}/libcef_dll/cpptoc/*.cc"
    "${CEF_ROOT}/libcef_dll/ctocpp/*.cc"
    "${CEF_ROOT}/libcef_dll/wrapper/*.cc"
    "${CEF_ROOT}/libcef_dll/base/*.cc"
)

file(GLOB CEF_ROOT_WRAPPER_SOURCES
    "${CEF_ROOT}/libcef_dll/*.cc" 
)

# Combine them
list(APPEND CEF_WRAPPER_SOURCES ${CEF_ROOT_WRAPPER_SOURCES})

add_library(libcef_dll_wrapper STATIC ${CEF_WRAPPER_SOURCES})

target_compile_definitions(libcef_dll_wrapper PUBLIC
    USING_CEF_SHARED
    WIN32
    _WINDOWS
    NOMINMAX
    UNICODE
    _UNICODE
)

target_compile_definitions(libcef_dll_wrapper PRIVATE
    WRAPPER_NO_LIBCEF_DLL_EXPORTS
    WRAPPING_CEF_SHARED
)

target_include_directories(libcef_dll_wrapper PUBLIC "${CEF_ROOT}")

# =========================================================
# 5. CYDESIGNER APPLICATION
# =========================================================
file(GLOB_RECURSE APP_SOURCES 
    "src/*.cpp" 
    "src/*.h"
    "src/*.c"
)

add_executable(CyDesigner WIN32 ${APP_SOURCES})

target_include_directories(CyDesigner PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CEF_ROOT}"
)

target_link_libraries(CyDesigner PRIVATE
    raylib
    imgui
    rlImGui
    libcef_dll_wrapper
    "${CEF_LIB_DIR}/libcef.lib"
    shlwapi winmm version user32 gdi32 comdlg32 advapi32 ole32 shell32
)

# =========================================================
# 6. MSVC & POST-BUILD
# =========================================================
if (MSVC)
    target_compile_options(CyDesigner PRIVATE /EHsc)
    set_target_properties(CyDesigner PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_target_properties(libcef_dll_wrapper PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Runtime Asset Copying
add_custom_command(TARGET CyDesigner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_LIB_DIR}/libcef.dll" "$<TARGET_FILE_DIR:CyDesigner>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_LIB_DIR}/chrome_elf.dll" "$<TARGET_FILE_DIR:CyDesigner>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CEF_LIB_DIR}/v8_context_snapshot.bin" "$<TARGET_FILE_DIR:CyDesigner>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:CyDesigner>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SRC_DIR}" "$<TARGET_FILE_DIR:CyDesigner>/assets"
)