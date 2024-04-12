cmake_minimum_required(VERSION 3.25.0)
project (ServerApp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB AllSourceFiles CONFIGURE_DEPENDS "GameServer/*" "*.cpp")


include_directories(include)

add_executable(${PROJECT_NAME} 
    ${AllSourceFiles}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ws2_32
)