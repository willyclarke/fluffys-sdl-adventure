/**
 * Description: Draw some wireframe objects to veriy projection code.
 *
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <vector>

#include "../src/lib/drawprimitives.hpp"
#include "../src/lib/triangle2d.hpp"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

namespace
{

struct cube
{
   Uint32 Color{};
   bool UseColorGradient{};
   bool Rotate{};
   fluffy::math3d::tup V[8]{};
   fluffy::render::vertice_2d Pixel[8]{};
};

struct fps_info
{
   uint32_t StartTime{};      // When the current second started
   uint32_t FrameCount{};     // Number of frames rendered in the current second
   uint32_t LastFrameTime{};  // Time when the last frame was rendered
   float FPS{};               // The measured FPS value
   fluffy::render::text_fmt Output{};
};

struct screen_objects
{
   fluffy::math3d::FLOAT FOV{90};
   fluffy::render::projection Projection{};
   fluffy::math3d::matrix MatrixProjection{};
   fluffy::math3d::matrix MatrixScreen{};
   fluffy::math3d::matrix MatrixConversion{};

   Uint32 Color{};
   bool UseColorGradient{};
   fluffy::render::vertice_2d V{};
   std::vector<cube> vCubes{};
   std::vector<fluffy::render::text_fmt> vTextObjects{};
   std::string ResourcePath{};

   fps_info FpsInfo{};
};

/**
 * Do clever things with the screen objects.
 */
void ProcessScreenObjects(screen_objects& ScreenObjects)
{
   if (!ScreenObjects.vCubes.empty())
   {
      // auto M = fluffy::math3d::RotateZ(fluffy::math3d::Deg2Rad(0.1));
      auto& Cube = ScreenObjects.vCubes[0];
      auto ZNext = Cube.V[4].Z + 01.0;
      if (ZNext > 50) ZNext = 11;
      Cube.V[4].Z = ZNext;
      Cube.V[5].Z = ZNext;
      Cube.V[6].Z = ZNext;
      Cube.V[7].Z = ZNext;

      /**
       * Create the cube in screen ScreenCoord
       */
      for (size_t Idx = 0;                              //!<
           Idx < sizeof(cube::V) / sizeof(cube::V[0]);  //!<
           ++Idx                                        //!<
      )
      {
         auto P = ScreenObjects.MatrixConversion * Cube.V[Idx];
         Cube.Pixel[Idx] = {P.X, P.Y};
      }
   }

   if (!ScreenObjects.vTextObjects.empty())
   {
      for (auto& TextObject : ScreenObjects.vTextObjects)
      {
         auto& CurrPos = TextObject.Position;
         CurrPos.y += 1;
         if (CurrPos.y > SCREEN_HEIGHT) CurrPos.y = 0;
      }
   }

   /**
    * Generate the FPS indicator.
    */
   {
      auto CurrentTime = SDL_GetTicks();
      auto DeltaTime = CurrentTime - ScreenObjects.FpsInfo.LastFrameTime;
      ScreenObjects.FpsInfo.LastFrameTime = CurrentTime;
      ++ScreenObjects.FpsInfo.FrameCount;

      if (CurrentTime - ScreenObjects.FpsInfo.StartTime >= 1000)  // If a second has passed
      {
         ScreenObjects.FpsInfo.FPS =
             (float)ScreenObjects.FpsInfo.FrameCount;  // fps will be the number of frames rendered in the past second
         ScreenObjects.FpsInfo.Output.Text = std::to_string(int(ScreenObjects.FpsInfo.FPS)) + "fps";
         ScreenObjects.FpsInfo.FrameCount = 0;
         ScreenObjects.FpsInfo.StartTime = CurrentTime;
      }
   }
}

