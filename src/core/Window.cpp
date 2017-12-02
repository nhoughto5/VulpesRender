#include "vpr_stdafx.h"
#include "core/Window.hpp"
#include <imgui.h>
#include "core/Instance.hpp"


namespace vpr {

    Window::Window(const Instance* instance, const uint32_t& _width, const uint32_t& _height) : parent(instance), width(_width), height(_height) {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        createWindow();
    }

    Window::~Window() {
        vkDestroySurfaceKHR(parent->vkHandle(), surface, nullptr);
    }

    void Window::createWindow() {

        std::string window_title = Instance::GraphicsSettings.ApplicationName;
        if (!Instance::GraphicsSettings.EnableFullscreen) {
            window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), window_title.c_str(), nullptr, nullptr);
        }
        else {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            window = glfwCreateWindow(mode->width, mode->height, window_title.c_str(), monitor, nullptr);
        }
        ImGuiIO& io = ImGui::GetIO();
#ifdef _WIN32
        io.ImeWindowHandle = glfwGetWin32Window(window);
#endif
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

        glfwSetWindowSizeCallback(window, ResizeCallback);
        createInputHandler(); // TODO: Input handler class.
        setExtensions();

    }

    GLFWwindow* Window::glfwWindow() noexcept {
        return window;
    }

    void Window::setExtensions() {
#ifdef __APPLE__
        int status = glfwVulkanSupported();
        if(status != GLFW_TRUE) {
            std::cerr << "Couldn't find Vulkan support w/ GLFW! Check link settings for MoltenVK.";
        }
#endif

        uint32_t extension_count = 0;
        const char** extension_names;
        extension_names = glfwGetRequiredInstanceExtensions(&extension_count);
        for(uint32_t i = 0; i < extension_count; ++i) {
            extensions.emplace_back(extension_names[i]);
        }
    }

    const std::vector<const char*>& Window::Extensions() const noexcept {
        return extensions;
    }

    const VkSurfaceKHR & Window::vkSurface() const noexcept {
        return surface;
    }

    glm::ivec2 Window::GetWindowSize() const noexcept{
        glm::ivec2 result(0, 0);
        glfwGetWindowSize(const_cast<GLFWwindow*>(window), &result.x, &result.y);
        return result;
    }

    void Window::CreateSurface() {
        VkResult err = glfwCreateWindowSurface(parent->vkHandle(), window, nullptr, &surface);
        VkAssert(err);
        LOG(INFO) << "Created window surface.";
    }

    void Window::createInputHandler() {
        InputHandler = std::make_unique<input_handler>(this);
    }

void Window::ResizeCallback(GLFWwindow* window, int width, int height) {

    ImGuiIO& io = ImGui::GetIO();
    if(width == 0 || height == 0) {
        LOG(WARNING) << "Resize callback called with zero width or height for window. Attempting to fall back on last stored value...";
        width = static_cast<int>(io.DisplaySize.x);
        height = static_cast<int>(io.DisplaySize.y);        
    }
    else {
        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

    Instance::VulpesState.NeedWindowResize = true;

}

    void Window::SetWindowUserPointer(void* user_ptr) {
        glfwSetWindowUserPointer(window, user_ptr);
    }

}