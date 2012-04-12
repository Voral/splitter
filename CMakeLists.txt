cmake_minimum_required ( VERSION 2.6 )
project ( splitter )
set ( SRC_DIR ${PROJECT_SOURCE_DIR}/src )
set ( APP_NAME splitter )
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE RELEASE )
list(APPEND SRC ${SRC_DIR}/main.cpp )
add_executable ( ${APP_NAME} ${SRC} )
