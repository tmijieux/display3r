
set(HEADER_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/display3r)

set(display3r_headers
  ${HEADER_DIRECTORY}/Color.hpp
  ${HEADER_DIRECTORY}/Scene.hpp
  ${HEADER_DIRECTORY}/Solid.hpp
  ${HEADER_DIRECTORY}/Frame.hpp
  ${HEADER_DIRECTORY}/Renderer.hpp
  ${HEADER_DIRECTORY}/Equation.hpp
  ${HEADER_DIRECTORY}/Window.hpp
  ${HEADER_DIRECTORY}/WindowMaker.hpp
  ${HEADER_DIRECTORY}/Drawable.hpp
  ${HEADER_DIRECTORY}/Texture.hpp
  ${HEADER_DIRECTORY}/Camera.hpp
  ${HEADER_DIRECTORY}/Config.hpp
  ${HEADER_DIRECTORY}/Lens.hpp
  ${HEADER_DIRECTORY}/Light.hpp
  ${HEADER_DIRECTORY}/Util.hpp
  ${HEADER_DIRECTORY}/ZBuffer.hpp
  ${HEADER_DIRECTORY}/Object2D.hpp
  ${HEADER_DIRECTORY}/Object3D.hpp
  )


set(display3r_src
  Color.cpp
  Scene.cpp
  Solid.cpp
  Frame.cpp
  Renderer.cpp
  RendererLight.cpp
  Renderer2D.cpp
  Renderer3D.cpp
  equation.cpp
  main.cpp
  Window.cpp
  WindowMaker.cpp
  Drawable.cpp
  Texture.cpp
  Camera.cpp
  Config.cpp
  Lens.cpp
  Light.cpp
  Util.cpp
  ZBuffer.cpp
  )

set(sdl_backend_src  SDL/backend_SDL.cpp)
add_library(SDL_3displayer MODULE ${sdl_backend_src})
target_link_libraries(SDL_3displayer ${SDL2_LIBRARY} ${Boost_LIBRARIES})
add_sanitizers(SDL_3displayer)
install(TARGETS SDL_3displayer LIBRARY DESTINATION bin)

if(NOT WIN32)
	set(ncurses_backend_src  ncurses/backend_ncurses.cpp ncurses/ncurses_ctl.cpp)
	add_library(ncurses_3displayer MODULE ${ncurses_backend_src})
	target_link_libraries(ncurses_3displayer ncursesw ${Boost_LIBRARIES})
	add_sanitizers(ncurses_3displayer)
	install(TARGETS ncurses_3displayer LIBRARY DESTINATION bin)
endif()

add_executable(display3r ${display3r_src} ${display3r_headers})
target_compile_features(display3r PRIVATE cxx_range_for)
target_link_libraries(display3r ${SDL2_LIBRARY} ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
add_sanitizers(display3r)
install(TARGETS display3r RUNTIME DESTINATION bin)

