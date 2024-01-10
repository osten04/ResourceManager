#include <string>
#include <iostream>

#include "ResourceManager/cResourceManager.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

#define WNDW_WIDTH 1600
#define WNDW_HEIGHT 900

bgfx::ShaderHandle loadShader(const char* FILENAME)
{
    const char* shaderPath = "???";

    switch ( bgfx::getRendererType() ) 
    {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
        case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
    }

    size_t shaderLen = strlen(shaderPath);
    size_t fileLen = strlen(FILENAME);
    char* filePath = (char*)malloc(shaderLen + fileLen);
    memcpy(filePath, shaderPath, shaderLen);
    memcpy(&filePath[shaderLen], FILENAME, fileLen);

    FILE* file = fopen(FILENAME, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

int mainGame()
{
	RM::byteArray data = *RM::cResourceManager::getInstance().getResource( "bakedData/testfolder/stringtest.txt" );
    std::cout << std::string( (char*)data.data(), data.size() ) << '\n';

    glfwInit();
    GLFWwindow* window = glfwCreateWindow( WNDW_WIDTH, WNDW_HEIGHT, "Hello, bgfx!", NULL, NULL );

    bgfx::PlatformData pd;
    pd.nwh = glfwGetWin32Window( window );

    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::OpenGL; // Automatically choose a renderer.
    bgfxInit.resolution.width  = WNDW_WIDTH;
    bgfxInit.resolution.height = WNDW_HEIGHT;
    bgfxInit.resolution.reset  = BGFX_RESET_VSYNC;
    bgfxInit.platformData = pd;

    bgfx::init( bgfxInit );

    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355F, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, WNDW_WIDTH, WNDW_HEIGHT );

    unsigned int counter = 0;
    while (true) {
        bgfx::frame();
        counter++;
    }

	return 0;
}

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/string.h>

entry_imple