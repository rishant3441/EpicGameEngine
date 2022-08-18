//
// Created by Rishan Thangaraj on 7/27/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    namespace Debug
    {
        class EPICGAMEENGINE_API Log
        {
        public:
            static void Init();
            static void Shutdown();

            template <typename... Args>
            inline static void LogInfo(spdlog::format_string_t<Args...> text, Args&&... args)
            {
                spdlog::info(text, std::forward<Args>(args)...);

            }
            template <typename... Args>
             inline static void LogWarn(spdlog::format_string_t<Args...> text, Args&&... args)
             {
                 spdlog::info(text, std::forward<Args>(args)...);
             }
            template <typename... Args>
            inline static void LogError(spdlog::format_string_t<Args...> text, Args&&... args)
            {
                spdlog::error(text, std::forward<Args>(args)...);
            }
        };
    };
}