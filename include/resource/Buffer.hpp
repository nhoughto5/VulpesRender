#pragma once
#ifndef VULPES_VK_BUFFER_H
#define VULPES_VK_BUFFER_H
#include "vpr_stdafx.h"
#include "../ForwardDecl.hpp"
#include "../resource/Allocator.hpp"
namespace vulpes {

	class Buffer {
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;
	public:

		Buffer(const Device* parent = nullptr);

		~Buffer();

		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;

		void CreateBuffer(const VkBufferUsageFlags& usage_flags, const VkMemoryPropertyFlags& memory_flags, const VkDeviceSize& size);

		void Destroy();

		void CopyToMapped(void* data, const VkDeviceSize& size, const VkDeviceSize& offset);
		void CopyTo(void * data, VkCommandBuffer & transfer_cmd, const VkDeviceSize& copy_size, const VkDeviceSize& copy_offset);
		void CopyTo(void* data, CommandPool* cmd_pool, const VkQueue & transfer_queue, const VkDeviceSize& size, const VkDeviceSize& offset);

		void Update(VkCommandBuffer& cmd, const VkDeviceSize& data_sz, const VkDeviceSize& offset, const void* data);

		void Map(const VkDeviceSize& offset = 0);
		void Unmap();

		const VkBuffer& vkHandle() const noexcept;
		VkBuffer& vkHandle() noexcept;

		VkDescriptorBufferInfo GetDescriptor() const noexcept;

		VkDeviceSize Size() const noexcept;

		// Due to alignment requirements, the size reported by Size() might not reflect the size of the data uploaded.
		// Use this when we are checking against data uploaded (for changes)
		VkDeviceSize InitDataSize() const noexcept;

		void* MappedMemory = nullptr;

		static void CreateStagingBuffer(const Device* dvc, const VkDeviceSize& size, VkBuffer& dest, Allocation& dest_memory_range);
		static void DestroyStagingResources(const Device* device);

	protected:

		static std::vector<std::pair<VkBuffer, Allocation>> stagingBuffers;

		void createStagingBuffer(const VkDeviceSize& size, const VkDeviceSize& offset, VkBuffer& staging_buffer, Allocation& dest_memory_range);

		const Device* parent;
		const VkAllocationCallbacks* allocators = nullptr;
		VkBuffer handle;
		VkBufferCreateInfo createInfo;
		VkBufferView view;
		Allocation memoryAllocation;
		VkDeviceSize size;
		VkDeviceSize dataSize;
	};
	
}

#endif // !VULPES_VK_BUFFER_H