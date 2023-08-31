/**
 * Description: Draw some wireframe objects to veriy projection code.
 *
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <iostream>
#include <vector>

#include "../src/lib/drawprimitives.hpp"
#include "../src/lib/triangle2d.hpp"

namespace
{
struct screen_dimension
{
   static constexpr int SCREEN_WIDTH = 800;
   static constexpr int SCREEN_HEIGHT = 600;

   int Width{};
   int Height{};
   int PixelWidth{};
   int PixelHeight{};
};

screen_dimension gScreenDimension{};

constexpr bool UseColorGradient = true;
constexpr bool NoColorGradient = false;

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
   fluffy::math3d::FLOAT FOV{150};
   fluffy::render::projection Projection{};
   fluffy::math3d::matrix MatrixProjection{};
   fluffy::math3d::matrix MatrixScreen{};
   fluffy::math3d::matrix MatrixConversion{};

   Uint32 Color{};
   bool UseColorGradient{};
   fluffy::render::vertice_2d V{};

   fluffy::math3d::tup PStart{};
   fluffy::math3d::tup PEnd{};
   std::vector<fluffy::math3d::tup> SplineCtrlPoints{};
   fluffy::math3d::FLOAT t{};
   fluffy::math3d::FLOAT tdirection{1};
   fluffy::render::text_fmt TextSplineInfo{};
   std::vector<fluffy::math3d::tup> vSpline{};

   std::vector<cube> vCubes{};
   std::vector<fluffy::render::text_fmt> vTextObjects{};
   std::string ResourcePath{};

   fps_info FpsInfo{};
};

/**
 * Do clever things with the screen objects.
 */
