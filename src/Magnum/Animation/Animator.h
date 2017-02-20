#ifndef Magnum_Animation_Animator_h
#define Magnum_Animation_Animator_h
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
 * @brief Class @ref Magnum::Animation::Animator, enum class @ref Magnum::Animation::State, alias @ref Magnum::Animation::TypeFor
 */

#include <vector>

#include "Magnum/Animation/Clip.h"

namespace Magnum { namespace Animation {

/**
@brief Animation value type for given target

@see @ref Animator
@experimental
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class Target, Target target> using TypeFor = typename Implementation::TargetTraits<Target, target>::Type;
#endif

/**
@brief Animation result type for given target

@see @ref Animator
@experimental
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class Target, Target target> using ResultFor = ResultOf<TypeFor<Target, target>>;
#endif

/**
@brief Animation clip state

@see @ref Animator
@experimental
*/
enum class State: UnsignedShort {
    /**
     * The animation clip is currently playing. Setting the state to
     * @ref State::Playing does nothing.
     */
    Playing,

    /**
     * The animation clip is currently paused. Setting the state to
     * @ref State::Playing starts playing from where it left, setting
     * the state to @ref State::Stopped stops the animation, setting
     * the state to @ref State::Paused does nothing.
     */
    Paused,

    /**
     * The animation clip is currently stopped. Setting the state to
     * @ref State::Playing starts playing from the beginning,
     * attempting to set the state to @ref State::Paused will retain
     * the @ref State::Stopped state, setting the state to
     * @ref State::Stopped does nothing.
     */
    Stopped
};

/**
@brief Animator
@tparam Clip_   Clip type
@tparam Time_   Time type

Manages state of various @ref AnimationClip. Tracks referenced by the clips are
not owned so the user has to ensure that the tracks do not go out of scope for
the whole lifetime of the animator.
@experimental
*/
template<class Clip_, class Time_ = typename Clip_::Frame> class Animator {
    public:
        typedef Clip_ Clip;                     /**< @brief Clip type */
        typedef typename Clip::Target Target;   /**< @brief Animation target type */
        typedef typename Clip::Frame Frame;     /**< @brief Frame reference type */
        typedef Time_ Time;                     /**< @brief Time type */

        /**
         * @brief Constructor
         *
         * Creates an empty animator. Use @ref add() to add clips.
         */
        explicit Animator();

        /**
         * @brief Add a clip
         * @param clip      Animation clip
         *
         * Adds tracks from given @p clip to the animator. The original
         * @ref AnimationClip does not need to be kept in scope, but the
         * @ref AnimationTrack instances referenced by it has to be available
         * for the whole animator lifetime. Returns a clip index which is used
         * in @ref play(), @ref pause(), @ref stop(), @ref setSpeed() and
         * @ref setWeight() functions. The clip indices are sequential,
         * starting from `0`.
         */
        std::size_t add(const Clip& clip);

        /**
         * @brief Index count for given targets
         *
         * For index count above one it's only possible to use
         * @ref advanceIndexed() for calculating animation values (and not
         * @ref advance()).
         */
        template<Target ...targets> std::size_t indexCount() const;

        /**
         * @brief Targets handled by the animator
         *
         * @see @ref advance(), @ref advanceIndexed()
         */
        Containers::Array<Target> targets() const;

        /**
         * @brief Clip state
         *
         * Expects that @p clip is a clip index returned from @ref add().
         * @see @ref play(), @ref pause(), @ref stop(), @ref setState()
         */
        State state(std::size_t clip) const;

        /**
         * @brief Play a clip
         * @param time  Time
         * @param clip  Clip index
         *
         * Expects that @p clip is a clip index returned from @ref add().
         */
        void play(Time time, std::size_t clip);
        void play(Time time, std::initializer_list<std::size_t> clips); /**< @overload */

        /**
         * @brief Play a clip with specific play count
         *
         * Unlike @ref play(std::size_t) overrides the play count specified
         * when calling @ref add().
         */
        void play(Time time, std::size_t clip, std::size_t playCount);
        void play(Time time, std::initializer_list<std::size_t> clips, std::size_t playCount); /**< @overload */

        /**
         * @brief Pause a clip
         *
         * Expects that @p clip is a clip index returned from @ref add().
         */
        void pause(Time time, std::size_t clip);
        void pause(Time time, std::initializer_list<std::size_t> clips); /**< @overload */

        /**
         * @brief Stop a clip
         *
         * Expects that @p clip is a clip index returned from @ref add().
         */
        void stop(std::size_t clip);
        void stop(std::initializer_list<std::size_t> clips); /**< @overload */

        /**
         * @brief Set clip speed
         *
         * Sets clip speed with immediate effect. Calling this function while
         * the clip is playing is allowed, but may cause jumps in the playback.
         */
        void setSpeed(std::size_t clip, Float speed);
        void setSpeed(std::initializer_list<std::size_t> clips, Float speed);

        /**
         * @brief Set clip weight
         *
         * Sets clip weight with immediate effect. Calling this function while
         * the clip is playing is allowed, but may cause jumps in the playback.
         */
        void setWeight(std::size_t clip, Float speed);
        void setWeight(std::initializer_list<std::size_t> clips, Float speed);

        /**
         * @brief Advance the animation
         *
         * Expects that the animation doesn't have indexed targets
         * (@ref Track::index() being zero for all tracks), use
         * @ref advanceIndexed() otherwise.
         *
         * Calling this function with targets that are not handled by the
         * animator will return implicitly constructed values. It is not an
         * error to call this function with just a subset of targets or
         * multiple times with the same @p time value.
         * @todo what about decreasing time?
         */
        template<Target first, Target second, Target ...next> std::tuple<ResultFor<Target, first>, ResultFor<Target, second>, ResultFor<Target, next>...> advance(Time time);

        template<Target target> ResultFor<Target, target> advance(Time time);

        /**
         * @brief Advance the animation with indexed targets
         * @param[in]  time     Time to which to advance the animation
         * @param[out] values   Where to store the interpolated values
         *
         * Expects that the array size is large enough to store values for all
         * indexed targets. You can use @ref indexCount() with the same target
         * list for verifying the size.
         */
        template<Target ...target> void advanceIndexed(Time time, Containers::ArrayView<std::tuple<ResultFor<Target, target>...>> values);

    private:
        struct Track {
            TrackRef<Target, Frame> track;
            Time startTime;
            Float weight;
            Float speed;
        };

        std::vector<Track> _tracks;
        std::vector<std::size_t> _clipMap;
        std::vector<std::size_t> _targetMap;
        std::vector<std::pair<Target, std::size_t>> _targets;
};

}}

#endif
