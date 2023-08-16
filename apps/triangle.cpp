/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <SDL3/SDL.h>

#include <iostream>
#include <vector>

#include "../src/lib/drawprimitives.hpp"
#include "../src/lib/triangle2d.hpp"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

namespace
{

struct screen_render
{
   Uint32 Color{};
   bool UseColorGradient{};
   fluffy::render::vertice_2d V0{};
   fluffy::render::vertice_2d V1{};
   fluffy::render::vertice_2d V2{};
};

//-----------------------------------------------------------------------------
auto Fillrender(SDL_Surface* screenSurface,            //!<
                fluffy::render::vertice_2d const& V0,  //!<
                fluffy::render::vertice_2d const& V1,  //!<
                fluffy::render::vertice_2d const& V2,  //!<
                Uint32 Color,                          //!<
                bool UseColorGradient                  //!<
                )                                      //!<
    -> void
{
   auto BB = fluffy::render::BoundingBox(V0, V1, V2);
   // Draw on the surface
   for (int Y = BB.Min.Y; Y < BB.Max.Y; ++Y)
   {
      bool InsideDetected{};

      for (int X = BB.Min.X; X < BB.Max.X; ++X)
      {
         auto AreaParallelPiped = fluffy::render::EdgeCross(V0, V1, V2);
         if (AreaParallelPiped <= fluffy::math3d::EPSILON) continue;  // dont want to divide by Zero.

         fluffy::render::vertice_2d const P{static_cast<fluffy::math3d::FLOAT>(X),
                                            static_cast<fluffy::math3d::FLOAT>(Y)};

         /** Find cross products between the edges and the point P. These are the Barycentric weights. */
         auto const W0 = fluffy::render::EdgeCross(V1, V2, P);
         auto const W1 = fluffy::render::EdgeCross(V0, V1, P);
         auto const W2 = fluffy::render::EdgeCross(V2, V0, P);

         /** Compute the Barycentric coordinates */
         auto const Alfa = W0 / AreaParallelPiped;
         auto const Beta = W1 / AreaParallelPiped;
         auto const Gamma = W2 / AreaParallelPiped;

         // if (fluffy::render::Isinside(P, V0, V1, V2))
         if (W0 >= 0 && W1 >= 0 && W2 >= 0)  // then point is on the inside
         {
            InsideDetected = true;
            if (UseColorGradient)
               Color = uint8_t(Alfa * 0xFF) << 16 | uint8_t(Beta * 0xFF) << 8 | uint8_t(Gamma * 0xFF);
            ((Uint32*)screenSurface->pixels)[Y * screenSurface->w + X] = Color;
         }
         else if (InsideDetected)  // break to do the next Y.
         {
            break;
         }
         else  // continue with the next X.
         {
            continue;
         }
      }
   }
}

//-----------------------------------------------------------------------------
void Render(SDL_Surface* screenSurface, std::vector<screen_render>& vrenders)
{
   int Count{};

   fluffy::math3d::FLOAT Angle = fluffy::math3d::Deg2Rad(0.1);
   for (auto& render : vrenders)
   {
      render.V1 = fluffy::render::Rotate(render.V0, render.V1, Angle);
      render.V2 = fluffy::render::Rotate(render.V0, render.V2, Angle);
      Fillrender(screenSurface, render.V0, render.V1, render.V2, render.Color, render.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, render.V0, render.V1, render.Color, render.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, render.V0, render.V2, render.Color, render.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, render.V1, render.V2, render.Color, render.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, render.V0, fluffy::math3d::FLOAT(10), render.Color,
                                 render.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, render.V1, fluffy::math3d::FLOAT(10), render.Color,
                                 render.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, render.V2, fluffy::math3d::FLOAT(10), render.Color,
                                 render.UseColorGradient);
   }

   static int Call{};
   ++Call;
}
};  // end of anonymous namespace

//-----------------------------------------------------------------------------
int main(int argc, char* args[])
{
   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      return 1;
   }

   // Create a window
   SDL_Window* window = SDL_CreateWindow("SDL Pixel Drawing", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
   if (!window)
   {
      SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return 1;
   }

   // Get window surface
   SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
   if (!screenSurface)
   {
      SDL_Log("Could not get window surface! SDL_Error: %s\n", SDL_GetError());
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 1;
   }

   // Main loop flag
   bool quit = false;

   // Event handler
   SDL_Event e;

   fluffy::math3d::FLOAT X{};

   /**
    * NOTE: The renders need to be concave when traversing the vertices in a clockwise direction.
    */
   fluffy::render::vertice_2d V0{SCREEN_WIDTH / fluffy::math3d::FLOAT(2), SCREEN_HEIGHT / fluffy::math3d::FLOAT(4)};
   fluffy::render::vertice_2d V1{SCREEN_WIDTH / fluffy::math3d::FLOAT(2) + 100,
                                 SCREEN_HEIGHT / fluffy::math3d::FLOAT(2)};
   fluffy::render::vertice_2d V2{SCREEN_WIDTH / fluffy::math3d::FLOAT(2) - 100,
                                 SCREEN_HEIGHT / fluffy::math3d::FLOAT(2) - 100};
   fluffy::render::vertice_2d V3{V1.X, V0.Y};

   std::vector<screen_render> vScreenrender{};
   constexpr bool UseColorGradient = true;
   constexpr bool NoColorGradient = false;
   vScreenrender.push_back(screen_render{0xFF0000, UseColorGradient, V0, V1, V2});
   vScreenrender.push_back(screen_render{0x00FF00, NoColorGradient, V0, V3, V1});

   // Main loop
   while (!quit)
   {
      // Handle events on queue
      while (SDL_PollEvent(&e) != 0)
      {
         // User requests quit
         if (e.type == SDL_EVENT_QUIT)
         {
            quit = true;
            std::cout << "Detected quit. Goodbye!" << std::endl;
         }
      }

      // Lock the surface
      if (SDL_MUSTLOCK(screenSurface))
      {
         SDL_LockSurface(screenSurface);
      }

      // Clear the screen to black
      SDL_FillSurfaceRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

      // Do the rendering
      Render(screenSurface, vScreenrender);

      // Unlock the surface
      if (SDL_MUSTLOCK(screenSurface))
      {
         SDL_UnlockSurface(screenSurface);
      }

      // Update the surface
      SDL_UpdateWindowSurface(window);

      // Wait for a bit
      SDL_Delay(1000 / 60);  // roughly 60 frames per second
   }

   // Destroy window
   SDL_DestroyWindow(window);

   // Quit SDL subsystems
   SDL_Quit();

   std::cout << "Cleanup complete. All good." << std::endl;

   return 0;
}

/**
* The MIT License (MIT)
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
