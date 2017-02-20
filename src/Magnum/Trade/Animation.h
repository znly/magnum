#ifndef Magnum_Trade_Animation_h
#define Magnum_Trade_Animation_h
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
 * @brief Enum @ref Magnum::Trade::AnimationTarget, typedef @ref Magnum::Trade::AnimationClip
 */

#include "Magnum/Magnum.h"
#include "Magnum/Animation/Animation.h"

namespace Magnum { namespace Trade {

/**
@brief Animation target

@see @ref Animation::Track, @ref Animation::Animator
@experimental
*/
enum class AnimationTarget: UnsignedShort {
    Translation,    /**< Translation (@ref Vector3) */
    TranslationX,   /**< Translation on X axis (@ref Float) */
    TranslationY,   /**< Translation on Y axis (@ref Float) */
    TranslationZ,   /**< Translation on Z axis (@ref Float) */

    Rotation,       /**< Rotation (@ref Quaternion) */
    RotationX,      /**< Rotation on X axis (@ref Rad) */
    RotationY,      /**< Rotation on Y axis (@ref Rad) */
    RotationZ,      /**< Rotation on Z axis (@ref Rad) */

    Scaling,        /**< Scaling (@ref Vector3) */
    ScalingX,       /**< Scaling on X axis (@ref Float) */
    ScalingY,       /**< Scaling on Y axis (@ref Float) */
    ScalingZ,        /**< Scaling on Z axis (@ref Float) */

    Color
};

typedef Animation::Clip<Trade::AnimationTarget, Float> AnimationClip;
template<class T> using AnimationTrack = Animation::Track<Trade::AnimationTarget, Float, T>;

}

namespace Animation { namespace Implementation {

template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::Translation> {
    typedef Vector3 Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::TranslationX> {
    typedef Float Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::TranslationY> {
    typedef Float Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::TranslationZ> {
    typedef Float Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::Rotation> {
    typedef Quaternion Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::RotationX> {
    typedef Rad Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::RotationY> {
    typedef Rad Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::RotationZ> {
    typedef Rad Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::Scaling> {
    typedef Vector3 Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::ScalingX> {
    typedef Vector3 Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::ScalingY> {
    typedef Vector3 Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::ScalingZ> {
    typedef Vector3 Type;
};
template<> struct TargetTraits<Trade::AnimationTarget, Trade::AnimationTarget::Color> {
    typedef Color4 Type;
};

}}}

#endif
