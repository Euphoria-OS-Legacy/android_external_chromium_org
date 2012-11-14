// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/resource.h"
#include "third_party/khronos/GLES2/gl2ext.h"

namespace cc {

void Resource::setDimensions(const gfx::Size& size, GLenum format)
{
    m_size = size;
    m_format = format;
}

size_t Resource::bytes() const
{
    if (m_size.IsEmpty())
        return 0u;

    return memorySizeBytes(m_size, m_format);
}

size_t Resource::bytesPerPixel(GLenum format)
{
    unsigned int componentsPerPixel = 0;
    unsigned int bytesPerComponent = 1;
    switch (format) {
    case GL_RGBA:
    case GL_BGRA_EXT:
        componentsPerPixel = 4;
        break;
    case GL_LUMINANCE:
        componentsPerPixel = 1;
        break;
    default:
        NOTREACHED();
    }
    return componentsPerPixel * bytesPerComponent;
}

size_t Resource::memorySizeBytes(const gfx::Size& size, GLenum format)
{
    return bytesPerPixel(format) * size.width() * size.height();
}

}  // namespace cc