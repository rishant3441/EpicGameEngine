//
// Created by Rishan Thangaraj on 9/24/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <optional>

// TODO: Dysfunctional

namespace CoreLib
{
    namespace result
    {
        template <typename successT, typename failureT>
        class Result
        {
        public:
            Result(successT& sucess)
            : success(sucess)
            {}
            Result(failureT& failure)
                : failure(failure)
                {}

            using SuccessType = successT;
            using FailureType = failureT;

        private:
            std::optional<SuccessType> success;
            std::optional<FailureType> failure;
        };

        template<typename failureT>
        class Result<void, failureT>
        {
        public:
            Result() = default;
            Result(failureT& failure)
                : failure(failure)
                {}

            using FailureType = failureT;

            void as_value()
            {
               return;
            }

            failureT as_error()
            {
               return failure;
            }
        private:
            std::optional<failureT> failure;
        };

        template <typename successT, typename failureT, typename... Args>
        Result<successT, failureT> Ok(Args... args)
        {
            return Result<successT, failureT>(successT(std::forward<Args>(args)...));
        }

        template<>
        Result<void, int> Ok()
        {
            return {};
        }
    };

}