void Render(SDL_Surface* screenSurface, screen_objects& ScreenObjects)
{
   ProcessScreenObjects(ScreenObjects);

   /**
    */
   for (auto& Cube : ScreenObjects.vCubes)
   {
      if (Cube.Rotate)
      {
         fluffy::math3d::FLOAT Angle = fluffy::math3d::Deg2Rad(0.1);
         Cube.Pixel[1] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[1], Angle);
         Cube.Pixel[2] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[2], Angle);
         Cube.Pixel[3] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[3], Angle);
         Cube.Pixel[4] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[4], Angle);
         Cube.Pixel[5] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[5], Angle);
         Cube.Pixel[6] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[6], Angle);
         Cube.Pixel[7] = fluffy::render::Rotate(Cube.Pixel[0], Cube.Pixel[7], Angle);
      }

      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[0], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[1], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[2], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[3], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[4], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[5], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[6], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawCircle(screenSurface, Cube.Pixel[7], 4, Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[0], Cube.Pixel[2], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[0], Cube.Pixel[4], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[0], Cube.Pixel[1], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[1], Cube.Pixel[3], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[1], Cube.Pixel[5], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[2], Cube.Pixel[3], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[2], Cube.Pixel[6], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[3], Cube.Pixel[7], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[4], Cube.Pixel[5], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[4], Cube.Pixel[6], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[5], Cube.Pixel[7], Cube.Color, Cube.UseColorGradient);
      fluffy::render::DrawLine(screenSurface, Cube.Pixel[6], Cube.Pixel[7], Cube.Color, Cube.UseColorGradient);

      /**
       * Display position info.
       */
      if (!ScreenObjects.vTextObjects.empty())
      {
         auto& BaseTO = ScreenObjects.vTextObjects[0];
         fluffy::render::text_fmt PosInfo = BaseTO;
         PosInfo.Position.x = Cube.Pixel[7].X;
         PosInfo.Position.y = Cube.Pixel[7].Y;
         auto strX = std::to_string(PosInfo.Position.x);
         auto strY = std::to_string(PosInfo.Position.y);
         PosInfo.Text = "(" + strX + "," + strY + ")";
         fluffy::render::Text(screenSurface, PosInfo);
         // TTF_SetFontSizeDPI(PosInfo.ptrFont, 14, 140, 140);
      }

      /**
       * Display the FPS.
       */
      fluffy::render::Text(screenSurface, ScreenObjects.FpsInfo.Output);
   }

   for (auto& TextObject : ScreenObjects.vTextObjects)
   {
      break;
      fluffy::render::Text(screenSurface, TextObject);
      auto t = fluffy::math3d::FLOAT(TextObject.Position.y) / fluffy::math3d::FLOAT(SCREEN_HEIGHT);
      constexpr int MinFontSize = 8;
      constexpr int MaxFontSize = 30;
      constexpr int MinDpi = 96;
      constexpr int MaxDpi = 240;
      int ptsize = (1 - t) * MinFontSize + t * MaxFontSize;
      int dpi = (1 - t) * MinDpi + t * MaxDpi;
      // TTF_SetFontSize(TextObject.ptrFont, ptsize);
      TTF_SetFontSizeDPI(TextObject.ptrFont, ptsize, dpi, dpi);
   }
}
};  // namespace

