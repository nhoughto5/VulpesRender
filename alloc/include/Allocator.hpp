#pragma once
#ifndef VULPES_VK_ALLOCATOR_H
#define VULPES_VK_ALLOCATOR_H
#include "vpr_stdafx.h"
#include "ForwardDecl.hpp"
#include <memory>

namespace vpr {

    /** The allocator module encompasses everything required for the operation/use of this project's rather
     *  complex GPU memory management subsystem. In Vulkan, allocating VkDeviceMemory objects for every single 
     *  individual resource requiring GPU memory is wasteful, and not practical: there is a limit in each Vulkan 
     *  implementation to just how many allocations can exist at one time (sometimes as low as 1024). Thus, we will
     *  instead allocate large chunks of memory when a previously unallocated memory type is requested: from there, 
     *  resources (like buffers, images, etc) will bind to subregions of this larger memory object. Binding is much
     *  faster than allocation, and it is also much quicker to de-allocate resources since this only involves registering
     *  a newly freed location in this subsystem. 
     * 
     *  The allocator object is spawned as a member of the LogicalDevice class, but it can be accessed by anyone that can access
     *  a logical device. Its relatively safe to access, and should be thread-safe. Utility methods exist to simplify the creation
     *  and allocation of VkImage and VkBuffer objects as much as possible, as well.
     *  
     *  This module also is minimally complete, at best. It is robust enough to not fail in common usage, and currently has functioned 
     *  wonderfully in most tasks thrown at it. However, the todo list details stuff that still needs to be done (e.g, further splitting pools and 
     *  creating some kind of defragmentation system).
     * 
     *  Lastly, huge credit to GPU-Open as this is primarily just a slightly more object-oriented/"Modern C++" styled implementation of their *excellent*
     *  memory allocator. This would not have been possible without their work: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
     * 
     *  \defgroup Allocation
     */

    struct AllocatorImpl;
    struct AllocationRequirements;

    enum class AllocationType : uint32_t {
        INVALID = 0,
        Buffer = 1,
        ImageLinear = 2,
        ImageTiled = 3,
        Unknown = 4
    };

    /** The primary interface and class of this subsystem. This object is responsible for creating resources when requested, managing memory,
     *  checking integrity of memory, and cleaning up after itself and when deallocation has been requested.
     *  \ingroup Allocation
     */
    class VPR_API Allocator {
        Allocator(const Allocator&) = delete;
        Allocator(Allocator&&) = delete;
        Allocator& operator=(const Allocator&) = delete;
        Allocator& operator=(Allocator&&) = delete;
    public:

        enum class allocation_extensions {
            None = 0,
            DedicatedAllocations = 1
        };

        /** Allocator is attached to a single logical device and physical device. Extensions for improved dedicated allocations are set based on parameter.*/
        Allocator(const VkDevice& parent_device, const VkPhysicalDevice& physical_device, allocation_extensions dedicated_alloc_enabled);
        ~Allocator();

        /**Destroys and re-creates the allocator subsystem. Pre-existing allocations are destroyed and not rebuilt - the allocator is effectively "zeroed out".*/
        void Recreate();
        const VkDevice& DeviceHandle() const noexcept;
        /**Called by clients to allocate memory into the passed Allocation reference. Usually wiser to just use AllocateForBuffer or AllocateForImage.*/
        VkResult AllocateMemory(const VkMemoryRequirements& memory_reqs, const AllocationRequirements& alloc_details, const AllocationType& suballoc_type, Allocation& dest_allocation);
        /**Releases the memory used by the given Allocation.*/
        void FreeMemory(const Allocation* memory_to_free);

        /**Allocates memory for an image, using given handle to get requirements. Allocation information is written to dest_memory_range, so it can then be used to bind the resources together.*/
        VkResult AllocateForImage(VkImage& image_handle, const AllocationRequirements& details, const AllocationType& alloc_type, Allocation& dest_allocation);
        /**Much like AllocateForImage: uses given handle to get requirements, writes details of allocation ot given range, making memory valid for binding.*/
        VkResult AllocateForBuffer(VkBuffer& buffer_handle, const AllocationRequirements& details, const AllocationType& alloc_type, Allocation& dest_allocation);

    private:
        friend class DebugVisualization;
        friend struct DebugVisualizationImpl;
        std::unique_ptr<AllocatorImpl> impl;
    };

    VPR_API void SetLoggingRepository_VprAlloc(void* storage_ptr);
    VPR_API void* GetLoggingRepository_VprAlloc();

}

#endif // !VULPES_VK_ALLOCATOR_H
