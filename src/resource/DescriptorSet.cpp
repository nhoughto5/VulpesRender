#include "vpr_stdafx.h"
#include "resource/DescriptorSet.h"
#include "core/LogicalDevice.h"
#include "resource/DescriptorPool.h"
namespace vulpes {

    DescriptorSet::~DescriptorSet() {
        vkFreeDescriptorSets(device->vkHandle(), descriptorPool->vkHandle(), 1, &descriptorSet);
    }

    void DescriptorSet::AddDescriptorBinding(const VkDescriptorType& descriptor_type, const VkShaderStageFlagBits& shader_stage, const uint32_t& descriptor_binding_loc){
        
        VkDescriptorSetLayoutBinding new_binding {
            descriptor_binding_loc,
            descriptor_type,
            1,
            shader_stage,
            nullptr
        };

        bindings.insert(std::make_pair(descriptor_binding_loc, new_binding));

    }

    void DescriptorSet::AddDescriptorInfo(const VkDescriptorImageInfo& info, const size_t& item_binding_idx) {

        VkWriteDescriptorSet write_descriptor {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            nullptr,
            descriptorSet,
            item_binding_idx,
            0,
            1,
            bindings.at(item_binding_idx).descriptorType,
            &info,
            nullptr,
            nullptr,
        };

        writeDescriptors.insert(std::make_pair(item_binding_idx, write_descriptor));

    }

    void DescriptorSet::AddDescriptorInfo(const VkDescriptorBufferInfo& info, const size_t& item_binding_idx) {
        
        VkWriteDescriptorSet write_descriptor {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            nullptr,
            descriptorSet,
            item_binding_idx,
            0,
            1,
            bindings.at(item_binding_idx).descriptorType,
            nullptr,
            &info,
            nullptr,
        };

        writeDescriptors.insert(std::make_pair(item_binding_idx, write_descriptor));

    }

    void DescriptorSet::CreateLayout() {
       
        size_t num_bindings = bindings.size();
        std::vector<VkDescriptorSetLayoutBinding> bindings_vec;
        for(const auto& entry : bindings) {
            bindings_vec.push_back(entry.second);
        }

        VkDescriptorSetLayoutCreateInfo set_layout_create_info = vk_descriptor_set_layout_create_info_base;
        set_layout_create_info.bindingCount = static_cast<uint32_t>(num_bindings);
        set_layout_create_info.pBindings = bindings_vec.data();

        VkResult result = vkCreateDescriptorSetLayout(device->vkHandle(), &set_layout_create_info, nullptr, &descriptorSetLayout);
        VkAssert(result);


    }

    void DescriptorSet::Allocate(const DescriptorPool* parent_pool) {
        
        descriptorPool = parent_pool;

        VkDescriptorSetAllocateInfo alloc_info = vk_descriptor_set_alloc_info_base;
        alloc_info.descriptorPool = descriptorPool->vkHandle();
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &descriptorSetLayout;

        VkResult result = vkAllocateDescriptorSets(device->vkHandle(), &alloc_info, &descriptorSet);
        allocated = true;

    }

    void DescriptorSet::Update() {

        assert(descriptorPool && allocated);
        
    }

     const std::map<size_t, VkDescriptorSetLayoutBinding>& DescriptorSet::GetBindings() const noexcept {
         return bindings;
     }

}