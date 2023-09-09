/**
 * License : MIT. See bottom of file.
 * Copyright : Willy Clarke.
 */

#include <cmath>
#include <iostream>
#include <vector>

#include "splines.hpp"

namespace fluffy
{
namespace splines
{
auto InitCatmullRom(std::vector<fluffy::math3d::tup> const &vP) -> spline_catmull_rom
{
   if (vP.size() < 2) return {};

   spline_catmull_rom Spline{};

   Spline.CtrlPoints = vP;

   /**
    * Get a Control Points reference.
    */
   auto &CP = Spline.CtrlPoints;

   /**
    * Create two additional control points for the start and the
    * end of the spline.
    */

   /**
    * The first point is calculated by reversing the vector that
    * originates between P0 and P1.
    */
   auto Vector0 = (CP[1] - CP[0]) * fluffy::math3d::FLOAT(-1) / fluffy::math3d::FLOAT(3);
   auto PointBegin = CP[0] + Vector0;
   CP.insert(CP.begin(), PointBegin);

   /**
    * The second point is again calculated as a vector originating from the last
    * two control points given.
    */
   auto &PointEnd = CP[CP.size() - 1];
   auto &PointEndMinus1 = CP[CP.size() - 2];
   auto VectorEnd = (PointEndMinus1 - PointEnd) * fluffy::math3d::FLOAT(-1) / fluffy::math3d::FLOAT(3);
   CP.push_back(VectorEnd + PointEnd);

   /**
    * Iterate over the points to create the segment matrix for each segment.
    */
   auto MatCatmRom = fluffy::math3d::SplineMatrixCatmullRom();

   for (size_t Idx = 0;       //!<
        Idx < CP.size() - 3;  //!<
        ++Idx                 //!<
   )
   {
      auto const &P0 = CP.at(Idx + 0);
      auto const &P1 = CP.at(Idx + 1);
      auto const &P2 = CP.at(Idx + 2);
      auto const &P3 = CP.at(Idx + 3);
      auto const MSegment = fluffy::math3d::MultSpline(MatCatmRom, P0, P1, P2, P3);
      Spline.vMatSeg.push_back(MSegment);
   }

   return Spline;
}

/**
 * Return the spline point at the point (in time?) t.
 */
auto SplineValueCatmullRom(spline_catmull_rom const &Spline, fluffy::math3d::FLOAT t)
    -> fluffy::splines::spline_catmull_rom::point
{
   if (t < 0) return {};
   if (t > 1) return {};
   if (Spline.vMatSeg.empty()) return {};

   spline_catmull_rom::point SplineValue{};

   /**
    * Compute the index into the vMatSeg based on the t-value.
    */
   auto const tSpaced = fluffy::math3d::FLOAT(1) / Spline.vMatSeg.size();

   SplineValue.Idx = t < (tSpaced - fluffy::math3d::EPSILON)
                         ? 0
                         : static_cast<std::size_t>(std::floor((t - fluffy::math3d::EPSILON) / tSpaced));

   /**
    * Get the point value at t by computing a value u that corresponds to the t
    * for a specific segment.
    */

   auto Normalize = [](fluffy::math3d::FLOAT t,        //!<
                       fluffy::math3d::FLOAT tSpaced,  //!<
                       std::size_t Idx                 //!<
                       ) -> fluffy::math3d::FLOAT
   {
      auto tStart = tSpaced * Idx;
      auto u = t - tStart;
      auto N = u / tSpaced;
      return N;
   };

   auto const u = Normalize(t, tSpaced, SplineValue.Idx);

   SplineValue.t = t;

   SplineValue.P = fluffy::math3d::MultSpline(u, Spline.vMatSeg[SplineValue.Idx]);

   return SplineValue;
}

/**
 * Create a test spline for ease of debugging.
 */
auto SplineTestCatmullRom(fluffy::math3d::FLOAT Xoffs,  //!<
                          fluffy::math3d::FLOAT Yoffs,  //!<
                          fluffy::math3d::FLOAT Zoffs   //!<
                          ) -> spline_catmull_rom
{
   std::vector<fluffy::math3d::tup> vP{};

   /**
    * Create the beginning of a square wave.
    */

   vP.push_back(fluffy::math3d::Point(-1 + Xoffs, 0 + Yoffs, 0 + Zoffs));
   vP.push_back(fluffy::math3d::Point(0 + Xoffs, 0 + Yoffs, 0 + Zoffs));
   vP.push_back(fluffy::math3d::Point(0 + Xoffs, -1 + Yoffs, 0 + Zoffs));
   vP.push_back(fluffy::math3d::Point(1 + Xoffs, -1 + Yoffs, 0 + Zoffs));
   vP.push_back(fluffy::math3d::Point(1 + Xoffs, 0 + Yoffs, 0 + Zoffs));
   vP.push_back(fluffy::math3d::Point(2 + Xoffs, 0 + Yoffs, 0 + Zoffs));

   auto Spline = fluffy::splines::InitCatmullRom(vP);

   constexpr fluffy::math3d::FLOAT Deltat = 0.001;
   fluffy::math3d::FLOAT t = 0;

   while (t < fluffy::math3d::FLOAT(1))
   {
      auto const SplineValue = fluffy::splines::SplineValueCatmullRom(Spline, t);
      Spline.vSpline.push_back(SplineValue);
      t += Deltat;
   }

   return Spline;
}
};  // namespace splines
};  // namespace fluffy

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
