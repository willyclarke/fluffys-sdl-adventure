#ifndef SRC_LIB_SPLINES_HPP_ED5915BA_7F16_424D_88A9_82923C75498B
#define SRC_LIB_SPLINES_HPP_ED5915BA_7F16_424D_88A9_82923C75498B

/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include "fluffymath.hpp"

#include <memory>
#include <vector>

namespace fluffy
{
namespace splines
{
struct spline_catmull_rom
{
   /**
    * The control points make up the path for the spline.
    * Whenever a control point changes the Catmull Rom
    * matrix need to be updated.
    */
   std::vector<fluffy::math3d::tup> CtrlPoints{};

   /**
    * Vector containing points after doing the calculations
    * based on the control points and the Catmull Rom matrix's.
    * This is considered the output of the CatmullRom spline
    * function.
    * NOTE: This may have to go. FIXME: (Willy Clarke) Check if this is needed.
    */
   struct point
   {
      fluffy::math3d::tup P{};
      fluffy::math3d::tup Col{1, 1, 1, 0};
      fluffy::math3d::FLOAT t{};
      std::size_t Idx{};
   };
   std::vector<point> vSpline{};

   /**
    * A vector containing the final matrix for each segment on the spline curve.
    * The Catmull Rom spline requires four points per segment.
    */
   std::vector<fluffy::math3d::matrix> vMatSeg{};
};

/**
 * Initialize the struct for a Catmull Rom spline.
 * Copies the input vector and creates the section matrix's.
 * Two points are added. One for to start the spline and one to end the spline.
 * The added points are the result of a vector mirrored from the first two and
 * the last two points of the spline.
 */
auto InitCatmullRom(std::vector<fluffy::math3d::tup> const &vP) -> spline_catmull_rom;
auto SplineValueCatmullRom(spline_catmull_rom const &Spline, fluffy::math3d::FLOAT t)
    -> fluffy::splines::spline_catmull_rom::point;
auto SplineTestCatmullRom(fluffy::math3d::FLOAT Xoffs = 2,  //!<
                          fluffy::math3d::FLOAT Yoffs = 2,  //!<
                          fluffy::math3d::FLOAT Zoffs = 0   //!<
                          ) -> spline_catmull_rom;

};  // end of namespace splines
};  // end of namespace fluffy
#endif

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
