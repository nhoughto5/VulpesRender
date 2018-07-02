#pragma once
#ifndef VULPES_VK_FRAMEBUFFER_H
#define VULPES_VK_FRAMEBUFFER_H
#include "vpr_stdafx.h"
#include "ForwardDecl.hpp"

namespace vpr {

    /** The Framebuffer class merely handles lifetime of a VkFramebuffer object. All important information on
    *   how to setup this class is provided by the VkFramebufferCreateInfo struct in the constructor.
    *   \ingroup Rendering
    */
    class VPR_API Framebuffer {
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
    public:

        virtual ~Framebuffer();

        Framebuffer(const VkDevice& parent, const VkFramebufferCreateInfo& create_info);
        Framebuffer& operator=(Framebuffer&& other) noexcept;
        Framebuffer(Framebuffer&& other) noexcept;
        const VkFramebuffer& vkHandle() const noexcept;
        
        void Destroy();

    protected:

        VkDevice parent;
        VkFramebuffer handle;
    };

}
#endif // !VULPES_VK_FRAMEBUFFER_H