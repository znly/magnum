#ifndef Magnum_Animation_Interpolator_h
#define Magnum_Animation_Interpolator_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Typedef @ref Magnum::Animation::Interpolator
 */

#include "Magnum/Math/Bezier.h"
#include "Magnum/Math/Functions.h"
#include "Magnum/Animation/Track.h"

namespace Magnum { namespace Animation {

/**
@brief Interpolation function type

Takes @p track, casts it to a concrete @ref AnimationTrack type and
interpolates values at @p i and @p j using @p t. Can assume that both @p i and
@p j are in bounds and immediately next to each other.
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class Result> using Interpolator = Result(*)(const TrackBase<Result>& track, std::size_t i, std::size_t j, Float t);
#endif

template<class Target, class Frame, class T> T constantInterpolator(const TrackBase<T>& track, std::size_t i, std::size_t, Float) {
    return reinterpret_cast<const Track<Target, Frame, T>&>(track)[i];
}

template<class Target, class Frame, std::size_t size, class T> Math::Vector<size, T> linearInterpolator(const TrackBase<Math::Vector<size, T>>& track, std::size_t i, std::size_t j, Float t) {
    return Math::lerp(reinterpret_cast<const Track<Target, Frame, Math::Vector<size, T>>&>(track)[i],
                      reinterpret_cast<const Track<Target, Frame, Math::Vector<size, T>>&>(track)[j]);
}

template<class Target, class Frame, UnsignedInt order, UnsignedInt dimensions, class T> Math::Vector<dimensions, T> bezierInterpolator(const TrackBase<Math::Vector<dimensions, T>>& track, std::size_t i, std::size_t, Float t) {
    return reinterpret_cast<const Track<Target, Frame, Math::Bezier<order, dimensions, T>>&>(track)[i].interpolate(t);
}

}}

#endif
