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
#include "Magnum/visibility.h"
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

/** @debugoperatorenum{Extrapolation} */
MAGNUM_EXPORT Debug& operator<<(Debug& debug, Extrapolation value);

/**
@brief Base for animation tracks
@tparam Result  Result type of interpolated animation value

Base for tracks independent on the actual keyframe value stored.
*/
template<class Result> class TrackBase {};

/**
@brief Animation track view
@tparam Target_ Animation target enum type
@tparam Frame_  Frame reference type
@tparam T       Animated value

View onto keyframe + value pairs. Does not own the keyframe data, see
@ref Track for an alternative.
@see @ref Clip
@experimental
*/
template<class Target_, class Frame_, class T> class TrackView: public TrackBase<ResultOf<T>> {
    public:
        /** @brief Animation target enum type */
        typedef Target_ Target;

        /** @brief Frame reference type */
        typedef Frame_ Frame;

        /** @brief Animated value */
        typedef T Type;

        /** @brief Interpolation function */
        typedef Animation::Interpolator<ResultOf<T>> Interpolator;

        /**
         * @brief Constructor
         * @param target        Animation target
         * @param data          Keyframe data
         * @param interpolator  Interpolation function
         * @param before        Extrapolation behavior
         * @param after         Extrapolation behavior after
         *
         * The keyframe data are assumed to be stored in sorted order. It's not
         * an error to have two successive keyframes with the same frame value.
         */
        constexpr explicit TrackView(Target target, std::size_t index, Containers::ArrayView<const std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation before, Extrapolation after) noexcept: _target{target}, _index{index}, _interpolator{interpolator}, _before{before}, _after{after}, _data{data} {}

        /** @overload
         * Equivalent to calling @ref TrackView(Target, std::size_t, Containers::ArrayView<const std::pair<Frame, T>>, Interpolator, Extrapolation, Extrapolation)
         * with both @p before and @p after set to @p extrapolation.
         */
        constexpr explicit TrackView(Target target, std::size_t index, Containers::ArrayView<const std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant) noexcept: TrackView<Target, Frame, T>{target, index, std::move(data), interpolator, extrapolation, extrapolation} {}

        /** @overload
         * Equivalent to calling @ref TrackView(Target, std::size_t, Containers::ArrayView<const std::pair<Frame, T>>, Interpolator, Extrapolation, Extrapolation)
         * with @p index set to `0`.
         */
        constexpr explicit TrackView(Target target, Containers::ArrayView<const std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation before, Extrapolation after) noexcept: TrackView<Target, Frame, T>{target, 0, std::move(data), interpolator, before, after} {}

        /** @overload
         * Equivalent to calling @ref TrackView(Target, std::size_t, Containers::ArrayView<const std::pair<Frame, T>>, Interpolator, Extrapolation, Extrapolation)
         * with @p index set to `0` and both @p before and @p after set to
         * @p extrapolation.
         */
        constexpr explicit TrackView(Target target, Containers::ArrayView<const std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant) noexcept: TrackView<Target, Frame, T>{target, 0, std::move(data), interpolator, extrapolation, extrapolation} {}

        /** @brief Animation target */
        Target target() const { return _target; }

        /** @brief Animation target index */
        std::size_t index() const { return _index; }

        /**
         * @brief Extrapolation behavior before first keyframe
         *
         * @see @ref after(), @ref at(), @ref Clip::start(), @ref Clip::end()
         */
        Extrapolation before() const { return _before; }

        /**
         * @brief Extrapolation behavior after last keyframe
         *
         * @see @ref before(), @ref at(), @ref Clip::start(), @ref Clip::end()
         */
        Extrapolation after() const { return _after; }

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
        Target _target;
        std::size_t _index;
        Interpolator _interpolator;
        Extrapolation _before, _after;
        Containers::ArrayView<const std::pair<Frame, T>> _data;
};

