#include "vpr_stdafx.h"
#include "resource/PipelineCache.hpp"
#include "core/LogicalDevice.hpp"
#include "core/PhysicalDevice.hpp"
#include "BaseScene.hpp"

#ifdef USE_EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
#endif

namespace vulpes {

    PipelineCache::PipelineCache(const Device* _parent, const uint16_t& hash_id) : parent(_parent), 
        createInfo(VkPipelineCacheCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO, nullptr, 0, 0, nullptr }), hashID(hash_id) {
        
        std::string cache_dir = std::string("./rsrc/shader_cache/");
            
#ifdef USE_EXPERIMENTAL_FILESYSTEM
        std::experimental::filesystem::path cache_path(cache_dir);

        if (!std::experimental::filesystem::exists(cache_path)) {
            LOG(INFO) << "Shader cache path didn't exist, creating...";
            std::experimental::filesystem::create_directories(cache_path);
        }
#endif
            
        std::string fname = cache_dir + std::to_string(hash_id) + std::string(".vkdat");
        filename = fname;

        // Attempts to load cache from file: if failed, doesn't matter much.
        LoadCacheFromFile(fname.c_str());

        VkResult result = vkCreatePipelineCache(parent->vkHandle(), &createInfo, nullptr, &handle);
        VkAssert(result);

        BaseScene::PipelineCacheCreated(hash_id);
    }

    PipelineCache::~PipelineCache() {
        
        // Want to make sure cache isn't in use before saving.
        vkDeviceWaitIdle(parent->vkHandle());
        
        VkResult saved = saveToFile();
        VkAssert(saved);

        vkDestroyPipelineCache(parent->vkHandle(), handle, nullptr);

    }

    VkResult PipelineCache::saveToFile() const {

        VkResult result = VK_SUCCESS;
        size_t cache_size;

        if (!parent) {
            LOG(ERROR) << "Attempted to delete/save a non-existent cache!";
            return VK_ERROR_DEVICE_LOST;
        }

        // works like enumerate calls: get size first, then use size to get data.
        result = vkGetPipelineCacheData(parent->vkHandle(), handle, &cache_size, nullptr);
        VkAssert(result);

        if (cache_size != 0) {
            try {
                std::ofstream file(filename, std::ios::out | std::ios::trunc);

                void* cache_data;
                cache_data = malloc(cache_size);

                result = vkGetPipelineCacheData(parent->vkHandle(), handle, &cache_size, cache_data);
                VkAssert(result);

                file.write(reinterpret_cast<const char*>(cache_data), cache_size);
                file.close();

                free(cache_data);
                LOG(INFO) << "Saved pipeline cache data to file successfully";

                return VK_SUCCESS;
            }
            catch (std::ofstream::failure&) {
                LOG(WARNING) << "Saving of pipeline cache to file failed with unindentified exception in std::ofstream.";
                return VK_ERROR_VALIDATION_FAILED_EXT;
            }
        }
        else {
            LOG(WARNING) << "Cache data was reported empty by Vulkan: errors possible.";
            return VK_SUCCESS;
        }
        
    }

    bool PipelineCache::Verify(const std::vector<int8_t>& cache_header) const {

        const uint32_t* header = reinterpret_cast<const uint32_t*>(cache_header.data());
        
        if (header[0] != 32) {
            return false;
        }

        if (header[1] != static_cast<uint32_t>(VK_PIPELINE_CACHE_HEADER_VERSION_ONE)) {
            return false;
        }

        auto& physical_device = parent->GetPhysicalDevice();

        if (header[2] != physical_device.Properties.vendorID) {
            return false;
        }

        if (header[3] != physical_device.Properties.deviceID) {
            return false;
        }

        uint8_t cache_uuid[VK_UUID_SIZE] = {};
        memcpy(cache_uuid, cache_header.data() + 16, VK_UUID_SIZE);

        if (memcmp(cache_uuid, physical_device.Properties.pipelineCacheUUID, sizeof(cache_uuid)) != 0) {
            LOG(INFO) << "Pipeline cache UUID incorrect, requires rebuilding.";
            return false;
        }

        return true;
    }

    void PipelineCache::LoadCacheFromFile(const char * _filename) {
        /*
        check for pre-existing cache file.
        */
        std::ifstream cache(_filename, std::ios::in);
        if (cache) {

            // get header (4 uint32_t, 16 int8_t) = (32 int8_t)
            std::vector<int8_t> header(64);
            cache.get(reinterpret_cast<char*>(header.data()), 64);

            // Check to see if header data matches current device.
            if (Verify(header)) {
                LOG(INFO) << "Found valid pipeline cache data with ID # " << std::to_string(hashID) << " .";
                std::string cache_str((std::istreambuf_iterator<char>(cache)), std::istreambuf_iterator<char>());
                uint32_t cache_size = static_cast<uint32_t>(cache_str.size() * sizeof(char));
                createInfo.initialDataSize = cache_size;
                createInfo.pInitialData = cache_str.data();
            }
            else {
                LOG(INFO) << "Pre-existing cache file isn't valid: creating new pipeline cache.";
                createInfo.initialDataSize = 0;
                createInfo.pInitialData = nullptr;
            }
        }
    }

    const VkPipelineCache& PipelineCache::vkHandle() const{
        return handle;
    }

}
