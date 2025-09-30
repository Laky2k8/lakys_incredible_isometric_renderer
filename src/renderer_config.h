#pragma once

#if defined(USE_RAYLIB)
  #include <raylib.h>
  using Texture = Texture2D;
#elif defined(USE_SDL)
  #include <SDL.h>
  using Texture = SDL_Texture*;
#else
  // fallback
  using Texture = void*;
#endif

// Add Texture variable from the rendering system you use
