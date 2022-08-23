//
// Created by Rishan Thangaraj on 8/21/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <string.h>
#include <iostream>

namespace EpicGameEngine
{
    namespace CoreLib
    {
        struct BumpMemPool
        {
            void* startPtr = nullptr;
            void* offset = nullptr;
            void* endPtr = nullptr;

            size_t size = 0;
            size_t filledSize = 0;
        };

        struct BumpAllocator
        {
            BumpAllocator(size_t size = 1024)
            {
               pool = BumpMemPool();
               Realloc(size);
            }

            BumpAllocator(BumpMemPool& newPool, size_t size = 1024)
            {
                pool = newPool;

                // Creates a new self-managed pool.
                if (pool.startPtr == nullptr)
                {
                    Realloc(size);
                }
            }
            ~BumpAllocator()
            {
                free(pool.startPtr);
            }

            template<typename T, typename... Args>
            T* Allocate(size_t amountOfType = 1, Args&&... args)
            {
                if (pool.size - pool.filledSize < sizeof(T) * amountOfType)
                {
                    ReallocAndCopy((pool.size * 2) + sizeof(T) * amountOfType);
                }

                void* newPtr = pool.offset;
                T *t = new(newPtr) T(std::forward<Args>(args)...);
                pool.offset = (char*) pool.offset + sizeof(T) * amountOfType;
                pool.filledSize += sizeof(T) * amountOfType;
                return t;
            }

            template<typename T>
            void DeAllocate(T* p, std::size_t n)
            {
                // Not a proper deallocation due to how simple this type of allocator is
                memset(p, 0, n);
            }

            void Realloc(std::size_t newSize = 1024)
            {
                pool.size = newSize;
                pool.filledSize = 0;

                pool.offset = malloc(pool.size);
                pool.startPtr = pool.offset;
                pool.endPtr = (char*) pool.startPtr + pool.size;
            }

            void ReallocAndCopy(std::size_t newSize)
            {
                void* newAlloc = malloc(newSize);
                memcpy(newAlloc, pool.startPtr, pool.size);
                free(pool.startPtr);
                pool.size = newSize;
                pool.startPtr = newAlloc;
            }

            void Clear()
            {
                memset(pool.startPtr, 0, pool.size);

                pool.filledSize = 0;

                pool.offset = pool.startPtr;
                pool.endPtr = (char*) pool.startPtr + pool.size;
            }
        private:
            BumpMemPool pool;
        };
    }
}
