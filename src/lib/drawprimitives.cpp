/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <iostream>
#include <vector>

#include "drawprimitives.hpp"

namespace fluffy
{
namespace render
{
//-----------------------------------------------------------------------------
auto DrawLine(SDL_Surface* screenSurface,            //!<
              fluffy::render::vertice_2d const& V0,  //!<
              fluffy::render::vertice_2d const& V1,  //!<
              Uint32 Color,                          //!<
              bool UseColorGradient                  //!<
              )                                      //!<
    -> void
{
   /* Compute number of pixels based on line length. */
   auto NumPixels = fluffy::render::Length(V0, V1);
   if (NumPixels < fluffy::math3d::FLOAT(1)) return;

   fluffy::math3d::FLOAT Idx{};

   while (Idx < NumPixels)
   {
      auto P = V0 + fluffy::render::Vector(V0, V1) * (Idx / NumPixels);

      auto X = static_cast<int>(P.X);
      auto Y = static_cast<int>(P.Y);
      if (UseColorGradient)
      {
         auto Alfa = Idx / NumPixels;
         auto Gamma = 1 - Idx / NumPixels;
         auto Beta = std::abs(Alfa - Gamma);
         Color = uint8_t(Alfa * 0xFF) << 16 | uint8_t(Beta * 0xFF) << 8 | uint8_t(Gamma * 0xFF);
      }

      /**
       * Check that the pixel is inside the rectangle. Stop drawing when this happens.
       */
      if (X > screenSurface->clip_rect.w || Y > screenSurface->clip_rect.h) break;
      if (X < 0 || Y < 0) break;

      ((Uint32*)screenSurface->pixels)[Y * screenSurface->w + X] = Color;
      ++Idx;
   }
}

//------------------------------------------------------------------------------
auto DrawCircle(SDL_Surface* screenSurface,                //!<
                fluffy::render::vertice_2d const& Center,  //!<
                fluffy::math3d::FLOAT Radius,              //!<
                Uint32 Color,                              //!<
                bool UseColorGradient                      //!<
                )                                          //!<
    -> void
{
   auto NumPixels = Radius * fluffy::math3d::FLOAT(2) * M_PI;
   if (NumPixels < fluffy::math3d::FLOAT(1)) return;

   auto AngleDelta = fluffy::math3d::FLOAT(1) / NumPixels;
   fluffy::math3d::FLOAT Angle{};
   fluffy::render::vertice_2d V1 = Center + fluffy::render::vertice_2d{Radius, fluffy::math3d::FLOAT(0)};

   constexpr fluffy::math3d::FLOAT STOPANGLE = fluffy::math3d::FLOAT(2) * M_PI;
   while (Angle < STOPANGLE)
   {
      V1 = fluffy::render::Rotate(Center, V1, AngleDelta);

      auto X = static_cast<int>(V1.X);
      auto Y = static_cast<int>(V1.Y);

      /**
       * Check that the pixel is inside the rectangle. Stop drawing when this happens.
       */
      if (X > screenSurface->clip_rect.w || Y > screenSurface->clip_rect.h) break;
      if (X < 0 || Y < 0) break;

      if (UseColorGradient)
      {
         DrawLine(screenSurface, Center, V1, Color, UseColorGradient);
      }
      ((Uint32*)screenSurface->pixels)[Y * screenSurface->w + X] = Color;

      Angle += AngleDelta;
   }
}

//-----------------------------------------------------------------------------
auto Text(SDL_Surface* screenSurface,  //!<
          text_fmt& TextFmt            //!<
          )                            //!<
    -> void
{
   if (TextFmt.Text.empty()) return;

   if (TextFmt.ptrFont == nullptr) return;

   if (TextFmt.Dirty && TextFmt.ptrSurface)
   {
      SDL_DestroySurface(TextFmt.ptrSurface);
      TextFmt.ptrSurface = nullptr;
   }

   if (TextFmt.ptrSurface == nullptr)
   {
      TextFmt.ptrSurface = TTF_RenderUTF8_Solid(TextFmt.ptrFont, TextFmt.Text.c_str(), TextFmt.Color);
   }

   if (TextFmt.ptrSurface)
   {
      SDL_BlitSurface(TextFmt.ptrSurface, NULL, screenSurface, &TextFmt.Position);
   }
}

//------------------------------------------------------------------------------
std::string GetResourcePath(const std::string& subDir = "")
{
   // Use SDL_GetBasePath() to get the base resource path
   std::string ResourceBasePath{};

   char* basePath = SDL_GetBasePath();
   if (basePath)
   {
      std::string StrBasePath(basePath);
      std::cout << __PRETTY_FUNCTION__ << " -> basePath:" << StrBasePath << std::endl;
      ResourceBasePath = StrBasePath;

      SDL_free(basePath);
   }
   else
   {
      // Error handling: you might want to use SDL_GetError() for logging
      SDL_Log("Could not find ResourceBasePath! SDL_Error: %s.", SDL_GetError());
      return "";
   }

   // We'll assume the assets are in the same path as the compiled program,
   // up one directory, and inside a directory named "resources".
   size_t pos = ResourceBasePath.rfind("bin");
   if (pos == std::string::npos)
   {
      SDL_Log("Could not find 'bin' in in path.");
      ResourceBasePath += "../resources/";
   }
   else
   {
      ResourceBasePath = ResourceBasePath.substr(0, pos) + "resources/";
   }

   // If a subdirectory is specified, append it to the base path
   return subDir.empty() ? ResourceBasePath : ResourceBasePath + subDir + "/";
}
};  // end of namespace render
};  // end of namespace fluffy

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
