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

#include "RigidBody.h"

#include <algorithm>

#include "RigidBodyGroup.h"

using namespace std;

namespace Magnum { namespace Physics {

template<size_t dimensions> RigidBody<dimensions>::RigidBody(RigidBodyGroupType* group, typename SceneGraph::AbstractObject<dimensions>::ObjectType* parent): ShapedObject<dimensions>(group, parent), _group(group) {
    _group->bodies.push_back(this);
}

template<size_t dimensions> RigidBody<dimensions>::~RigidBody() {
    _group->bodies.erase(find(_group->bodies.begin(), _group->bodies.end(), this));
}

/* Explicitly instantiate the templates */
template class RigidBody<2>;
template class RigidBody<3>;

}}
