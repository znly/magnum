#ifndef Magnum_Animation_Clip_h
#define Magnum_Animation_Clip_h
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
 * @brief Class @ref Magnum::Animation::Clip
 */

#include <Corrade/Containers/Array.h>

#include "Magnum/Magnum.h"
#include "Magnum/Animation/Animation.h"

namespace Magnum { namespace Animation {

/**
@brief Animation clip
@tparam Target_ Animation target enum type
@tparam Frame_  Frame reference type

Immutable storage of animation track views of unique targets. Does not own the
referenced tracks, so the user has to ensure that the tracks do not go out of
scope for the whole lifetime of the clip.
@experimental
*/
template<class Target_, class Frame_> class Clip {
    public:
        typedef Target_ Target;
        typedef Frame_ Frame;

        /**
         * @brief Constructor
         * @param tracks    Tracks contained in this clip
         * @param start     Clip start frame
         * @param end       Clip end frame
         *
         * Expects that all @p tracks have unique target/index combination,
         * @p start is not larger than any keyframe and @p end is not smaller
         * than any keyframe.
         */
        explicit Clip(Containers::Array<TrackRef<Target, Frame>>&& tracks, Frame start, Frame end) noexcept: _tracks{std::move(tracks)}, _start{start}, _end{end} {}

        /** @overload
         * Equivalent to the above with @p start and @p end calculated from the
         * track contents.
         */
        explicit Clip(Containers::Array<TrackRef<Target, Frame>>&& tracks) noexcept;

        /**
         * @brief Clip start frame
         *
         * Animation values in tracks which have the first keyframe larger than
         * the start frame are extrapolated according to
         * @ref AnimationTrack::extrapolation().
         */
        Frame start() const { return _start; }

        /**
         * @brief Clip end frame
         *
         * Animation values in tracks which have the first keyframe smaller
         * than the start frame are extrapolated according to
         * @ref AnimationTrack::extrapolation().
         */
        Frame end() const { return _end; }

        /** @brief Track list */
        Containers::ArrayView<const TrackRef<Target, Frame>> tracks() const { return _tracks; }

    private:
        Containers::Array<TrackRef<Target, Frame>> _tracks;
        Frame _start, _end;
};

}}

#endif
