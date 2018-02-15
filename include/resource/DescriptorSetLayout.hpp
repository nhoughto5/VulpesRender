#pragma once
#ifndef VPR_DESCRIPTOR_SET_LAYOUT_HPP
#define VPR_DESCRIPTOR_SET_LAYOUT_HPP
#include "vpr_stdafx.h"
#include "ForwardDecl.hpp"
#include <vulkan/vulkan.h>
#include <map>

namespace vpr {

    class VPR_API DescriptorSetLayout {
    public:

        DescriptorSetLayout(const Device* _dvc);
        ~DescriptorSetLayout();

        /** Specifies that a descriptor of the given type be accessible from the given stage, and sets the index it will be accessed at in the shader.
        */
        void AddDescriptorBinding(const VkDescriptorType& descriptor_type, const VkShaderStageFlags& shader_stage, const uint32_t& descriptor_binding_loc) noexcept;
        
        const VkDescriptorSetLayout& vkHandle() const noexcept;
    private:
        mutable bool ready = false;
        void create() const;
        const Device* device;
        mutable VkDescriptorSetLayout handle;
        std::map<size_t, VkDescriptorSetLayoutBinding> bindings;
    };

}

#endif