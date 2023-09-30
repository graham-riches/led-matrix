/**
 * \file mtry.hpp 
 * \brief try-catch exception handling monad
 * \version 0.1
 * \date 2021-01-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/********************************** Includes *******************************************/
#include "expected.hpp"

namespace reactive
{
/**
 * \brief try catch monad for error handling
 * 
 * \param f function to try to parse
 * \retval Expected contains either the result, or the exception that was thrown
 */
template <typename F, typename R = typename std::result_of_t<F()>, typename Expected = expected<R, std::exception_ptr>>
Expected mtry(F f) {
    try {
        return Expected::success(f());
    } catch ( ... ) {
        return Expected::error(std::current_exception());
    }
}

};  // namespace reactive
