#ifndef Magnum_SceneGraph_Drawable_h
#define Magnum_SceneGraph_Drawable_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018
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
 * @brief Class @ref Magnum::SceneGraph::Drawable, @ref Magnum::SceneGraph::DrawableGroup, alias @ref Magnum::SceneGraph::BasicDrawable2D, @ref Magnum::SceneGraph::BasicDrawable3D, @ref Magnum::SceneGraph::BasicDrawableGroup2D, @ref Magnum::SceneGraph::BasicDrawableGroup3D, typedef @ref Magnum::SceneGraph::Drawable2D, @ref Magnum::SceneGraph::Drawable3D, @ref Magnum::SceneGraph::DrawableGroup2D, @ref Magnum::SceneGraph::DrawableGroup3D
 */

#include "Magnum/SceneGraph/AbstractGroupedFeature.h"

namespace Magnum { namespace SceneGraph {

/**
@brief Drawable

Adds drawing functionality to the object. Each Drawable is part of some
@ref DrawableGroup and the whole group can be drawn with particular camera
using @ref Camera::draw().

@section SceneGraph-Drawable-usage Usage

First thing is to add @ref Drawable feature to some object and implement
@ref draw() function. You can do it conveniently using multiple inheritance
(see @ref scenegraph-features for introduction). Example drawable object that
draws blue sphere:

@snippet MagnumSceneGraph-gl.cpp Drawable-usage

The @p transformationMatrix parameter in @ref draw() function contains
transformation of the object (to which the drawable is attached) relative to
@p camera. The camera contains projection matrix. Some shaders (like the
@ref Shaders::Phong used in the example) have separate functions for setting
transformation and projection matrix, but some (such as @ref Shaders::Flat)
have single function to set composite transformation and projection matrix. In
that case you need to combine the two matrices manually like in the following
code. Some shaders have additional requirements for various transformation
matrices, see their respective documentation for details.

@snippet MagnumSceneGraph-gl.cpp Drawable-usage-shader

There is no way to just draw all the drawables in the scene, you need to create
some drawable group and add the drawable objects to both the scene and the
group. You can also use @ref DrawableGroup::add() and
@ref DrawableGroup::remove() instead of passing the group in the constructor.

@snippet MagnumSceneGraph-gl.cpp Drawable-usage-instance

The last thing you need is camera attached to some object (thus using its
transformation). Using the camera and the drawable group you can perform
drawing in your @ref Platform::Sdl2Application::drawEvent() "drawEvent()"
implementation. See @ref Camera2D and @ref Camera3D documentation for more
information.

@snippet MagnumSceneGraph-gl.cpp Drawable-usage-camera

@section SceneGraph-Drawable-multiple-groups Using multiple drawable groups to improve performance

You can organize your drawables to multiple groups to minimize OpenGL state
changes --- for example put all objects using the same shader, the same light
setup etc into one group, then put all transparent into another and set common
parameters once for whole group instead of setting them again in each
@ref draw() implementation. Example:

@snippet MagnumSceneGraph-gl.cpp Drawable-multiple-groups

@section SceneGraph-Drawable-explicit-specializations Explicit template specializations

The following specializations are explicitly compiled into @ref SceneGraph
library. For other specializations (e.g. using @ref Magnum::Double "Double"
type) you have to use @ref Drawable.hpp implementation file to avoid linker
errors. See also @ref compilation-speedup-hpp for more information.

-   @ref Drawable2D
-   @ref Drawable3D

@see @ref scenegraph, @ref BasicDrawable2D, @ref BasicDrawable3D,
    @ref Drawable2D, @ref Drawable3D, @ref DrawableGroup
*/
template<UnsignedInt dimensions, class T> class Drawable: public AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T> {
    public:
        /**
         * @brief Constructor
         * @param object    Object this drawable belongs to
         * @param drawables Group this drawable belongs to
         *
         * Adds the feature to the object and also to the group, if specified.
         * Otherwise you can use @ref DrawableGroup::add().
         */
        explicit Drawable(AbstractObject<dimensions, T>& object, DrawableGroup<dimensions, T>* drawables = nullptr);

        /**
         * @brief Group containing this drawable
         *
         * If the drawable doesn't belong to any group, returns @cpp nullptr @ce.
         */
        DrawableGroup<dimensions, T>* drawables() {
            return AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group();
        }

        /** @overload */
        const DrawableGroup<dimensions, T>* drawables() const {
            return AbstractGroupedFeature<dimensions, Drawable<dimensions, T>, T>::group();
        }

        /**
         * @brief Draw the object using given camera
         * @param transformationMatrix  Object transformation relative to camera
         * @param camera                Camera
         *
         * Projection matrix can be retrieved from
         * @ref SceneGraph::Camera::projectionMatrix() "Camera::projectionMatrix()".
         */
        virtual void draw(const MatrixTypeFor<dimensions, T>& transformationMatrix, Camera<dimensions, T>& camera) = 0;
};

/**
@brief Drawable for two-dimensional scenes

Convenience alternative to @cpp Drawable<2, T> @ce. See @ref Drawable for more
information.
@see @ref Drawable2D, @ref BasicDrawable3D
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using BasicDrawable2D = Drawable<2, T>;
#endif

/**
@brief Drawable for two-dimensional float scenes

@see @ref Drawable3D
*/
typedef BasicDrawable2D<Float> Drawable2D;

/**
@brief Drawable for three-dimensional scenes

Convenience alternative to @cpp Drawable<3, T> @ce. See @ref Drawable for more
information.
@see @ref Drawable3D, @ref BasicDrawable3D
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using BasicDrawable3D = Drawable<3, T>;
#endif

/**
@brief Drawable for three-dimensional float scenes

@see @ref Drawable2D
*/
typedef BasicDrawable3D<Float> Drawable3D;

/**
@brief Group of drawables

See @ref Drawable for more information.
@see @ref scenegraph, @ref BasicDrawableGroup2D, @ref BasicDrawableGroup3D,
    @ref DrawableGroup2D, @ref DrawableGroup3D
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<UnsignedInt dimensions, class T> using DrawableGroup = FeatureGroup<dimensions, Drawable<dimensions, T>, T>;
#endif

/**
@brief Group of drawables for two-dimensional scenes

Convenience alternative to @cpp DrawableGroup<2, T> @ce. See @ref Drawable for
more information.
@see @ref DrawableGroup2D, @ref BasicDrawableGroup3D
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using BasicDrawableGroup2D = DrawableGroup<2, T>;
#endif

/**
@brief Group of drawables for two-dimensional float scenes

@see @ref DrawableGroup3D
*/
typedef BasicDrawableGroup2D<Float> DrawableGroup2D;

/**
@brief Group of drawables for three-dimensional scenes

Convenience alternative to @cpp DrawableGroup<3, T> @ce. See @ref Drawable for
more information.
@see @ref DrawableGroup3D, @ref BasicDrawableGroup2D
*/
#ifndef CORRADE_MSVC2015_COMPATIBILITY /* Multiple definitions still broken */
template<class T> using BasicDrawableGroup3D = DrawableGroup<3, T>;
#endif

/**
@brief Group of drawables for three-dimensional float scenes

@see @ref DrawableGroup2D
*/
typedef BasicDrawableGroup3D<Float> DrawableGroup3D;

#if defined(CORRADE_TARGET_WINDOWS) && !defined(__MINGW32__)
extern template class MAGNUM_SCENEGRAPH_EXPORT Drawable<2, Float>;
extern template class MAGNUM_SCENEGRAPH_EXPORT Drawable<3, Float>;
#endif
}}

#endif
