#include "vpr_stdafx.h"
#include "render/Renderpass.h"
#include "core/LogicalDevice.h"
namespace vulpes {
	Renderpass::Renderpass(const Device* dvc, const VkRenderPassCreateInfo & create_info) : parent(dvc), createInfo(create_info) {
		VkResult result = vkCreateRenderPass(dvc->vkHandle(), &create_info, allocators, &handle);
		VkAssert(result);
	}

	Renderpass::~Renderpass(){
		Destroy();
	}

	void Renderpass::Destroy(){
		if (handle != VK_NULL_HANDLE) {
			vkDestroyRenderPass(parent->vkHandle(), handle, allocators);
			handle = VK_NULL_HANDLE;
		}
	}

	const VkRenderPass & Renderpass::vkHandle() const noexcept{
		return handle;
	}

	Renderpass::operator VkRenderPass() const noexcept{
		return handle;
	}

	const VkRenderPassCreateInfo & Renderpass::CreateInfo() const noexcept{
		return createInfo;
	}

}
