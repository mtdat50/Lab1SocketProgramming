cmake_minimum_required(VERSION 3.25.0)
project (ClientApp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB AllSourceFiles CONFIGURE_DEPENDS "GameClient/*" "GUI/*" "*.cpp")


include_directories(include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} 
    ${AllSourceFiles}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib 
    opengl32 
    gdi32 
    winmm
    ws2_32
)