//-----------------------------------------------------------------------------
int main(int argc, char* args[])
{
   // Initialize SDL
   if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
   {
      SDL_Log("SDL could not initialize! SDL_Error: %s.", SDL_GetError());
      return 1;
   }

   if (TTF_Init() == -1)
   {
      SDL_Log("TTF_Init: %s\n", TTF_GetError());
      // Handle the error here...
      SDL_Quit();
      return 1;
   }

   screen_objects ScreenObjects{};

   ScreenObjects.ResourcePath = fluffy::render::GetResourcePath("");
   SDL_Log("ResourcePath: %s", ScreenObjects.ResourcePath.c_str());

   SDL_Window* ptrWindow = SDL_CreateWindow("SDL Pixel Drawing", SCREEN_WIDTH, SCREEN_HEIGHT,
                                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_METAL);
   if (!ptrWindow)
   {
      SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");

   /**
    * Try to figure out if the application is running on a high dpi display.
    */
   fluffy::math3d::FLOAT FontAdjustment{1};
   {
      int Width{};
      int Height{};
      int PixelWidth{};
      int PixelHeight{};

      SDL_GetWindowSize(ptrWindow, &Width, &Height);
      SDL_GetWindowSizeInPixels(ptrWindow, &PixelWidth, &PixelHeight);
      SDL_Log("Window size reported is : Width:%i Height: %i, PixelWidth: %i PixelHeight: %i", Width, Height,
              PixelWidth, PixelHeight);
      if (PixelHeight > Height)
      {
         FontAdjustment = fluffy::math3d::FLOAT(PixelHeight) / Height;
      }
   }

   /**
    * Load a font from the resources directory.
    */
   char const* ptrFontName = "SourceCodePro-Regular.ttf";
   std::string FontName(ptrFontName);
   FontName = ScreenObjects.ResourcePath + "/TTF/" + FontName;

   /**
    * Load the font and try to adjust to the high dpi display if possible.
    */
   int PointSize = static_cast<int>(14 * FontAdjustment);
   TTF_Font* ptrFont = TTF_OpenFont(FontName.c_str(), PointSize);
   if (ptrFont == nullptr)
   {
      SDL_Log("TTF_OpenFont: Failed to load  font %s. Error: %s.", ptrFontName, TTF_GetError());
      SDL_Quit();
      return 1;
   }
   SDL_Log("TTF_OpenFont:  Loaded  font %s with size %i.", FontName.c_str(), PointSize);

   ScreenObjects.FpsInfo.Output.ptrFont = ptrFont;

   SDL_Color textColor = {255, 255, 255};
   SDL_Surface* ptrTextSurface = TTF_RenderUTF8_Solid(ptrFont, "Hello, SDL!", textColor);
   if (ptrTextSurface == nullptr)
   {
      SDL_Log("TTF_RenderText_Solid: %s.", TTF_GetError());
      SDL_Quit();
      return 1;
   }

   // Get window surface
   SDL_Surface* ptrScreenSurface = SDL_GetWindowSurface(ptrWindow);
   if (!ptrScreenSurface)
   {
      SDL_Log("Could not get window surface! SDL_Error: %s.", SDL_GetError());
      SDL_DestroyWindow(ptrWindow);
      SDL_Quit();
      return 1;
   }

   constexpr bool UseColorGradient = true;
   constexpr bool NoColorGradient = false;

   cube Cube{};
   Cube.Color = 0xFF0000;
   Cube.Rotate = true;
   Cube.UseColorGradient = UseColorGradient;
   Cube.V[0] = fluffy::math3d::Point(-1, 1, 10);   //!< a or 0
   Cube.V[1] = fluffy::math3d::Point(1, 1, 10);    //!< b or 1
   Cube.V[2] = fluffy::math3d::Point(-1, -1, 10);  //!< c or 2
   Cube.V[3] = fluffy::math3d::Point(1, -1, 10);   //!< d or 3
   Cube.V[4] = fluffy::math3d::Point(3, 1, 21);    //!< e or 4
   Cube.V[5] = fluffy::math3d::Point(5, 1, 21);    //!< f or 5
   Cube.V[6] = fluffy::math3d::Point(3, -1, 21);   //!< g or 6
   Cube.V[7] = fluffy::math3d::Point(5, -1, 21);   //!< h or 7

   // Main loop flag
   bool Quit{};

   // Event handler
   SDL_Event e{};

   /**
    * NOTE: The renders need to be concave when traversing the vertices in a clockwise direction.
    */
   fluffy::render::vertice_2d V0{SCREEN_WIDTH / fluffy::math3d::FLOAT(2), SCREEN_HEIGHT / fluffy::math3d::FLOAT(4)};
   fluffy::render::vertice_2d V1{SCREEN_WIDTH / fluffy::math3d::FLOAT(2) + 100,
                                 SCREEN_HEIGHT / fluffy::math3d::FLOAT(2)};
   fluffy::render::vertice_2d V2{SCREEN_WIDTH / fluffy::math3d::FLOAT(2) - 100,
                                 SCREEN_HEIGHT / fluffy::math3d::FLOAT(2) - 100};
   fluffy::render::vertice_2d V3{V1.X, V0.Y};

   ScreenObjects.Projection =
       fluffy::render::Projection(SCREEN_WIDTH, SCREEN_HEIGHT, fluffy::math3d::Deg2Rad(ScreenObjects.FOV), -10, 100);
   ScreenObjects.MatrixProjection = fluffy::render::Projection(ScreenObjects.Projection);
   ScreenObjects.MatrixScreen = fluffy::render::ScreenCoord(ScreenObjects.Projection);
   ScreenObjects.MatrixConversion = ScreenObjects.MatrixScreen * ScreenObjects.MatrixProjection;

   /**
    * Create the cube in screen ScreenCoord
    */
   for (size_t Idx = 0;                              //!<
        Idx < sizeof(cube::V) / sizeof(cube::V[0]);  //!<
        ++Idx                                        //!<
   )
   {
      auto P = ScreenObjects.MatrixConversion * Cube.V[Idx];
      Cube.Pixel[Idx] = {P.X, P.Y};

      // std::cout << "Screen coordinate for Cube[" << Idx << "]:" << P << " with color " << Cube.Color << std::endl;
   }
   ScreenObjects.vCubes.push_back(Cube);

   /**
    * Texts to display.
    */
   fluffy::render::text_fmt TextObject{};
   TextObject.Text = "Hello Kitty";
   TextObject.ptrFont = ptrFont;
   ScreenObjects.vTextObjects.push_back(TextObject);

   int ScanCount{};

   // Main loop
   while (!Quit)
   {
      auto CurrentTimeNSBegin = SDL_GetTicksNS();

      // Handle events on queue
      while (SDL_PollEvent(&e) != 0)
      {
         switch (e.type)
         {
            case SDL_EVENT_WINDOW_RESIZED:
            {
               if (e.window.type == SDL_EVENT_WINDOW_RESIZED)
               {
                  // Handle window resizing here
                  SDL_Log("Detected resizing");

                  SDL_DestroySurface(ptrScreenSurface);
                  ptrScreenSurface = SDL_GetWindowSurface(ptrWindow);
                  if (!ptrScreenSurface)
                  {
                     SDL_Log("Could not get window surface! SDL_Error: %s.", SDL_GetError());
                     SDL_DestroyWindow(ptrWindow);
                     SDL_Quit();
                     return 1;
                  }
               }
            }
            break;
            case SDL_EVENT_QUIT:
            {
               Quit = true;
               SDL_Log("Detected quit. Goodbye!");
            }
            break;
         }
      }

      // Lock the surface
      if (SDL_MUSTLOCK(ptrScreenSurface))
      {
         SDL_LockSurface(ptrScreenSurface);
      }

      // Clear the screen to black
      SDL_FillSurfaceRect(ptrScreenSurface, NULL, SDL_MapRGB(ptrScreenSurface->format, 0, 0, 0));

      // Do the rendering
      Render(ptrScreenSurface, ScreenObjects);

      ++ScanCount;

      SDL_Rect textPosition{};
      textPosition.x = 100;
      textPosition.y = ScanCount % SCREEN_HEIGHT;
      // textPosition.w = ptrTextSurface->w;  // Width of the text
      // textPosition.h = ptrTextSurface->h;  // Height of the text
      // textPosition.w = TextObject.ptrSurface->w;
      // textPosition.h = TextObject.ptrSurface->h;
      TextObject.Position = textPosition;
      TextObject.Text = "ScanCount:" + std::to_string(ScanCount);
      TextObject.Dirty = true;

      // SDL_BlitSurface(ptrTextSurface, NULL, screenSurface, &textPosition);
      fluffy::render::Text(ptrScreenSurface, TextObject);

      // Unlock the surface
      if (SDL_MUSTLOCK(ptrScreenSurface))
      {
         SDL_UnlockSurface(ptrScreenSurface);
      }

      // Update the surface
      SDL_UpdateWindowSurface(ptrWindow);

      /**
       * Compute the remaining time to reach the desired framerate.
       */
      {
         auto CurrentTimeNSEnd = SDL_GetTicksNS();
         auto TimeSpent = CurrentTimeNSEnd - CurrentTimeNSBegin;

         constexpr fluffy::math3d::FLOAT FPS = 60;
         constexpr fluffy::math3d::FLOAT DesiredFrameTime =
             fluffy::math3d::FLOAT(1000) / fluffy::math3d::FLOAT(FPS) * 1000000;

         auto TimeRemaining = DesiredFrameTime - TimeSpent;
         Uint32 Delay = std::max(Uint32(5), std::min(Uint32(100), Uint32(TimeRemaining / 1000000)));

         // Wait for a bit
         SDL_Delay(Delay);  // roughly X frames per second
      }
   }

   // Destroy resources
   if (ptrScreenSurface) SDL_DestroySurface(ptrScreenSurface);
   if (ptrWindow) SDL_DestroyWindow(ptrWindow);
   if (ptrTextSurface) SDL_DestroySurface(ptrTextSurface);

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