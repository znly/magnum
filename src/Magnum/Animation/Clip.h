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

#include "Magnum/Animation/Animation.h"

namespace Magnum { namespace Animation {

/**
@brief Animation clip
@tparam Kind    Animation kind enum type
@tparam Frame   Frame reference type

Immutable storage of animation track views of unique kinds. Does not own the
referenced tracks, so the user has to ensure that the tracks do not go out of
scope for the whole lifetime of the clip.
@experimental
*/
template<class Kind, class Frame> class Clip {
    public:
        /**
         * @brief Constructor
         * @param tracks    Tracks contained in this clip
         * @param start     Clip start frame
         * @param end       Clip end frame
         *
         * Expects that all @p tracks have unique kinds, @p start is not larger
         * than any keyframe and @p end is not smaller than any keyframe.
         */
        explicit Clip(Containers::Array<TrackView<Kind, Frame>>&& tracks, Frame start, Frame end) noexcept;

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
        Containers::ArrayView<const TrackView<Kind, Frame>> tracks() const { return _tracks; }

    private:
        Frame _start, _end;
        Containers::Array<TrackView<Kind, Frame>> _tracks;
};

}

#endif
