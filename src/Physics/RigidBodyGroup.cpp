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

namespace Magnum { namespace Physics {

template<size_t dimensions> void RigidBodyGroup<dimensions>::physicsStep(GLfloat timeDelta) {
    for(typename RigidBody<dimensions>::RigidBodyType* body: bodies) {
        /* Compute force at current position */
        body->force = {};
        body->physicsStep(timeDelta);

        typename RigidBody<dimensions>::VectorType velocityIncrease = 0.5f*(_gravity + body->force/body->_mass)*timeDelta;

        /* New position */
        body->translate((body->_linearVelocity += velocityIncrease)*timeDelta);

        /* Compute force at new position */
        body->force = {};
        body->physicsStep(timeDelta);

        /* New velocity */
        body->_linearVelocity += /*velocityIncrease + */0.5f*(_gravity + body->force/body->_mass)*timeDelta;
    }
}

/* Explicitly instantiate the templates */
template class RigidBodyGroup<2>;
template class RigidBodyGroup<3>;

}}