void ProcessScreenObjects(screen_objects &ScreenObjects)
{
   /**
    * Create the control points for the spline to be drawn.
    */
   if (ScreenObjects.vSpline.empty())
   {
      ScreenObjects.PStart = fluffy::math3d::Point(0, 0, 0);
      ScreenObjects.PEnd = fluffy::math3d::Point(0, 1, 0);
      ScreenObjects.SplineCtrlPoints.push_back(ScreenObjects.PStart);
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(0, 1, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(1, 1, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(1, 0, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(2, 0, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(2, 1, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(3, 1, 0));
      ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::Point(3, 0, 0));
      ScreenObjects.SplineCtrlPoints.push_back(ScreenObjects.PEnd);
   }

   {
      ScreenObjects.vSpline.clear();

      auto MatCatmRom = fluffy::math3d::SplineInitCatmullRom();

      auto &P0 = ScreenObjects.SplineCtrlPoints[0];
      auto const &P1 = ScreenObjects.SplineCtrlPoints[1];
      auto const &P2 = ScreenObjects.SplineCtrlPoints[2];
      auto const &P3 = ScreenObjects.SplineCtrlPoints[3];
      auto const &P4 = ScreenObjects.SplineCtrlPoints[4];
      auto const &P5 = ScreenObjects.SplineCtrlPoints[5];
      auto const &P6 = ScreenObjects.SplineCtrlPoints[6];
      auto const &P7 = ScreenObjects.SplineCtrlPoints[7];

      /**
       * Lerp between two points.
       */
      P0 = ScreenObjects.PStart * (1 - ScreenObjects.t) + ScreenObjects.t * ScreenObjects.PEnd;
      ScreenObjects.t += (ScreenObjects.tdirection * 0.002);

      /**
       * Handle rollover of the t.
       */
      if (ScreenObjects.t > fluffy::math3d::FLOAT(1)) ScreenObjects.tdirection = -1;
      if (ScreenObjects.t < fluffy::math3d::FLOAT(0)) ScreenObjects.tdirection = 1;

      auto Mc0 = fluffy::math3d::MultSpline(MatCatmRom, P0, P1, P2, P3);
      auto Mc1 = fluffy::math3d::MultSpline(MatCatmRom, P1, P2, P3, P4);
      auto Mc2 = fluffy::math3d::MultSpline(MatCatmRom, P2, P3, P4, P5);
      auto Mc3 = fluffy::math3d::MultSpline(MatCatmRom, P3, P4, P5, P6);
      auto Mc4 = fluffy::math3d::MultSpline(MatCatmRom, P4, P5, P6, P7);
      constexpr fluffy::math3d::FLOAT NumSplineElem = 5;

      constexpr size_t NumVal = 20;
      fluffy::math3d::FLOAT Increment = fluffy::math3d::FLOAT(1) / fluffy::math3d::FLOAT(NumVal);
      fluffy::math3d::FLOAT U{};

      for (size_t Idx = 0;  //!<
           Idx < NumVal;    //!<
           ++Idx)
      {
         ScreenObjects.vSpline.push_back(fluffy::math3d::MultSpline(U, Mc0));
         U += Increment;
      }

      U = fluffy::math3d::FLOAT(0);
      for (size_t Idx = 0;  //!<
           Idx < NumVal;    //!<
           ++Idx)
      {
         ScreenObjects.vSpline.push_back(fluffy::math3d::MultSpline(U, Mc1));
         U += Increment;
      }

      U = fluffy::math3d::FLOAT(0);
      for (size_t Idx = 0;  //!<
           Idx < NumVal;    //!<
           ++Idx)
      {
         ScreenObjects.vSpline.push_back(fluffy::math3d::MultSpline(U, Mc2));
         U += Increment;
      }

      U = fluffy::math3d::FLOAT(0);
      for (size_t Idx = 0;  //!<
           Idx < NumVal;    //!<
           ++Idx)
      {
         ScreenObjects.vSpline.push_back(fluffy::math3d::MultSpline(U, Mc3));
         U += Increment;
      }

      U = fluffy::math3d::FLOAT(0);
      for (size_t Idx = 0;  //!<
           Idx < NumVal;    //!<
           ++Idx)
      {
         ScreenObjects.vSpline.push_back(fluffy::math3d::MultSpline(U, Mc4));
         U += Increment;
      }

      /**
       * Walk along the spline segments and draw a circle.
       */
      if (ScreenObjects.t >= 0 && ScreenObjects.t < fluffy::math3d::FLOAT(1) / NumSplineElem)
      {
         auto t = NumSplineElem * ScreenObjects.t;
         ScreenObjects.SplineCtrlPoints.pop_back();
         ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::MultSpline(t, Mc0));
      }
      else if (ScreenObjects.t >= fluffy::math3d::FLOAT(1) / NumSplineElem &&
               ScreenObjects.t < fluffy::math3d::FLOAT(2) / NumSplineElem)
      {
         auto t = 4 * (ScreenObjects.t - fluffy::math3d::FLOAT(1) / NumSplineElem);
         ScreenObjects.SplineCtrlPoints.pop_back();
         ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::MultSpline(t, Mc1));
      }
      else if (ScreenObjects.t >= fluffy::math3d::FLOAT(2) / NumSplineElem &&
               ScreenObjects.t < fluffy::math3d::FLOAT(3) / NumSplineElem)
      {
         auto t = 4 * (ScreenObjects.t - fluffy::math3d::FLOAT(2) / NumSplineElem);
         ScreenObjects.SplineCtrlPoints.pop_back();
         ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::MultSpline(t, Mc2));
      }
      else if (ScreenObjects.t >= fluffy::math3d::FLOAT(3) / NumSplineElem &&
               ScreenObjects.t < fluffy::math3d::FLOAT(4) / NumSplineElem)
      {
         auto t = 4 * (ScreenObjects.t - fluffy::math3d::FLOAT(3) / NumSplineElem);
         ScreenObjects.SplineCtrlPoints.pop_back();
         ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::MultSpline(t, Mc3));
      }
      else if (ScreenObjects.t >= fluffy::math3d::FLOAT(4) / NumSplineElem &&
               ScreenObjects.t < fluffy::math3d::FLOAT(5) / NumSplineElem)
      {
         auto t = 4 * (ScreenObjects.t - fluffy::math3d::FLOAT(4) / NumSplineElem);
         ScreenObjects.SplineCtrlPoints.pop_back();
         ScreenObjects.SplineCtrlPoints.push_back(fluffy::math3d::MultSpline(t, Mc4));
      }
   }

   if (!ScreenObjects.vCubes.empty())
   {
      // auto M = fluffy::math3d::RotateZ(fluffy::math3d::Deg2Rad(0.1));
      auto &Cube = ScreenObjects.vCubes[0];
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

#if 0
   if (!ScreenObjects.vTextObjects.empty())
   {
      for (auto &TextObject : ScreenObjects.vTextObjects)
      {
         auto &CurrPos = TextObject.Position;
         CurrPos.y += 1;
         if (CurrPos.y > gScreenDimension.PixelHeight) CurrPos.y = 0;
      }
   }
#endif

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
         ScreenObjects.FpsInfo.FPS = (float)ScreenObjects.FpsInfo.FrameCount;  // fps will be the number of frames
                                                                               // rendered in the past second
         ScreenObjects.FpsInfo.Output.Text = std::to_string(int(ScreenObjects.FpsInfo.FPS)) + "fps";
         ScreenObjects.FpsInfo.FrameCount = 0;
         ScreenObjects.FpsInfo.StartTime = CurrentTime;
      }
   }
}

