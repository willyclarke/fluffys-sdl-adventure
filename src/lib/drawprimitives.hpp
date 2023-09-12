#ifndef DRAWPRIMITIVES_HPP_A89F04E4_080C_4054_9288DCF5CFA31525
#define DRAWPRIMITIVES_HPP_A89F04E4_080C_4054_9288DCF5CFA31525

/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <string>
#include <vector>

#include "fluffymath.hpp"
#include "triangle2d.hpp"

namespace fluffy
{
namespace render
{
struct text_fmt
{
   text_fmt(){};

   ~text_fmt()
   {
      if (ptrSurface)
      {
         SDL_DestroySurface(ptrSurface);
         ptrSurface = nullptr;
      }
   };

   std::string Text{};                //!< Consider using a buffer to avoid runtime memory allocations.
   SDL_Rect Position{};               //!<
   SDL_Color Color{255, 255, 255};    //!<
   SDL_Surface* ptrSurface{nullptr};  //!< Text() will allocate as needed.
   TTF_Font* ptrFont{nullptr};        //!< Must point to valid font when using the Text() function.
   bool Dirty{true};                  //!< Set when text has changed.
   bool UseColorGradient{};           //!<
};

//-----------------------------------------------------------------------------
auto DrawLine(SDL_Surface* screenSurface,            //!<
              fluffy::render::vertice_2d const& V0,  //!<
              fluffy::render::vertice_2d const& V1,  //!<
              Uint32 Color,                          //!<
              bool UseColorGradient                  //!<
              )                                      //!<
    -> void;

//------------------------------------------------------------------------------
auto DrawCircle(SDL_Surface* screenSurface,                //!<
                fluffy::render::vertice_2d const& Center,  //!<
                fluffy::math3d::FLOAT Radius,              //!<
                Uint32 Color,                              //!<
                bool UseColorGradient                      //!<
                )                                          //!<
    -> void;

//-----------------------------------------------------------------------------
/**
 * Write text on the screenSurface according to the text_fmt.
 * NOTE: There are side effects when using this function with
 *       respect to the Dirty flag. When set the text_fmt object
 *       will be updated.
 */
auto Text(SDL_Surface* screenSurface,  //!<
          text_fmt& TextFmt            //!<
          )                            //!<
    -> void;

//-----------------------------------------------------------------------------
std::string GetResourcePath(const std::string& subDir);
uint32_t LerpColor(uint32_t Color1, uint32_t Color2, float t);

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
#endif  // DRAWPRIMITIVES_HPP_A89F04E4_080C_4054_9288DCF5CFA31525
