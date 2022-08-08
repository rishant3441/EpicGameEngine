cmake_minimum_required(VERSION 3.16)
project(EpicGameEngine-Scripting VERSION 0.1.0 LANGUAGES CSharp)
include(CSharpUtilities)
add_library(EpicGameEngine-Scripting SHARED
            Source/Main.cs
            Source/Camera.cs
        )
