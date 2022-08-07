//
// Created by Rishan Thangaraj on 8/1/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

namespace EpicGameEngine
{
    class FileDialogs
    {
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
}