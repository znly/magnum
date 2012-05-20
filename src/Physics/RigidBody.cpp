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

#include "AbstractShape.h"

namespace Magnum { namespace Physics {

bool RigidBody::trySetTransformation(const Matrix4& transformation) {
    Matrix4 oldTransformation = this->transformation();

    /* Try to set new transformation */
    setTransformation(transformation);
    if(!collides()) return true;

    /* Reset old transformation */
    setTransformation(oldTransformation);
    return false;
}

void RigidBody::setDirty() {
    _group->setDirty();

    Object::setDirty();
}

Matrix4 RigidBody::setClean() {
    Matrix4 absoluteTransformation = Object::setClean();

    if(_shape) _shape->applyTransformation(absoluteTransformation);

    return absoluteTransformation;
}


}}
