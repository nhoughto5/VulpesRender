#pragma once
#ifndef VULPES_VERTEX_T_HPP
#define VULPES_VERTEX_T_HPP

#include "vpr_stdafx.h"
#include "glm/gtx/hash.hpp"

namespace vulpes {

    /** \ingroup Objects
    */
    struct vertex_t {
        vertex_t() = default;
        ~vertex_t() = default;
        vertex_t(const vertex_t&) = default;

        vertex_t(const glm::vec3& _pos, const glm::vec3& _norm = glm::vec3(0.0f), const glm::vec2 _uv = glm::vec2(0.0f)) :
            pos(_pos), normal(_norm), uv(_uv) {}

        vertex_t& operator=(const vertex_t&) = default;

        vertex_t& operator=(vertex_t&& other) {
            pos = std::move(other.pos);
            normal = std::move(other.normal);
            uv = std::move(other.uv);
            return *this;
        }

        vertex_t(vertex_t&& other) : pos(other.pos), normal(other.normal), uv(other.uv) {}
        
        glm::vec3 pos = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 uv = glm::vec2(0.0f);

		bool operator==(const vertex_t& other) const noexcept {
			return (pos == other.pos) && (normal == other.normal) && (uv == other.uv);
		}
        
        constexpr static VkVertexInputBindingDescription bindingDescription{
            0, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2), VK_VERTEX_INPUT_RATE_VERTEX
        };

        constexpr static std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{
            VkVertexInputAttributeDescription{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 },
            VkVertexInputAttributeDescription{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(glm::vec3) },
            VkVertexInputAttributeDescription{ 2, 0, VK_FORMAT_R32G32_SFLOAT, sizeof(glm::vec3) * 2 }
        };
    };
}

namespace std {
    /** Hash method ovverride so that we can use vulpes::vertex_t in standard library containers. Used with tinyobj to remove/avoid duplicated vertices. \ingroup Objects */
    template<>
    struct hash<vulpes::vertex_t> {
        size_t operator()(const vulpes::vertex_t& vert) const {
            return (hash<glm::vec3>()(vert.pos)) ^ (hash<glm::vec3>()(vert.normal) << 1) ^ (hash<glm::vec2>()(vert.uv) << 4);
        }
    };
}

#endif //!VULPES_VERTEX_T_HPP