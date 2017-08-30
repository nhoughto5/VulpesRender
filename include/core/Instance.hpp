#pragma once
#ifndef VULPES_VK_INSTANCE_H
#define VULPES_VK_INSTANCE_H
#include "vpr_stdafx.h"
#include "../ForwardDecl.hpp"
#include "InstanceConfig.hpp"
#include "../util/Camera.hpp"
#include "../util/Arcball.hpp"
#include "../gui/imguiWrapper.hpp"

namespace vulpes {

	struct PhysicalDeviceFactory;

	class Instance {
		Instance(const Instance&) = delete;
		Instance(Instance&&) = delete;
		Instance& operator=(const Instance&) = delete;
		Instance& operator=(Instance&&) = delete;
	public:
		
		Instance() = default;

		virtual void SetupSurface() = 0;
		virtual void SetupPhysicalDevices();
		void UpdateMovement(const float& dt);
		virtual ~Instance();

		// Allocators 
		static const VkAllocationCallbacks* AllocationCallbacks;

		const VkInstance& vkHandle() const;
		operator VkInstance() const;
		const VkSurfaceKHR GetSurface() const;
		const VkPhysicalDevice& GetPhysicalDevice() const noexcept;
		// Debug callbacks
		VkDebugReportCallbackEXT errorCallback, warningCallback, perfCallback, infoCallback, vkCallback;
		bool validationEnabled;
		
		std::vector<const char*> extensions;
		std::vector<const char*> layers;

		static std::array<bool, 1024> keys;
		static std::array<bool, 3> mouse_buttons;
		static float LastX, LastY;
		static float mouseDx, mouseDy;
		static float mouseScroll;
		float frameTime;
		static bool cameraLock;

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		
		PhysicalDeviceFactory* physicalDeviceFactory;
		PhysicalDevice* physicalDevice;

		glm::mat4 GetViewMatrix() const noexcept;
		glm::mat4 GetProjectionMatrix() const noexcept;
		glm::vec3 GetCamPos() const noexcept;

		glm::mat4 projection;

		void SetCamPos(const glm::vec3& pos);

		void SetCameraLookAt(const glm::vec3 & new_look_at);

		static void MouseDrag(const int& button, const float& rot_x, const float& rot_y);
		static void MouseScroll(const int& button, const float& zoom_delta);
		static void MouseDown(const int& button, const float & x, const float & y);
		static void MouseUp(const int& button, const float & x, const float & y);

        static cfg::vulpesInstanceInfo VulpesInstanceConfig;

	protected:
		static Camera cam;
		static Arcball arcball;
		VkInstance handle;
		uint32_t width, height;
		VkInstanceCreateInfo createInfo;
	};


	class InstanceGLFW : public Instance {
	public:

		InstanceGLFW(VkInstanceCreateInfo create_info, const bool& enable_validation, const uint32_t& width = DEFAULT_WIDTH, const uint32_t& height = DEFAULT_HEIGHT);

		void CreateWindowGLFW(const bool& fullscreen_enabled = false);

		virtual void SetupSurface() override;

		GLFWwindow* Window;

		static void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int code);
		static void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
		static void KeyboardCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);
		static void CharCallback(GLFWwindow *, unsigned int c);
		static void SetClipboardCallback(void* window, const char* text);
		static const char* GetClipboardCallback(void* window);
		void Refresh();
		static void ResizeCallback(GLFWwindow* window, int width, int height);
	};

}

#endif // !INSTANCE_H