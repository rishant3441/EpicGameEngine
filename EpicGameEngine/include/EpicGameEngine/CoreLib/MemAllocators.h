//
// Created by Rishan Thangaraj on 8/21/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <string.h>
#include <iostream>

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
           realloc(size);
        }

        BumpAllocator(BumpMemPool& newPool, size_t size = 1024)
        {
            pool = newPool;

            // Creates a new self-managed pool.
            if (pool.startPtr == nullptr)
            {
                realloc(size);
            }
        }
        ~BumpAllocator()
        {
            free(pool.startPtr);
        }

        template<typename T, typename... Args>
        T* allocate(size_t amountOfType = 1, Args&&... args)
        {
            if (pool.size - pool.filledSize < sizeof(T) * amountOfType)
            {
                realloc_and_copy((pool.size * 2) + sizeof(T) * amountOfType);
            }

            void* newPtr = pool.offset;
            T *t = new(newPtr) T(std::forward<Args>(args)...);
            pool.offset = (char*) pool.offset + sizeof(T) * amountOfType;
            pool.filledSize += sizeof(T) * amountOfType;
            return t;
        }

        template<typename T>
        void deallocate(T* p, std::size_t n)
        {
            // Not a proper deallocation due to how simple this type of allocator is
            memset(p, 0, n);
        }

        void realloc(std::size_t newSize = 1024)
        {
            pool.size = newSize;
            pool.filledSize = 0;

            pool.offset = malloc(pool.size);
            pool.startPtr = pool.offset;
            pool.endPtr = (char*) pool.startPtr + pool.size;
        }

        void realloc_and_copy(std::size_t newSize)
        {
            void* newAlloc = malloc(newSize);
            memcpy(newAlloc, pool.startPtr, pool.size);
            free(pool.startPtr);
            pool.size = newSize;
            pool.startPtr = newAlloc;
        }

        void clear()
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
