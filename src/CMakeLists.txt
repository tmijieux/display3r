set(display3r_src
  Color.cpp
  Face.cpp
  Scene.cpp
  Vertex.cpp
  Solid.cpp
  Frame.cpp
  Renderer.cpp
  Renderer2D.cpp
  equation.cpp
  Renderer3D.cpp
  main.cpp
  Window.cpp
  SDL_Window.cpp
  Object.cpp
  Texture.cpp
  Camera.cpp
  Config.cpp
  )


add_executable(display3r ${display3r_src})
add_sanitizers(display3r)
target_compile_features(display3r PRIVATE cxx_range_for cxx_constexpr cxx_noexcept)
target_link_libraries(display3r ${SDL2_LIBRARY})

install(TARGETS display3r RUNTIME DESTINATION bin)