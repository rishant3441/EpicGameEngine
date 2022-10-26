//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once
#include <SDL_gpu.h>
#include <EpicGameEngine/Debug.h>

namespace EpicGameEngine
{
    class Texture
    {
    public:
        Texture()
        {
            texture = GPU_CreateImage(32, 32, GPU_FORMAT_RGBA);
            if (texture == nullptr)
            {
                Debug::Log::LogError("TEXTURE: Texture is nullptr");
            }
        }
        Texture(int w, int h)
        {
            texture = GPU_CreateImage(w, h, GPU_FORMAT_RGBA);
            if (texture == nullptr)
            {
                Debug::Log::LogError("TEXTURE: Texture is nullptr");
            }
        }
        ~Texture()
        {
            GPU_FreeImage(texture);
        }

#undef LoadImage // Wonky Win32 API Interference
        int LoadImage(const std::string& filePath);

        // Get Native Texture Handle/ID
        GPU_Image* GetTexture() { return texture; }
        void* GetTextureHandle() { return (void*) GPU_GetTextureHandle(texture); }
        operator GPU_Image*() { return texture; }
        operator GPU_Image*() const { return texture; }

    private:
        GPU_Image* texture;
    };
}

