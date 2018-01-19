#include "vpr_stdafx.h"
#include "render/Framebuffer.hpp"
#include "core/LogicalDevice.hpp"

namespace vpr {

    Framebuffer::~Framebuffer() {
        Destroy();
    }

    Framebuffer::Framebuffer(const Device * _parent, const VkFramebufferCreateInfo & create_info) : parent(_parent) {
        VkResult result = vkCreateFramebuffer(parent->vkHandle(), &create_info, nullptr, &handle);
        VkAssert(result);
    }

    Framebuffer::Framebuffer(Framebuffer&& other) noexcept : handle(std::move(other.handle)), parent(std::move(other.parent)) {}

    Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept {
        handle = std::move(other.handle);
        parent = std::move(other.parent);
        other.handle = VK_NULL_HANDLE;
        return *this;
    }
    
    const VkFramebuffer & Framebuffer::vkHandle() const noexcept{
        return handle;
    }

    void Framebuffer::Destroy(){
        vkDestroyFramebuffer(parent->vkHandle(), handle, nullptr);
    }


}