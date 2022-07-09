//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Renderer/Texture.h>
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    /// Loads an image from the filepath. Returns non-zero if the file doesn't load properly.
    int Texture::LoadImage(const std::string& filePath)
    {
        texture = GPU_LoadImage(filePath.c_str());
        if (texture != nullptr)
            return 0;
        else
            return -1;
    }
}