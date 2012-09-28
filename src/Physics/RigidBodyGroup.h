#ifndef Magnum_Physics_RigidBodyGroup_h
#define Magnum_Physics_RigidBodyGroup_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Class Magnum::Physics::RigidBodyGroup, typedef Magnum::Physics::RigidBodyGroup2D, Magnum::Physics::RigidBodyGroup3D
 */

#include "ShapedObjectGroup.h"
#include "RigidBody.h"

namespace Magnum { namespace Physics {

/**
@brief Base class for groups of rigid bodies

@ref RigidBody "RigidBody*D" instances are added to the group by specifying it
in the constructor. When the group is deleted, all objects belogning to it are
deleted too.
@see RigidBodyGroup2D, RigidBodyGroup3D
*/
template<size_t dimensions> class RigidBodyGroup: public ShapedObjectGroup<dimensions> {
    friend class RigidBody<dimensions>::RigidBodyType;

    public:
        /** @brief Gravity */
        inline typename SceneGraph::AbstractObject<dimensions>::VectorType gravity() const { return _gravity; }

        /** @brief Set gravity */
        inline void setGravity(const typename SceneGraph::AbstractObject<dimensions>::VectorType& gravity) { _gravity = gravity; }

        /**
         * @brief Do physics step
         *
         * Call in game loop before drawing the scene.
         *
         * Implemented using Velocity Verlet algorithm.
         */
        void physicsStep(GLfloat timeDelta);

    private:
        std::vector<typename Implementation::RigidBodyDimensionTraits<dimensions>::RigidBodyType*> bodies;

        typename SceneGraph::AbstractObject<dimensions>::VectorType _gravity;

        GLfloat currentTimeDelta;
};

#ifndef DOXYGEN_GENERATING_OUTPUT
/* These templates are instantiated in source file */
extern template class PHYSICS_EXPORT RigidBodyGroup<2>;
extern template class PHYSICS_EXPORT RigidBodyGroup<3>;
#endif

/** @brief Group of two-dimensional rigid bodies */
typedef RigidBodyGroup<2> RigidBodyGroup2D;

/** @brief Group of three-dimensional rigid bodies */
typedef RigidBodyGroup<3> RigidBodyGroup3D;

}}

#endif
