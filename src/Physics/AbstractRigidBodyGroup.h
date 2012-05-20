#ifndef Magnum_Physics_AbstractRigidBodyGroup_h
#define Magnum_Physics_AbstractRigidBodyGroup_h
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
 * @brief Class Magnum::Physics::AbstractRigidBodyGroup
 */

#include <vector>

namespace Magnum { namespace Physics {

class RigidBody;

/**
@brief Base class for groups of rigid bodies interacting with each other
*/
class AbstractRigidBodyGroup {
    AbstractRigidBodyGroup(const AbstractRigidBodyGroup& other) = delete;
    AbstractRigidBodyGroup(AbstractRigidBodyGroup&& other) = delete;
    AbstractRigidBodyGroup& operator=(const AbstractRigidBodyGroup& other) = delete;
    AbstractRigidBodyGroup& operator=(AbstractRigidBodyGroup&& other) = delete;

    public:
        /** @brief Whether to include or exclude collisions of child objects */
        enum class ChildrenCollisions {
            Exclude,    /**< @brief Exclude object children */
            Include     /**< @brief Include object children */
        };

        /** @brief Destructor */
        inline virtual ~AbstractRigidBodyGroup() {}

        /**
         * @brief Add body to the group
         *
         * The body is removed from previous group.
         */
        virtual void add(RigidBody* body) = 0;

        /**
         * @brief Remove body from the group
         *
         * If the body is not part of this group, does nothing.
         */
        virtual void remove(RigidBody* body) = 0;

        /**
         * @brief Whether two bodies collide
         * @param a             The body which could be colliding
         * @param b             The body into which @p a could collide
         * @param childrenCollisions Take children of @p a collisions into
         *      account (collisions with children of @p b are never computed)
         *
         * If any body is not part of this group, returns `false`.
         */
        virtual bool collides(RigidBody* a, RigidBody* b, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) = 0;

        /**
         * @brief Whether the body collides with anything
         * @param body          The body which could be colliding
         * @param childrenCollisions Take its children collisions into account
         *
         * If the body is not part of this group, returns `false`.
         */
        virtual bool collides(RigidBody* body, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) = 0;

        /**
         * @brief List of bodies colliding with given body
         * @param body          The body which could be colliding
         * @param childrenCollisions Take its children collisions into account
         *
         * If the body is not part of this group, returns empty vector.
         */
        virtual std::vector<RigidBody*> collisions(RigidBody* body, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude) = 0;

        /**
         * @brief Whether the object is dirty
         * @return  True, if transformation of the object, any parent or camera
         *      has changed since last asking, false otherwise.
         */
        inline bool isDirty() const { return dirty; }

        /**
         * @brief Set the group as dirty
         *
         * If some body in the group changes its transformation, it sets dirty
         * status also on the group to indicate that the body and maybe also
         * group state needs to be cleaned before computing collisions.
         *
         * @see setClean()
         */
        inline void setDirty() { dirty = true; }

        /**
         * @brief Set the group and all bodies as clean
         *
         * This function is called before computing any collisions to ensure
         * all bodies and group state are cleaned.
         *
         * @attention This function must be reimplemented and also called
         *      from the reimplementation to have the state set back to clean.
         */
        inline virtual void setClean() { dirty = false; }

    private:
        bool dirty;
};

}}

#endif
