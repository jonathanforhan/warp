file(GLOB_RECURSE APP_SOURCE *.c*)

add_executable(${CMAKE_PROJECT_NAME} ${APP_SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ".")
