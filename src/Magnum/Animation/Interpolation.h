#ifndef Magnum_Animation_Interpolation_h
#define Magnum_Animation_Interpolation_h
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

#include "Magnum/Animation/Animation.h"

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

template<class Kind, class Frame> Vector2 linearInterpolator(const TrackBase<Result>& track, std::size_t i, std::size_t j, Float t) {
    return Math::lerp(reinterpret_cast<const Track<Kind, Frame>&>(track)[i], reinterpret_cast<const Track<Kind, Frame>&>(track)[j]);
}

template<class Kind, class Frame> Vector2 bezierInterpolator(const TrackBase<Result>& track, std::size_t i, std::size_t j, Float t) {
    return reinterpret_cast<const Track<Kind, Frame>&>(track)[i].interpolate(t);
}

}}

#endif
