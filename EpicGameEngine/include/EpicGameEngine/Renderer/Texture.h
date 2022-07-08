//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

namespace EpicGameEngine
{
    class Texture
    {
    public:
        Texture()
        {
            texture = nullptr;
        }
        ~Texture() = default;

        int LoadImage(const std::string& filePath);

        GPU_Image* GetTexture() { return texture; }
        operator GPU_Image*() { return texture; }
        operator GPU_Image*() const { return texture; }

    private:
        GPU_Image* texture = nullptr;
    };
}

