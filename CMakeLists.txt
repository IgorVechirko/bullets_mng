CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )

project( bullets_mng VERSION 0.1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

set( BULLETS_MNG_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( BULLETS_MNG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )


set( entry_point ${BULLETS_MNG_SRC_DIR}/main.cpp )

source_group( main FILES 
                        ${entry_point} 
)


set( BULLETS_MNG_SRC 
                    ${entry_point} 
)

add_executable( bullets_mng ${BULLETS_MNG_SRC} )