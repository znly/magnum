#ifndef Magnum_Audio_Listener_h
#define Magnum_Audio_Listener_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2015 Jonathan Hale <squareys@googlemail.com>

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
 * @brief Class @ref Magnum::Audio::Listener, @ref Magnum::Audio::Listener2D, @ref Magnum::Audio::Listener3D
 */

#include "Magnum/Audio/Audio.h"
#include "Magnum/Audio/visibility.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/SceneGraph/AbstractFeature.h"

namespace Magnum { namespace Audio {

/**
@brief Listener

Feature which manages the position, orientation and gain of the OpenAL listener
for an @ref SceneGraph::Object.

@section Audio-Listener-usage Usage

Minimal scene setup for a @ref Listener3D will require the following code:

@code{.cpp}
Scene3D scene;
Object3D object{&scene};
Listener3D listener{object};

// ... every frame, update the listener to changes in object transformation:
listener.update({});
@endcode

For two dimensional scenes simply replace all `3D` with `2D`.

@subsection Audio-Listener-usage-playablegroup Using Listener with PlayableGroup

When using @ref PlayableGroup, you can update the listener and groups as
follows:

@code{.cpp}
// ...
Listener3D listener{object};
PlayableGroup3D group1, group2;

// ... and every frame:
listener.update({group1, group2});
@endcode

@section Audio-Listener-active-listener Active listener

There can only be at the most *one* active listener at a given time, i.e. the
one on which @ref Listener::update() was called last. This is because OpenAL
only supports notion of one listener. Having multiple @ref Listener2D or
@ref Listener3D instances can still be useful for conveniently switching
between them for cinematics for example.

@ref AbstractObject::setClean() will not affect inactive listeners.

@section Audio-Listener-sound-transformation Sound transformation

@ref Listener::setSoundTransformation() enables you to set a transformation
matrix which is applied to the listeners orientation and position before passed
onto OpenAL. This can be used for rotating two dimensional scenes as planes
into the three dimensional audio space or even scaling the audio scene to match
a certain world scale. In the later case you might want to instead consider
@ref Renderer::setSpeedOfSound().

-   @ref Listener2D
-   @ref Listener3D

@see @ref Audio::Renderer, @ref Playable, @ref PlayableGroup
*/
template<UnsignedInt dimensions> class Listener: public SceneGraph::AbstractFeature<dimensions, Float> {
    public:
        /**
         * @brief Constructor
         * @param object    Object this listener belongs to
         *
         * Creates a listener with a forward vector of @cpp {0.0f, 0.0f, -1.0f} @ce
         * and up vector of @cpp {0.0f, 1.0f, 0.0f} @ce. These vectors cannot
         * be changed, the listeners orientation and translation can be instead
         * affected by @p object or via @ref Listener::setSoundTransformation().
         * @see @ref setGain()
         */
        explicit Listener(SceneGraph::AbstractObject<dimensions, Float>& object);

        ~Listener();

        /** @brief Sound transformation */
        const Matrix4& soundTransformation() const {
            return _soundTransformation;
        }

        /**
         * @brief Set sound transformation
         * @param soundTransformation Transformation for transforming from
         *        world to listener space
         * @return Reference to self (for method chaining)
         */
        Listener<dimensions>& setSoundTransformation(const Matrix4& soundTransformation) {
            _soundTransformation = soundTransformation;
            this->object().setDirty();
            return *this;
        }

        /**
         * @brief Update the listener
         * @param groups Groups to update
         *
         * Makes this Listener the active listener and calls
         * @ref SceneGraph::AbstractObject::setClean() on its parent object and
         * all objects of the @ref Playable s in the group. Updates listene
         * related configuration for @ref Renderer (position, orientation, gain).
         */
        void update(std::initializer_list<std::reference_wrapper<PlayableGroup<dimensions>>> groups);

        /** @brief Listener gain */
        Float gain() const { return _gain; }

        /**
         * @brief Set listener gain
         * @param gain Gain
         * @return Reference to self (for method chaining)
         */
        Listener<dimensions>& setGain(Float gain);

        /** @brief Whether this listener is the active listener */
        bool isActive() const;

    private:
        MAGNUM_AUDIO_LOCAL void clean(const MatrixTypeFor<dimensions, Float>& absoluteTransformationMatrix) override;

        Matrix4 _soundTransformation;
        Float _gain;
};

/**
 * @brief Listener for two dimensional float scenes
 *
 * @see @ref Listener3D
 */
typedef Listener<2> Listener2D;

/**
 * @brief Listener for three dimensional float scenes
 *
 * @see @ref Listener2D
 */
typedef Listener<3> Listener3D;

#if defined(CORRADE_TARGET_WINDOWS) && !defined(__MINGW32__)
extern template class MAGNUM_AUDIO_EXPORT Listener<2>;
extern template class MAGNUM_AUDIO_EXPORT Listener<3>;
#endif

}}

#endif