void Render(SDL_Surface *screenSurface, screen_objects &ScreenObjects)
{
   auto UpdateTextObjects = ScreenObjects.vSpline.empty();

   ProcessScreenObjects(ScreenObjects);

   /**
    * Set up text for the different points of the spline.
    * This needs to be done in Pixelpos?
    */
   if (UpdateTextObjects && !ScreenObjects.vSpline.empty())
   {
      fluffy::render::text_fmt TextObject{};
      TextObject.ptrFont = ScreenObjects.TextSplineInfo.ptrFont;

      auto SetPointText = [&](fluffy::math3d::matrix const &MatrixConversion,
                              std::vector<fluffy::math3d::tup> const &SplineCtrlPoints,
                              fluffy::render::text_fmt &TextObject, size_t Idx) -> void
      {
         auto const NumElem = SplineCtrlPoints.size();
         if (Idx >= NumElem) return;

         TextObject.Text = "P" + std::to_string(Idx);
         auto V = MatrixConversion * SplineCtrlPoints[Idx];
         TextObject.Position.x = V.X;
         TextObject.Position.y = V.Y;
         ScreenObjects.vTextObjects.push_back(TextObject);
      };

      /**
       * Create the PX text for all but the last element, hence the -1.
       */
      for (size_t Idx = 0;                                   //!<
           Idx < ScreenObjects.SplineCtrlPoints.size() - 1;  //!<
           ++Idx                                             //!<
      )
      {
         SetPointText(ScreenObjects.MatrixConversion, ScreenObjects.SplineCtrlPoints, TextObject, Idx);
      }
   }

   /**
    * Do the actual printing of the Text objects.
    */
   for (auto &TextObject : ScreenObjects.vTextObjects)
   {
      fluffy::render::Text(screenSurface, TextObject);
   }

   {
      /**
       * Draw the spline and the corresponding control points.
       * 1. A lambda for ease of drawing.
       * 2. The points on the spline.
       * 3. The control points.
       */

      auto PlotPoint = [&](fluffy::math3d::tup const &Point, int Color = 0xFFFFFF, int RadiusInPixels = 2) -> void
      {
         auto V = ScreenObjects.MatrixConversion * Point;
         fluffy::render::vertice_2d Vert{V.X, V.Y};
         fluffy::render::DrawCircle(screenSurface, Vert, RadiusInPixels, Color, NoColorGradient);
      };

      /**
       * Draw the actual spline between the points.
       */
      for (auto const &Point : ScreenObjects.vSpline)
      {
         PlotPoint(Point, 0xFF);
      }

      /**
       * Draw the control points to show how the spline is pulled and pushed.
       */
      {
         PlotPoint(ScreenObjects.PStart, 0x00FF00, 10);
         PlotPoint(ScreenObjects.PEnd, 0x00FF00, 10);

         auto const NumElem = ScreenObjects.SplineCtrlPoints.size();
         size_t Idx{};
         while (Idx < NumElem)
         {
            PlotPoint(ScreenObjects.SplineCtrlPoints[Idx], 0xAABBCC, 10);
            ++Idx;
         }
      }
   }

/**
 */
#if 0
   /**
    * Draw all the cubes.
    */
   for (auto &Cube : ScreenObjects.vCubes)
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
         auto &BaseTO = ScreenObjects.vTextObjects[0];
         fluffy::render::text_fmt PosInfo{};
         PosInfo.ptrFont = BaseTO.ptrFont;
         PosInfo.Position.x = Cube.Pixel[7].X;
         PosInfo.Position.y = Cube.Pixel[7].Y;
         auto strX = std::to_string(PosInfo.Position.x);
         auto strY = std::to_string(PosInfo.Position.y);
         PosInfo.Text = "(" + strX + "," + strY + ")";
         fluffy::render::Text(screenSurface, PosInfo);
         // TTF_SetFontSizeDPI(PosInfo.ptrFont, 14, 140, 140);
      }
   }
