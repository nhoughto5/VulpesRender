#pragma once
#ifndef VULPES_VK_IMAGE_H
#define VULPES_VK_IMAGE_H
#include "vpr_stdafx.h"
#include "ForwardDecl.hpp"
#include "alloc/Allocation.hpp"

namespace vpr {

    /**\deprecated{No longer recommended for use: unmaintained and not updated. }
     * Wraps the common image creation, transfer, and staging methods. Texture derives from this, and so does DepthStencil. Also provides
     * static functions to initialize a passed VkImage handle and Allocation object. 
    *  \ingroup Resources
    */
    class VPR_API Image {
        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;
    public:

        Image(const Device* parent);

        Image(Image&& other) noexcept;
        Image& operator=(Image&& other) noexcept;

        virtual ~Image();

        void Destroy();

        void Create(const VkImageCreateInfo& create_info, const VkMemoryPropertyFlagBits& memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        
        void CreateView(const VkImageViewCreateInfo& info);
        void CreateView(const VkImageAspectFlags& aspect_flags);

        /**Uses a memory barrier to enforce an image layout change. Useful for preparing various images for use before rendering.*/
        void TransitionLayout(const VkImageLayout& initial, const VkImageLayout& final, CommandPool* cmd, VkQueue& queue);

        /** Returns an appropriate memory barrier for the given image, to transfer it between image layouts. */ 
        static VkImageMemoryBarrier GetMemoryBarrier(const VkImage& image, const VkFormat& img_format, const VkImageLayout& prev, const VkImageLayout& next);

        /** Simplified version of the other CreateImage method, that takes an already setup VkImageCreateInfo struct instead of creating one from the given parameters. */
        static void CreateImage(VkImage& dest_image, Allocation& dest_alloc, const Device* parent, const VkImageCreateInfo& create_info, const VkMemoryPropertyFlags & memory_flags);

        const VkImageCreateInfo& CreateInfo() const noexcept;
        const VkImage& vkHandle() const noexcept;
        const VkImageView& View() const noexcept;
        const Allocation& MemoryAllocation() const noexcept;

        VkFormat Format() const noexcept;
        void SetFormat(VkFormat format) noexcept;

        VkImageLayout Layout() const noexcept;
        void SetFinalLayout(VkImageLayout new_layout);
        
    protected:

        const Device* parent;
        const VkAllocationCallbacks* allocators = nullptr;

        VkImageCreateInfo createInfo{ };
        VkImageSubresource subresource{ };
        VkSubresourceLayout subresourceLayout{ };

        VkImage handle{ VK_NULL_HANDLE };
        VkImageView view{ VK_NULL_HANDLE };
        Allocation memoryAllocation;

        VkImageLayout finalLayout{ VK_IMAGE_LAYOUT_MAX_ENUM };
    };

}

#endif // !VULPES_VK_IMAGE_H
