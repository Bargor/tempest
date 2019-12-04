// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "gpu_info.h"

#include <GL/glew.h>

namespace tst {
namespace engine {
    namespace opengl {

        gpu_info::device_vendor get_device_vendor(const GLubyte* vendor) {
            std::string vendorString(reinterpret_cast<const char*>(vendor));
            if (vendorString == "Intel") {
                return gpu_info::device_vendor::intel;
            } else if (vendorString == "Nvidia") {
                return gpu_info::device_vendor::nvidia;
            } else if (vendorString == "AMD") {
                return gpu_info::device_vendor::amd;
            }
            return gpu_info::device_vendor::other;
        }

        gpu_info::gpu_info() {
            GLint major, minor, patch = 0;

            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            versionMajor = major;
            versionMinor = minor;
            versionPatch = patch;
            deviceVendor = get_device_vendor(glGetString(GL_VENDOR));
            deviceType = deviceVendor == device_vendor::intel ? device_type::integrated : device_type::discrete;
            deviceName = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            
            GLint framebufferHeight, framebufferWidth, colorAttachments, textureSize, vertexAttribs, vertexAttribBindings, elementIndex;

            glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &framebufferWidth);
            glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &framebufferHeight);
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorAttachments);
            glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &textureSize);
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttribs);
            glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &vertexAttribBindings);
            glGetIntegerv(GL_MAX_ELEMENT_INDEX, &elementIndex);

            maxFramebufferWidth = framebufferWidth;
            maxFramebufferHeight = framebufferHeight;
            maxColorAttachments = colorAttachments;
            maxImageDimension2D = textureSize;
            maxVertexInputAttributes = vertexAttribs;
            maxVertexInputBindings = vertexAttribBindings;
            maxIndexValue = elementIndex;
        }
    }
}
}