#endif

   /**
    * Display the FPS.
    */
   fluffy::render::Text(screenSurface, ScreenObjects.FpsInfo.Output);
}
};  // namespace

//-----------------------------------------------------------------------------
int main(int argc, char *args[])
{
   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

#ifdef __APPLE__
   constexpr Uint32 FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_METAL;
#else
   constexpr Uint32 FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
#endif

   SDL_Window *ptrWindow =
       SDL_CreateWindow("SDL Pixel Drawing", screen_dimension::SCREEN_WIDTH, screen_dimension::SCREEN_HEIGHT, FLAGS);
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
      SDL_GetWindowSize(ptrWindow, &gScreenDimension.Width, &gScreenDimension.Height);
      SDL_GetWindowSizeInPixels(ptrWindow, &gScreenDimension.PixelWidth, &gScreenDimension.PixelHeight);
      SDL_Log(
          "Window size reported is : Width:%i Height: %i, PixelWidth: %i "
          "PixelHeight: %i",
          gScreenDimension.Width, gScreenDimension.Height, gScreenDimension.PixelWidth, gScreenDimension.PixelHeight);

      if (gScreenDimension.PixelHeight > gScreenDimension.Height)
      {
         FontAdjustment = fluffy::math3d::FLOAT(gScreenDimension.PixelHeight) / gScreenDimension.Height;
      }
   }

   /**
    * Load a font from the resources directory.
    */
   char const *ptrFontName = "SourceCodePro-Regular.ttf";
   std::string FontName(ptrFontName);
   FontName = ScreenObjects.ResourcePath + "/TTF/" + FontName;

   /**
    * Load the font and try to adjust to the high dpi display if possible.
    */
   int PointSize = static_cast<int>(14 * FontAdjustment);
   TTF_Font *ptrFont = TTF_OpenFont(FontName.c_str(), PointSize);
   if (ptrFont == nullptr)
   {
      SDL_Log("TTF_OpenFont: Failed to load  font %s. Error: %s.", ptrFontName, TTF_GetError());
      SDL_Quit();
      return 1;
   }
   SDL_Log("TTF_OpenFont:  Loaded  font %s with size %i.", FontName.c_str(), PointSize);

   ScreenObjects.FpsInfo.Output.ptrFont = ptrFont;

   SDL_Color textColor = {255, 255, 255};
   SDL_Surface *ptrTextSurface = TTF_RenderUTF8_Solid(ptrFont, "Hello, SDL!", textColor);
   if (ptrTextSurface == nullptr)
   {
      SDL_Log("TTF_RenderText_Solid: %s.", TTF_GetError());
      SDL_Quit();
      return 1;
   }

   // Get window surface
   SDL_Surface *ptrScreenSurface = SDL_GetWindowSurface(ptrWindow);
   if (!ptrScreenSurface)
   {
      SDL_Log("Could not get window surface! SDL_Error: %s.", SDL_GetError());
      SDL_DestroyWindow(ptrWindow);
      SDL_Quit();
      return 1;
   }

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

   ScreenObjects.Projection = fluffy::render::Projection(gScreenDimension.PixelWidth, gScreenDimension.PixelHeight,
                                                         fluffy::math3d::Deg2Rad(ScreenObjects.FOV), -10, 100);
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
   }
   ScreenObjects.vCubes.push_back(Cube);

   /**
    * Copy the font pointer into an object in ScreenObjects for reference only.
    */
   ScreenObjects.TextSplineInfo.ptrFont = ptrFont;

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

         constexpr fluffy::math3d::FLOAT FPS = 61;
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

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
