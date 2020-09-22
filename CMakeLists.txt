CMAKE_MINIMUM_REQUIRED( VERSION 3.8 )

project( bullets_mng VERSION 0.1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

set( BULLETS_MNG_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( BULLETS_MNG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )


set( SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE )
set( SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE )
set( BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE )
add_subdirectory( ${BULLETS_MNG_DIR}/externals/SFML )
set( SFML_INCLUDE_DIR ${BULLETS_MNG_DIR}/externals/SFML/include )


set( entry_point ${BULLETS_MNG_SRC_DIR}/main.cpp )
set( Application ${BULLETS_MNG_SRC_DIR}/Application.h ${BULLETS_MNG_SRC_DIR}/Application.cpp )
set( Common ${BULLETS_MNG_SRC_DIR}/Common.h )
set( IScenesSwitcher ${BULLETS_MNG_SRC_DIR}/IScenesSwitcher.h )
set( SceneBase ${BULLETS_MNG_SRC_DIR}/SceneBase.h ${BULLETS_MNG_SRC_DIR}/SceneBase.cpp )
set( RenderedUnit ${BULLETS_MNG_SRC_DIR}/RenderedUnit.h ${BULLETS_MNG_SRC_DIR}/RenderedUnit.cpp )
set( BulletsMngDisplayScene ${BULLETS_MNG_SRC_DIR}/BulletsMngDisplayScene.h ${BULLETS_MNG_SRC_DIR}/BulletsMngDisplayScene.cpp )
set( Line ${BULLETS_MNG_SRC_DIR}/Line.h ${BULLETS_MNG_SRC_DIR}/Line.cpp )
set( Point ${BULLETS_MNG_SRC_DIR}/Point.h ${BULLETS_MNG_SRC_DIR}/Point.cpp )

source_group( main FILES 
                        ${entry_point} 
)

source_group( application FILES 
							${Application}
							${Common}
)

source_group( scene\\base FILES
						${IScenesSwitcher}
						${SceneBase}
)
source_group( scene FILES
						${BulletsMngDisplayScene}
)

source_group( rendered_units\\base FILES
								${RenderedUnit}
)
source_group( rendered_units FILES 
								${Line}
								${Point}
)


set( BULLETS_MNG_SRC 
                    ${entry_point}
					${Application}
					${Common}
					${IScenesSwitcher}
					${SceneBase}
					${RenderedUnit}
					${BulletsMngDisplayScene}
					${Line}
					${Point}
)

add_executable( bullets_mng ${BULLETS_MNG_SRC} )

target_link_libraries( bullets_mng sfml-main sfml-system sfml-window sfml-graphics )
target_include_directories( bullets_mng PRIVATE ${SFML_INCLUDE_DIR} )

if ( MSVC )
    target_compile_options( bullets_mng PRIVATE /MP )

	target_compile_options( bullets_mng PRIVATE /wd4018)

endif()