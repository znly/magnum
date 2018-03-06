#ifndef Magnum_Vk_Vulkan_h
#define Magnum_Vk_Vulkan_h
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
 * @brief Enum @ref Magnum::Vk::ObjectFlag, enum set @ref Magnum::Vk::ObjectFlags, macro @ref MAGNUM_VK_ASSERT_OUTPUT()
 */

#include <Corrade/Containers/EnumSet.h>
#include <Corrade/Utility/Assert.h>

#include "Magnum/Magnum.h"
#include "MagnumExternal/Vulkan/flextVk.h"

namespace Magnum { namespace Vk {

/**
@brief Assert that a Vulkan function call doesn't fail

Equivalent to

@code{.cpp}
CORRADE_INTERNAL_ASSERT(call == VK_SUCCESS)
@endcode
*/
#define MAGNUM_VK_ASSERT_OUTPUT(call) \
    CORRADE_INTERNAL_ASSERT(call == VK_SUCCESS)

/**
@brief Object wrapping flag

@see @ref ObjectFlags, @ref Instance::wrap()
*/
enum class ObjectFlag: UnsignedByte {
    /** Delete the object on destruction. */
    DeleteOnDestruction = 1 << 0
};

/**
@brief Object wrapping flags

@see @ref Instance::wrap()
*/
typedef Containers::EnumSet<ObjectFlag> ObjectFlags;

}}

#endif
