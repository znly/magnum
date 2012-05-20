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

#include "RigidBodyGroup.h"
#include "RigidBody.h"
#include "AbstractShape.h"

namespace Magnum { namespace Physics {

bool RigidBodyGroup::collides(RigidBody* a, RigidBody* b, ChildrenCollisions) {
    if(!isDirty()) setClean();

    /** @todo Please think of the children! */
    return a->shape() && b->shape() && a->shape()->collides(b->shape());
}

bool RigidBodyGroup::collides(RigidBody* body, ChildrenCollisions) {
    if(!isDirty()) setClean();

    /** @todo Please think of the children! */
    if(!body->shape()) return false;

    for(RigidBody* other: bodies)
        if(other->shape() && body->shape()->collides(other->shape()))
            return true;

    return false;
}

std::vector<RigidBody*> RigidBodyGroup::collisions(RigidBody* body, ChildrenCollisions) {
    if(!isDirty()) setClean();

    /** @todo Please think of the children! */
    std::vector<RigidBody*> collisions;
    if(!body->shape()) return collisions;

    for(RigidBody* other: bodies)
        if(other->shape() && body->shape()->collides(other->shape()))
            collisions.push_back(other);

    return collisions;
}

void RigidBodyGroup::setClean() {
    for(RigidBody* body: bodies)
        if(body->isDirty()) setClean();

    AbstractRigidBodyGroup::setClean();
}

}}
