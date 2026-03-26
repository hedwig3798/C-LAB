include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.32.10
)
set(SDL_TESTS OFF CACHE BOOL "Disable SDL tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL examples" FORCE)

FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
    sdl2_gfx
    GIT_REPOSITORY https://github.com/ferzkopp/SDL2_gfx.git
    GIT_TAG        main
)
FetchContent_GetProperties(sdl2_gfx)
if(NOT sdl2_gfx_POPULATED)
    FetchContent_Populate(sdl2_gfx)
endif()
set(GFX_DIR ${sdl2_gfx_SOURCE_DIR})
add_library(
    SDL2_gfx 
    STATIC
        ${GFX_DIR}/SDL2_gfxPrimitives.c
        ${GFX_DIR}/SDL2_rotozoom.c
        ${GFX_DIR}/SDL2_framerate.c
        ${GFX_DIR}/SDL2_imageFilter.c
)
target_link_libraries(SDL2_gfx PRIVATE SDL2::SDL2)
target_include_directories(SDL2_gfx PUBLIC ${GFX_DIR})

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.6-docking
)
FetchContent_MakeAvailable(imgui)

add_library(
    imgui 
    STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)
target_include_directories(
    imgui 
    PUBLIC 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PRIVATE SDL2::SDL2)

add_executable (CollisionTest "CollisionTest.cpp" "CollisionTest.h" "SDLRenderer.h" "SDLRenderer.cpp" "ColliderSimulator.h" "ColliderSimulator.cpp" "BruteForceSimulator.h" "BruteForceSimulator.cpp" "GridSimluator.h" "GridSimluator.cpp" "AABBTreeSimulator.h" "AABBTreeSimulator.cpp")
target_link_libraries(CollisionTest PRIVATE SDL2::SDL2 imgui SDL2_gfx)


add_custom_command(
    TARGET CollisionTest 
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:CollisionTest>
)