//
// Created by Rishan Thangaraj on 11/4/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Args.h"
#include <CLI11.hpp>

namespace CoreLib
{
    Args::Args(int argc, char** argv)
    {
        CLI::App app{"Epic Game Engine"};

        app.add_option("--pf", projectFile, "Project File Path (.ege)");
        app.add_option("--dll", monoDLL, "Script Engine DLL (.dll)");

        app.parse(argc, argv);
    }
}