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
  SharedLibrary.cpp
  )

set(sdl_backend_src  SDL/backend_SDL.cpp)
add_library(SDL_3displayer MODULE ${sdl_backend_src})
target_link_libraries(SDL_3displayer ${SDL2_LIBRARY})
add_sanitizers(SDL_3displayer)

set(ncurses_backend_src  ncurses/backend_ncurses.cpp ncurses/ncurses_ctl.cpp)
add_library(ncurses_3displayer MODULE ${ncurses_backend_src})
target_link_libraries(ncurses_3displayer ncursesw)
add_sanitizers(ncurses_3displayer)

add_executable(display3r ${display3r_src})
add_sanitizers(display3r)

target_compile_features(display3r PRIVATE cxx_range_for cxx_constexpr cxx_noexcept)
target_link_libraries(display3r ${SDL2_LIBRARY} ${Boost_LIBRARIES} dl)

install(TARGETS display3r RUNTIME DESTINATION bin)
install(TARGETS SDL_3displayer LIBRARY DESTINATION bin)
install(TARGETS ncurses_3displayer LIBRARY DESTINATION bin)