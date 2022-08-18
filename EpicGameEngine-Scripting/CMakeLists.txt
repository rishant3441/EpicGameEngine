cmake_minimum_required(VERSION 3.16)
project(EpicGameEngine-Scripting VERSION 0.1.0 LANGUAGES CSharp)
include(CSharpUtilities)

add_library(EpicGameEngine-Scripting SHARED
            Source/EpicGameEngine/InternalCalls.cs
            Source/EpicGameEngine/Vector3.cs
            Source/EpicGameEngine/GameObjects/GameObject.cs
            Source/EpicGameEngine/Input/Keycodes.cs
            Source/EpicGameEngine/Input/Mousecodes.cs
            Source/EpicGameEngine/Input/Input.cs
        )

set_target_properties(EpicGameEngine-Scripting PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
