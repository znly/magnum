#ifndef Magnum_Animation_Track_h
#define Magnum_Animation_Track_h
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
 * @brief Class @ref Magnum::Animation::Track, @ref Magnum::Animation::TrackView, alias @ref Magnum::Animation::ResultOf
 */

#include <Corrade/Containers/Array.h>

#include "Magnum/Magnum.h"
#include "Magnum/Animation/Animation.h"

namespace Magnum { namespace Animation {

namespace Implementation {
    template<class T> struct TypeTraits {
        typedef T ResultType;
    };
    template<UnsignedInt order, UnsignedInt dimensions, class T> struct TypeTraits<Math::Bezier<order, dimensions, T>> {
        typedef Math::Vector<dimensions, T> ResultType;
    };
}

/**
@brief Animation result type for given value type

Result of interpolating two `T` values (for example interpolating two
@ref Color3 values gives a @ref Color3, but interpolating two @ref CubicBezier2D
values gives a @ref Vector2).
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using ResultOf = typename Implementation::TypeTraits<T>::ResultType;
#endif

/**
@brief Animation extrapolation behavior

Describes what value is returned for frames outside of keyframe range for given
track (frame lower than first keyframe or frame larger or equal to last
keyframe).
@see @ref Track, @ref Track::at()
@experimental
*/
enum class Extrapolation: UnsignedShort {
    /** Default-constructed value is returned. */
    DefaultConstructed,

    /**
     * Value of first/last keyframe is used. In other words, for the first
     * keyframe the interpolator is called with first two keyframes and
     * interpolation factor set to `0.0f`; for the last keyframe the
     * interpolator is called with last two keyframes and interpolation factor
     * set to `1.0f`. In case there is only one keyframe, it's passed to both
     * inputs of the interpolator.
     */
    Constant,

    /**
     * Values of first two / last two keyframes are extrapolated. In case
     * there is only one keyframe, it's passed to both inputs of the
     * interpolator.
     */
    Extrapolated

    /** @todo repeat? that would duplicate the play count feature though */
};

/**
@brief Base for animation tracks
@tparam Result  Result type of interpolated animation value

Base for tracks independent on the actual keyframe value stored.
*/
template<class Result> class TrackBase {};

/**
@brief Animation track
@tparam Kind    Animation kind enum type
@tparam Frame   Frame reference type
@tparam T       Animated value

Immutable storage of keyframe + value pairs.
@see @ref TrackView, @ref Clip
@experimental
*/
template<class Kind, class Frame, class T> class Track: public TrackBase<ResultOf<T>> {
    public:
        /** @brief Interpolation function */
        typedef Animation::Interpolator<ResultOf<T>> Interpolator;

        /**
         * @brief Constructor
         * @param kind          Animation kind
         * @param data          Keyframe data
         * @param interpolator  Interpolation function
         * @param extrapolation Extrapolation behavior
         *
         * The keyframe data are assumed to be stored in sorted order. It's not
         * an error to have two successive keyframes with the same frame value.
         */
        explicit Track(Kind kind, Containers::Array<std::pair<Frame, T>>&& data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant) noexcept;

        /** @brief Copying is not allowed */
        Track(const Track<Kind, Frame, T>&) = delete;

        /** @brief Move constructor */
        Track(Track<Kind, Frame, T>&&) = default;

        /** @brief Copying is not allowed */
        Track<Kind, Frame, T>& operator=(const Track<Kind, Frame, T>&) = delete;

        /** @brief Move constructor */
        Track<Kind, Frame, T>& operator=(Track<Kind, Frame, T>&&) = default;

        /** @brief Animation kind */
        Kind kind() const { return _kind; }

#error also an index

        /**
         * @brief Extrapolation behavior
         *
         * @see @ref at(), @ref Clip::start(), @ref Clip::end()
         */
        Extrapolation extrapolation() const { return _extrapolation; }

        /** @brief Interpolation function */
        Interpolator interpolator() const { return _interpolator; }

        /** @brief Keyframe data */
        Containers::ArrayView<const std::pair<Frame, T>> data() const { return _data; }

        /** @brief Keyframe access */
        const std::pair<Frame, T>& operator[](std::size_t i) const { return _data[i]; }

        /**
         * @brief Animated value at given time
         *
         * Does a linear search over the keyframes until it finds last keyframe
         * which is not larger than @p frame. The @p hint parameter hints where
         * to start the linear search. If no frame larger than @p frame is
         * found, the search is restarted from the beginning. Once the keyframe
         * is found, reference to it and the immediately following keyframe is
         * passed to @ref interpolator() along with calculated interpolation
         * factor. In case the first keyframe is already larger than @p frame
         * or @p frame is larger or equal to the last keyframe, either the
         * first two or last two keyframes are used and value is extrapolated
         * according to @ref extrapolation().
         */
        ResultOf<T> at(Frame frame, std::size_t& hint) const;

    private:
        Kind _kind;
        Extrapolation _extrapolation;
        Interpolator _interpolator;
        Containers::Array<std::pair<Frame, T>> _data;
};

/**
@brief Animation track view
@tparam Kind    Animation kind enum type
@tparam Frame   Frame reference type

Copyable type-erased view onto an animation track. Does not own the viewed
track, so the user has to ensure the track does not go out of scope for the
whole view lifetime.
@see @ref
@experimental
*/
template<class Kind, class Frame> class TrackView {
    public:
        /**
         * @brief Constructors
         *
         * Creates view onto the full track.
         */
        template<class T> /*implicit*/ TrackView(const Track<Kind, Frame, T>& track) noexcept: _kind{track.kind()}, _track{&track} {}

        /** @brief Animation kind */
        Kind kind() const { return _kind; }

        /**
         * @brief Value at given time
         *
         * Calls @ref Track::at() on the referenced track.
         * @todo make slicing possible here
         */
        template<class T> ResultOf<T> at(Frame frame, std::size_t& hint) const;

    private:
        Kind _kind;
        void* _track;
};

}}

#endif
