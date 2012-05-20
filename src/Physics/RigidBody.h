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
 * @brief Class Magnum::Physics::RigidBody
 */

#include "SceneGraph/Object.h"
#include "AbstractRigidBodyGroup.h"

namespace Magnum { namespace Physics {

class AbstractShape;

/**
@brief Rigid body
*/
class RigidBody: public SceneGraph::Object3D {
    public:
        /** @copydoc AbstractRigidBodyGroup::ChildrenCollisions */
        typedef AbstractRigidBodyGroup::ChildrenCollisions ChildrenCollisions;

        /**
         * @brief Constructor
         * @param group     Group this body is part of
         * @param parent    Parent object
         *
         * Constructs ridig body without shape. Call setShape() to set shape
         * of the object.
         */
        RigidBody(AbstractRigidBodyGroup* group, SceneGraph::Object3D* parent = nullptr): Object3D(parent), _group(nullptr), _shape(nullptr) {
            group->add(this);
        }

        /** @{ @name Collision detection */

        /** @brief Group this rigid body is part of */
        inline AbstractRigidBodyGroup* group() { return _group; }

        /** @brief Rigid body shape */
        inline const AbstractShape* shape() const { return _shape; }

        /**
         * @brief Set rigid body shape
         *
         * This operation sets the object as dirty, as it affects collision
         * detection.
         */
        inline void setShape(AbstractShape* shape) {
            _shape = shape;
            setDirty();
        }

        /**
         * @brief Whether the body collides with other
         * @param other                 Other body
         * @param childrenCollisions    Take collisions of this body children
         *      into account
         *
         * If the @p other body is not part of the same group, returns `false`.
         */
        inline bool collides(RigidBody* other, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) {
            return _group->collides(this, other, childrenCollisions);
        }

        /**
         * @brief Whether the body collides with anything else in the group
         * @param childrenCollisions    Take collisions of this body children
         *      into account
         */
        inline bool collides(ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) {
            return _group->collides(this, childrenCollisions);
        }

        /**
         * @brief List of all collision within the group
         * @param childrenCollisions    Take collisions of this body children
         *      into account
         */
        inline std::vector<RigidBody*> collisions(ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) {
            return _group->collisions(this, childrenCollisions);
        }

        /*@}*/

        /**
         * @{ @name Collision-aware rigid body transformation
         *
         * As in Object itself, all transformations are applied relatively
         * to parent.
         */

        /**
         * @brief Try to set transformation
         * @return `False` if the object would collide with another in the
         *      group after applying transformation, `true` otherwise
         */
        bool trySetTransformation(const Matrix4& transformation);

        /**
         * @brief Try to multiply transformation
         * @param transformation    Transformation
         * @param type              Transformation type
         * @return `False` if the object would collide with another in the
         *      group after applying transformation, `true` otherwise
         */
        bool tryMultiplyTransformation(const Matrix4& transformation, Transformation type = Transformation::Global) {
            return trySetTransformation(type == Transformation::Global ?
                transformation*this->transformation() : this->transformation()*transformation);
        }

        /**
         * @brief Try to translate object
         *
         * Same as calling tryMultiplyTransformation() with Matrix4::translation().
         */
        inline bool tryTranslate(Vector3 vec, Transformation type = Transformation::Global) {
            return tryMultiplyTransformation(Matrix4::translation(vec), type);
        }

        /**
         * @brief Try to scale object
         *
         * Same as calling tryMultiplyTransformation() with Matrix4::scaling().
         */
        inline bool tryScale(Vector3 vec, Transformation type = Transformation::Global) {
            return tryMultiplyTransformation(Matrix4::scaling(vec), type);
        }

        /**
         * @brief Try to rotate object
         *
         * Same as calling tryMultiplyTransformation() with Matrix4::rotation().
         */
        inline bool tryRotate(GLfloat angle, Vector3 vec, Transformation type = Transformation::Global) {
            return tryMultiplyTransformation(Matrix4::rotation(angle, vec), type);
        }

        /*@}*/

        /** @{ @name Caching and collision detection helpers
         *
         * These functions help to efficiently manage AbstractRigidBodyGroup
         * collision detection. After every shape or transformation change
         * they notify the group, so it can recalculate its internal state
         * to provide the best performance. These functions are used
         * internally, so you probably wouldn't need to call them directly.
         */
        void setDirty();
        Matrix4 setClean();
        /*@}*/

    private:
        AbstractRigidBodyGroup* _group;
        AbstractShape* _shape;
};

}}

#endif
