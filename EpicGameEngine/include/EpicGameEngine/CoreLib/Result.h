//
// Created by Rishan Thangaraj on 9/24/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <optional>
#include <type_traits>

#include "Error.h"

// TODO: Dysfunctional

namespace CoreLib
{
    template <typename T, typename E>
    class Result
    {
    public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = Error<E>;
        template <class U>
        using rebind = Result<U, error_type>;

        Result()
        {
            valueT = value_type();
            hasValue = true;
        }

        template<typename... Args>
        Result(Args... args)
        {
            valueT = value_type(std::forward<Args>(args)...);
            hasValue = true;
        }

        template <class G>
        constexpr explicit(!std::is_convertible_v<const G&, E>) Result(Error<G>& e)
        {
            errorT = e.error();
        }

        template <class G>
        constexpr explicit(!std::is_convertible_v<const G&, E>) Result(Error<G>&& e)
        {
            errorT = e.error();
        }

        ~Result()
        {

        }

        constexpr Result& operator=(const Result& other)
        {

        }

        bool has_value()
        {
            return hasValue;
        }

        constexpr value_type& value() &
        {
            return valueT;
        }

        constexpr E& error()
        {
            return errorT;
        }

        template <class U>
        constexpr T value_or(U&& default_value)
        {
            if (hasValue)
            {
                return valueT;
            }
            else
            {
                return default_value;
            }
        }

    private:
        bool hasValue = false;
        value_type valueT;
        error_type errorT;

    };
}