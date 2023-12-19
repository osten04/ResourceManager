#pragma once

#include <string>
#include <iostream>

#include "ResourceManager/cResourceManager.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#define WNDW_WIDTH 1600
#define WNDW_HEIGHT 900

int main()
{
    BX_CONFIG_DEBUG;

	RM::byteArray* data = RM::cResourceManager::getInstance().getResource("bakedData/testfolder/stringtest.txt");

	std::string test( (char*)data->data() , data->size() );

	std::cout << test;

	glfwInit();
    GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "Hello, bgfx!", NULL, NULL);

	bgfx::PlatformData pd;
    pd.nwh = glfwGetWin32Window(window);
    bgfx::setPlatformData(pd);

    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
    bgfxInit.resolution.width = WNDW_WIDTH;
    bgfxInit.resolution.height = WNDW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, WNDW_WIDTH, WNDW_HEIGHT);

    unsigned int counter = 0;
    while( true ) {   
        bgfx::frame();
        counter++;
    }

	return 0;
}