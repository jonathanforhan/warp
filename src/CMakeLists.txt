file(GLOB_RECURSE APP_SOURCE *.c *.cc *.cpp *.cxx)
add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCE})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
