#find packages that we depend on
find_package( SDL REQUIRED )
find_package( SDL_net REQUIRED )
find_package( SDL_sound REQUIRED )
find_package( SDL_mixer REQUIRED )
find_package( OpenGL REQUIRED )
find_package( X11 REQUIRED )
find_package( PNG REQUIRED )
find_package( JPEG REQUIRED )
find_package( BZip2 REQUIRED )
find_package( ZLIB REQUIRED )

if ( NOT SDL_FOUND )
	message ( SEND_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

if ( NOT SDLNET_FOUND )
	message ( STATUS "SDL_net not found!" )
endif ( NOT SDLNET_FOUND )

if ( NOT SDL_SOUND_FOUND )
	message ( STATUS "SDL_sound not found!" )
endif ( NOT SDL_SOUND_FOUND )

if ( NOT SDLMIXER_FOUND )
	message ( SEND_ERROR "SDL_mixer not found!" )
endif ( NOT SDLMIXER_FOUND )

if ( NOT OPENGL_FOUND )
	message ( STATUS "OPENGL not found!" )
endif ( NOT OPENGL_FOUND )

include_directories(
	include/
	${SDL_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${X11_INCLUDE_DIR}
	/usr/include/irrlicht/
	#	/usr/include/irrKlang/
)

link_directories(
	/usr/lib/
	/usr/local/lib/
)

link_libraries( LudumDare48
	${SDL_LIBRARY}
	${SDLNET_LIBRARY}
	${SDLSOUND_LIBRARIES}
	${SDLMIXER_LIBRARY}
	${OPENGL_LIBRARIES}
	${X11_LIBRARIES}
	${PNG_LIBRARIES}
	${JPEG_LIBRARIES}
	${ZLIB_LIBRARIES}
	${BZIP2_LIBRARIES}
	Xxf86vm Xext
	Irrlicht
	#	IrrKlang
	)

set(sourcefiles
	main.cpp
	player.cpp
	level.cpp
	device.h
	)

set (CMAKE_BUILD_TYPE debug)
add_executable(LudumDare48 ${sourcefiles})
install_programs(TARGETS LudumDare48 DESTINATION LudumDare48/bin)
