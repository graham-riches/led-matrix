/*! \file expected.h
*
*  \brief expected algebraic data type for error handling. This essentially an either type
*         or an expanded optional that returns either the type, or a custom error type.
*
*
*  \author Graham Riches
*/
#ifndef __EXPECTED_H
#    define __EXPECTED_H__

/********************************** Includes *******************************************/
#    include <optional>
#    include <utility>

/********************************** Types *******************************************/
/**
 * \brief class template for an expected type. This either contains a T with the _valid result
 *        of the computation, or an E, which is any custom error type
 * 
 * \tparam T type of the expected in the success case
 * \tparam E type of the expected in the error case
 */
template <typename T, typename E>
class expected {
  private:
    union {
        T _value;
        E _error;
    };
    bool _valid;

  public:
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
        result._valid = true;
        new (&result._value) T(std::forward<Args>(params)...);
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
        result._valid = false;
        new (&result._error) E(std::forward<Args>(params)...);
    }

    /**
     * \brief Construct a new expected object with the default constructor
     */
    expected() {};

    /**
     * \brief Construct a new expected object from a copy
     * 
     * \param other the other to copy from
     */
    expected(const expected& other)
        : _valid(other._valid) {
        if ( _valid ) {
            new (&_value) T(other._value);
        } else {
            new (&_error) E(other._error);
        }
    }

    /**
     * \brief Construct a new expected object from a move
     * 
     * \param other the other to mvoe
     */
    expected(expected&& other)
        : _valid(other._valid) {
        if ( _valid ) {
            new (&_value) T(std::move(other._value));
        } else {
            new (&_error) E(std::move(other._error));
        }
    }

    /**
     * \brief copy assignment operator uses the exception safe swap idiom
     * 
     * \param other other expected to assign to this
     * \retval expected& 
     */
    expected& operator=(expected other) {
        swap(other);
        return *this;
    }

    /**
     * \brief casting operator to bool so that expected types can directly be used in control flow
     * 
     * \retval returns true if the expected contains type T and not E
     */
    operator bool() const {
        return _valid;
    }

    /**
     * \brief casting operator to std optional 
     * 
     * \retval std::optional<T> 
     */
    operator std::optional<T>() const {
        if ( _valid ) {
            return _value;
        } else {
            return {};
        }
    }

    /**
     * \brief Destroy the expected object based on the contained type in the union    
     */
    ~expected() {
        if ( _valid ) {
            _value.~T();
        } else {
            _error.~E();
        }
    }

    /**
     * \brief helper function that implements the swap idiom for exception safety during the assignment
     *        operator
     * 
     * \param other the other expected to assign to this
     */
    void swap(expected& other) {
        if ( _valid ) {
            if ( other._valid ) {
                std::swap(_value, other._value);
            } else {
                auto temp_error = std::move(other._error);
                other._error.~E();
                new (&other._value) T(std::move(_value));
                _value.~T();
                new (&_error) E(std::move(temp_error));
                std::swap(_valid, other._valid);
            }
        } else {
            if ( other._valid ) {
                other.swap(*this);
            } else {
                std::swap(_error, other._error);
            }
        }
    }
};

#endif
