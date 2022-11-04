//
// Created by Rishan Thangaraj on 11/4/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>

namespace CoreLib
{
    class Args
    {
    public:
        Args(int argc, char** argv);

    private:
        std::string projectFile;
        std::string monoDLL;
    };
}

