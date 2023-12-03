#include <EpicGameEngine/Renderer/RendererAPI.h>
#include <EpicGameEngine/Renderer/Texture.h>
#include <EpicGameEngine/Renderer/OpenGL/OpenGLTexture.h>

namespace EpicGameEngine {

    Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
    {

        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "None not implemented!" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLTexture2D>(specification);
        }
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "None not implemented!" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLTexture2D>(path);
        }
        return nullptr;
    }

}