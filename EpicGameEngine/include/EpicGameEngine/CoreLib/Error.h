//
// Created by Rishan Thangaraj on 11/20/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

namespace CoreLib
{
   template <class E>
   class Error
   {
   public:
        Error(E&& c_error)
        {
           errorT = c_error;
        }

        constexpr E& error()
        {
            return errorT;
        }

   private:
       E errorT;
   };
}