#pragma once

#include <string>
#include <iostream>

#include "ResourceManager/cResourceManager.h"
#include "bgfx/bgfx.h"
#include "bgfx/bgfx.h"
#include "glf"

#define WNDW_WIDTH 1600
#define WNDW_HEIGHT 900

int main()
{
	RM::byteArray* data = RM::cResourceManager::getInstance().getResource("bakedData/testfolder/stringtest.txt");

	std::string test( (char*)data->data() , data->size() );

	std::cout << test;

	bgfx::init();
	GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "Hello, bgfx!", NULL, NULL);
	bgfx::init();

	return 0;
}