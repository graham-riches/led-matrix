/*! \file expected.h
*
*  \brief expected algebraic data type for error handling. This essentially an either type
*         or an expanded optional that returns either the type, or a custom error type.
*
*
*  \author Graham Riches
*/
#ifndef __EXPECTED_H
#define __EXPECTED_H__

#include <utility>

template <typename T, typename E>
class expected {
    public:

    /**
     * \brief Construct a new expected object from a copy
     * 
     * \param other the other to copy from
     */
    expected(const expected& other)
    : valid(other.valid) {
        if (valid) {
            new (&value) T(other.value);
        } else {
            new (&error) E(other.error);
        }
    }

    /**
     * \brief Construct a new expected object from a move
     * 
     * \param other the other to mvoe
     */
    expected(expected&& other) 
    : valid(other.valid) {
        if (valid) {
            new (&value) T(std::move(other.value));
        } else {
            new (&error) E(std::move(other.error));
        }
    }
    

    /**
     * \brief factory method to create an expected from the success type
     * 
     * \tparam Args parameter pack of arguments
     * \param params arguments parameter pack
     * \retval expected 
     */
    template <typename... Args>
    static expected success(Args&&... params) {
        expected result;
        result.valid = true;
        new (&result.value) T(std::forward<Args>(params)...);
        return result;
    }

    /**
     * \brief factory method to create an expected from the error type
     * 
     * \tparam Args parameter pack of arguments
     * \param params the pack of arguments
     * \retval expected 
     */
    template <typename... Args>
    static expected error(Args&&... params) {
        expected result;
        result.valid = false;
        new (&result.error) E(std::forward<Args>(params)...)
    }

    private:
    union {
        T value;
        E error;
    };
    bool valid;

    /**
     * \brief Destroy the expected object based on the contained type in the union    
     */
    ~expected() {
        if (valid) {
            value.~T();
        } else {
            error.~E();
        }
    }
};




#endif
