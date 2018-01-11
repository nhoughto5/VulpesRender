#pragma once
// Push this only for external libraries.
#pragma warning(push, 0)
#ifdef _WIN32
#define NOMINMAX
#endif //!_WIN32
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <cassert>
#include <mutex>

#if defined(_WIN32) || defined(__linux__)
#include <vulkan/vulkan.h>
#elif defined (__APPLE__)
#include "MoltenVK/mvk_vulkan.h"
#else
#pragma message("No valid platform detected for Vulkan!")
#endif

#if defined(__GNUC__)
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#endif

#ifdef VPR_DLL
#define VPR_API IMPORT
#elif defined(VPR_BUILD_DLL)
#define VPR_API EXPORT
#else
#define VPR_API
#endif

#if defined (VPR_DLL) || defined (VPR_BUILD_DLL)
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_VULKAN

#pragma warning(pop)

#include "common/CreateInfoBase.hpp"
#include "common/vkAssert.hpp"


