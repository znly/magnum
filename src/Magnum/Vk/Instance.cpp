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

#include "Instance.h"

#include "MagnumExternal/Vulkan/flextVkGlobal.h"

namespace Magnum { namespace Vk {

InstanceCreateInfo::InstanceCreateInfo(): _info{} {
    _info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
}

InstanceCreateInfo::InstanceCreateInfo(NoInitT) {}

InstanceCreateInfo::~InstanceCreateInfo() {}

Instance Instance::wrap(VkInstance handle, ObjectFlags flags) {
    Instance out{NoCreate};
    out._handle = handle;
    out._flags = flags;

    /* Init the function pointers */
    flextVkInitInstance(_handle, &out._functionPointers);

    return out;
}

Instance::Instance(const InstanceCreateInfo& info): _flags{ObjectFlag::DeleteOnDestruction} {
    MAGNUM_VK_ASSERT_OUTPUT(vkCreateInstance(&info.info(), nullptr, &_handle));

    /* Init the functions */
    flextVkInitInstance(_handle, &_functionPointers);
}

Instance::Instance(NoCreateT): _handle{}, _functionPointers{} {}

Instance::~Instance() {
    if(_handle && !(_flags & ObjectFlag::DeleteOnDestruction))
        _functionPointers.DestroyInstance(_handle, nullptr);
}

VkInstance Instance::release() {
    const VkInstance handle = _handle;
    _handle = nullptr;
    return handle;
}

void Instance::populateGlobalFunctionPointers() {
    flextVkInstance = _functionPointers;
}

}}
