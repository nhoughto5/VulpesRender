#pragma once
#ifndef VPR_ALLOCATION_HPP
#define VPR_ALLOCATION_HPP
#include "vpr_stdafx.h"
#include "ForwardDecl.hpp"
#include <memory>

namespace vpr {

    struct AllocationImpl;
    
    /**    
    *    Allocation class represents a singular allocation: can be a private allocation (i.e, only user
    *    of attached DeviceMemory) or a block allocation (bound to sub-region of device memory)
    *   \ingroup Allocation
    */
    class VPR_API Allocation {
    public:
        /** If this is an allocation bound to a smaller region of a larger object, it is a block allocation. 
         *  Otherwise, it has it's own VkDeviceMemory object and is a "PRIVATE_ALLOCATION" type.
        */
        Allocation();
        ~Allocation();
        Allocation(const Allocation&);
        Allocation& operator=(const Allocation&);
        Allocation(Allocation&& other) noexcept;
        Allocation& operator=(Allocation&& other) noexcept;

        /**user_data can be a pointer to anything, but the lifetime of this pointer has to be managed by the user. */
        void Init(MemoryBlock* parent_block, const VkDeviceSize& offset, const VkDeviceSize& alignment, const VkDeviceSize& alloc_size, void* user_data = nullptr);
        /**Changes parent memory block, and specifies a new offset for the memory. Requires re-updating internal data, and is usually part of a defrag routine.*/
        void Update(MemoryBlock* new_parent_block, const VkDeviceSize& new_offset);
        /** \param persistently_mapped: If set, this object will be considered to be always mapped. This will remove any worries about mapping/unmapping the object. */
        void InitPrivate(const uint32_t& type_idx, VkDeviceMemory& dvc_memory, bool persistently_mapped, void* mapped_data, const VkDeviceSize& data_size, void* user_data = nullptr);
        /**Maps the given size and offset of this allocation, and writes the mapped address to the specified pointer.*/
        void Map(const VkDeviceSize& size_to_map, const VkDeviceSize& offset_to_map_at, void** address_to_map_to) const;
        /**Unmaps the allocation - it is vital to call this when finished, as it may otherwise cause issues if left mapped.*/
        void Unmap() const noexcept;

        /**This handle to the underlying VkDeviceMemory object will be a shared handle, when this allocation is non-private.*/
        const VkDeviceMemory& Memory() const;
        VkDeviceSize Offset() const noexcept;
        uint32_t MemoryTypeIdx() const;
        void SetUserData(void* data) const;
        void* GetUserData() const;
        /**Returns true if this allocation is "private" - i.e, it is not part of a larger VkDeviceMemory block and instead received it's own unique VkDeviceMemory instance.*/
        bool IsPrivateAllocation() const noexcept;

        bool operator==(const Allocation& other) const noexcept;

        VkDeviceSize Size;
        VkDeviceSize Alignment;
    private:
        std::unique_ptr<AllocationImpl> impl;
        void* userData{ nullptr };
    };

}

#endif
