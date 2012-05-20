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

#include "AbstractRigidBodyGroup.h"
#include <set>

namespace Magnum { namespace Physics {

/**
@brief Simple implementation of rigid body group

@todo Do it in best possible way and don't provide alternatives?
*/
class RigidBodyGroup: public AbstractRigidBodyGroup {
    public:
        inline void add(RigidBody* body) { bodies.insert(body); }

        inline void remove(RigidBody* body) { bodies.erase(body); }

        bool collides(RigidBody* a, RigidBody* b, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude);

        bool collides(RigidBody* body, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude);

        std::vector<RigidBody*> collisions(RigidBody* body, ChildrenCollisions childrenCollisions = ChildrenCollisions::Exclude);

        void setClean();

    private:
        std::set<RigidBody*> bodies;
};

}}
