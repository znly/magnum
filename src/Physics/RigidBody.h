#ifndef Magnum_Physics_RigidBody_h
#define Magnum_Physics_RigidBody_h
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
 * @brief Class Magnum::Physics::RigidBody, typedef Magnum::Physics::RigidBody2D, Magnum::Physics::RigidBody3D
 */

#include "ShapedObject.h"

#include "magnumPhysicsVisibility.h"

namespace Magnum { namespace Physics {

template<size_t> class RigidBody;

/** @brief Two-dimensional rigid body */
typedef RigidBody<2> RigidBody2D;

/** @brief Three-dimensional rigid body */
typedef RigidBody<3> RigidBody3D;

template<size_t> class RigidBodyGroup;

typedef RigidBodyGroup<2> RigidBodyGroup2D;
typedef RigidBodyGroup<3> RigidBodyGroup3D;

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Implementation {
    template<size_t dimensions> struct RigidBodyDimensionTraits {};

    template<> struct RigidBodyDimensionTraits<2> {
        typedef RigidBody2D RigidBodyType;
        typedef RigidBodyGroup2D RigidBodyGroupType;
    };

    template<> struct RigidBodyDimensionTraits<3> {
        typedef RigidBody3D RigidBodyType;
        typedef RigidBodyGroup3D RigidBodyGroupType;
    };
}
#endif

/**
@brief Base class for rigid bodies

@see RigidBody2D, RigidBody3D
*/
template<size_t dimensions> class RigidBody: public ShapedObject<dimensions> {
    friend class Implementation::RigidBodyDimensionTraits<dimensions>::RigidBodyGroupType;

    public:
        /** @brief Rigid body type for given dimension count */
        typedef typename Implementation::RigidBodyDimensionTraits<dimensions>::RigidBodyType RigidBodyType;

        /** @brief Rigid body group type for given dimension count */
        typedef typename Implementation::RigidBodyDimensionTraits<dimensions>::RigidBodyGroupType RigidBodyGroupType;

        /**
         * @brief Constructor
         * @param group     Group this rigid body belongs to
         * @param parent    Parent object
         *
         * Creates object with zero velocity, zero mass and center at object
         * origin.
         * @see setMass(), setCenter()
         */
        RigidBody(RigidBodyGroupType* group, typename SceneGraph::AbstractObject<dimensions>::ObjectType* parent = nullptr);

        ~RigidBody();

        /** @brief Group this rigid body belongs to */
        inline RigidBodyGroupType* group() { return _group; }

        /** @overload */
        inline const RigidBodyGroupType* group() const { return _group; }

        /** @brief Center of mass */
        inline typename SceneGraph::AbstractObject<dimensions>::VectorType center() const {
            return _center;
        }

        /** @brief Set center of mass */
        inline void setCenter(const typename SceneGraph::AbstractObject<dimensions>::VectorType& center) {
            _center = center;
        }

        /** @brief Mass */
        inline GLfloat mass() const { return _mass; }

        /** @brief Set mass */
        inline void setMass(GLfloat mass) {
            _mass = mass;
        }

        /** @brief Linear velocity */
        inline typename SceneGraph::AbstractObject<dimensions>::VectorType linearVelocity() const {
            return _linearVelocity;
        }

        /** @brief Set linear velocity */
        inline void setLinearVelocity(const typename SceneGraph::AbstractObject<dimensions>::VectorType& velocity) {
            _linearVelocity = velocity;
        }

        /** @todo angular velocity */

        /**
         * @brief Apply impulse
         *
         * Applies one-time @p impulse to given @p point. Equivalent to
         * calling applyForce() in physicsStep() with infinitely small time
         * delta.
         */
        inline void applyImpulse(const typename SceneGraph::AbstractObject<dimensions>::VectorType& point, const typename SceneGraph::AbstractObject<dimensions>::VectorType& impulse) {
            _linearVelocity += impulse/_mass;

            /** @todo torque */
        }

        /** @todo angular impulse */

    protected:
        /**
         * @brief Do one physics step
         *
         * Called from RigidBodyGroup::physicsStep(). You can call applyForce()
         * here, the force will be applied for time given in @p timeDelta.
         * Default implementation does nothing.
         * @attention This function may be called one or more times per frame,
         *      so the implementation must be reentrant.
         * @todo Step depending also on time
         */
        virtual void physicsStep(GLfloat timeDelta);

        /**
         * @brief Apply force
         *
         * @attention Must be called only in physicsStep()
         */
        inline void applyForce(const typename SceneGraph::AbstractObject<dimensions>::VectorType& point, const typename SceneGraph::AbstractObject<dimensions>::VectorType& force) {
            this->force += force;

            /** @todo torque */
        }

    private:
        typename Implementation::RigidBodyDimensionTraits<dimensions>::RigidBodyGroupType* _group;
        typename SceneGraph::AbstractObject<dimensions>::VectorType _center;
        GLfloat _mass;
        typename SceneGraph::AbstractObject<dimensions>::VectorType _linearVelocity;

        typename SceneGraph::AbstractObject<dimensions>::VectorType force;
};

template<size_t dimensions> inline void RigidBody<dimensions>::physicsStep(GLfloat) {}

#ifndef DOXYGEN_GENERATING_OUTPUT
/* These templates are instantiated in source file */
extern template class PHYSICS_EXPORT RigidBody<2>;
extern template class PHYSICS_EXPORT RigidBody<3>;
#endif

}}

#endif