/**
@brief Animation track
@tparam Target_ Animation target enum type
@tparam Frame_  Frame reference type
@tparam T       Animated value

Immutable storage of keyframe + value pairs.
@see @ref TrackView, @ref TrackRef, @ref Clip
@experimental
*/
template<class Target_, class Frame_, class T> class Track: public TrackBase<ResultOf<T>> {
    public:
        /** @brief Animation target enum type */
        typedef Target_ Target;

        /** @brief Frame reference type */
        typedef Frame_ Frame;

        /** @brief Animated value */
        typedef T Type;

        /** @brief Interpolation function */
        typedef Animation::Interpolator<ResultOf<T>> Interpolator;

        /**
         * @brief Constructor
         * @param target        Animation target
         * @param data          Keyframe data
         * @param interpolator  Interpolation function
         * @param before        Extrapolation behavior
         * @param after         Extrapolation behavior after
         *
         * The keyframe data are assumed to be stored in sorted order. It's not
         * an error to have two successive keyframes with the same frame value.
         */
        explicit Track(Target target, std::size_t index, Containers::Array<std::pair<Frame, T>>&& data, Interpolator interpolator, Extrapolation before, Extrapolation after) noexcept: _target{target}, _index{index}, _interpolator{interpolator}, _before{before}, _after{after}, _data{std::move(data)} {}

        /** @overload */
        explicit Track(Target target, std::size_t index, std::initializer_list<std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation before, Extrapolation after): Track{target, index, Containers::Array<std::pair<Frame, T>>{Containers::InPlaceInit, data}, interpolator, before, after} {}

        /** @overload
         * Equivalent to calling @ref Track(Target, std::size_t, Containers::Array<std::pair<Frame, T>>&&, Interpolator, Extrapolation, Extrapolation)
         * with both @p before and @p after set to @p extrapolation.
         */
        explicit Track(Target target, std::size_t index, Containers::Array<std::pair<Frame, T>>&& data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant) noexcept: Track<Target, Frame, T>{target, index, std::move(data), interpolator, extrapolation, extrapolation} {}

        /** @overload */
        explicit Track(Target target, std::size_t index, std::initializer_list<std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant): Track{target, index, Containers::Array<std::pair<Frame, T>>{Containers::InPlaceInit, data}, interpolator, extrapolation} {}

        /** @overload
         * Equivalent to calling @ref Track(Target, std::size_t, Containers::Array<std::pair<Frame, T>>&&, Interpolator, Extrapolation, Extrapolation)
         * with @p index set to `0`.
         */
        explicit Track(Target target, Containers::Array<std::pair<Frame, T>>&& data, Interpolator interpolator, Extrapolation before, Extrapolation after) noexcept: Track<Target, Frame, T>{target, 0, std::move(data), interpolator, before, after} {}

        /** @overload */
        explicit Track(Target target, std::initializer_list<std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation before, Extrapolation after): Track{target, Containers::Array<std::pair<Frame, T>>{Containers::InPlaceInit, data}, interpolator, before, after} {}

        /** @overload
         * Equivalent to calling @ref Track(Target, std::size_t, Containers::Array<std::pair<Frame, T>>&&, Interpolator, Extrapolation, Extrapolation)
         * with @p index set to `0` and both @p before and @p after set to
         * @p extrapolation.
         */
        explicit Track(Target target, Containers::Array<std::pair<Frame, T>>&& data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant) noexcept: Track<Target, Frame, T>{target, 0, std::move(data), interpolator, extrapolation, extrapolation} {}

        /** @overload */
        explicit Track(Target target, std::initializer_list<std::pair<Frame, T>> data, Interpolator interpolator, Extrapolation extrapolation = Extrapolation::Constant): Track{target, Containers::Array<std::pair<Frame, T>>{Containers::InPlaceInit, data}, interpolator, extrapolation} {}

        /** @brief Copying is not allowed */
        Track(const Track<Target, Frame, T>&) = delete;

        /** @brief Move constructor */
        Track(Track<Target, Frame, T>&&) = default;

        /** @brief Copying is not allowed */
        Track<Target, Frame, T>& operator=(const Track<Target, Frame, T>&) = delete;

        /** @brief Move constructor */
        Track<Target, Frame, T>& operator=(Track<Target, Frame, T>&&) = default;

        /**
         * @brief Conversion to a view
         *
         * @ref TrackView shares the same layout so just reinterpreted `this`
         * reference is returned.
         */
        operator const TrackView<Target, Frame, T>&() const noexcept {
            return reinterpret_cast<const TrackView<Target, Frame, T>&>(*this);
        }

        /** @brief Animation target */
        Target target() const { return _target; }

        /** @brief Animation target index */
        std::size_t index() const { return _index; }

        /** @brief Interpolation function */
        Interpolator interpolator() const { return _interpolator; }

        /**
         * @brief Extrapolation behavior before first keyframe
         *
         * @see @ref after(), @ref at(), @ref Clip::start(), @ref Clip::end()
         */
        Extrapolation before() const { return _before; }

        /**
         * @brief Extrapolation behavior after last keyframe
         *
         * @see @ref before(), @ref at(), @ref Clip::start(), @ref Clip::end()
         */
        Extrapolation after() const { return _after; }

        /** @brief Keyframe data */
        Containers::ArrayView<const std::pair<Frame, T>> data() const { return _data; }

        /** @brief Keyframe access */
        const std::pair<Frame, T>& operator[](std::size_t i) const { return _data[i]; }

        /**
         * @brief Animated value at given time
         *
         * See @ref TrackView::at() for more information.
         */
        ResultOf<T> at(Frame frame, std::size_t& hint) const {
            return static_cast<const TrackView<Target, Frame, T>&>(*this).at(frame, hint);
        }

    private:
        Target _target;
        std::size_t _index;
        Interpolator _interpolator;
        Extrapolation _before, _after;
        Containers::Array<std::pair<Frame, T>> _data;
};

/**
@brief Animation track reference
@tparam Target_ Animation target enum type
@tparam Frame_  Frame reference type

Copyable type-erased reference to an animation track.
@see @ref Animator
@experimental
*/
template<class Target_, class Frame_> class TrackRef {
    public:
        /** @brief Animation target enum type */
        typedef Target_ Target;

        /** @brief Frame reference type */
        typedef Frame_ Frame;

        /**
         * @brief Constructor
         *
         * Creates reference onto given track. Does not own the referenced
         * track, so the user has to ensure the track does not go out of scope
         * for the whole reference lifetime.
         */
        template<class T> /*implicit*/ TrackRef(const TrackView<Target, Frame, T>& track) noexcept: _target{track.target()}, _index{track.index()}, _track{&track} {}

        /** @overload  */
        template<class T> /*implicit*/ TrackRef(const Track<Target, Frame, T>& track) noexcept: _target{track.target()}, _index{track.index()}, _track{&track} {}

        /** @brief Animation target */
        Target target() const { return _target; }

        /**
         * @brief Value at given time
         *
         * Calls @ref TrackView::at() on the referenced track.
         */
        template<class T> ResultOf<T> at(Frame frame, std::size_t& hint) const;

    private:
        Target _target;
        std::size_t _index;
        const void* _track;
};

template<class Target_, class Frame_, class T> ResultOf<T> TrackView<Target_, Frame_, T>::at(Frame frame, std::size_t& hint) const {
    return {};
}

}}

#endif
