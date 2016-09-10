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
  SDL_Window.cpp
  Drawable.cpp
  Texture.cpp
  Camera.cpp
  Config.cpp
  Lens.cpp
  Light.cpp
  Util.cpp
  ZBuffer.cpp
  )


add_executable(display3r ${display3r_src})
add_sanitizers(display3r)
target_compile_features(display3r PRIVATE cxx_range_for cxx_constexpr cxx_noexcept)
target_link_libraries(display3r ${SDL2_LIBRARY} ${Boost_LIBRARIES})

install(TARGETS display3r RUNTIME DESTINATION bin)