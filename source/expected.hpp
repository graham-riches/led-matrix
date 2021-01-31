/*! \file expected.h
*
*  \brief expected algebraic data type for error handling. This essentially an either type
*         or an expanded optional that returns either the type, or a custom error type.
*
*
*  \author Graham Riches
*/
#pragma once

/********************************** Includes *******************************************/
#include <optional>
#include <utility>
#include <exception>

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
        return result;
    }    

    /**
     * \brief get the expected value out of the variant
     * \note user should check if the value is valid before trying to access it
     * 
     * \retval T& returns a T if it exists, otherwise throws an exception
     */
    T& get_value() {
        if (_valid) {
            return _value;
        } else {
            throw std::logic_error("Expected does not contain a valid value type");
        }
    }

    /**
     * \brief Get the error value out of the union.
     * \note user should check that it is indeed an error before trying to retrieve it
     * 
     * \retval E& the error value if it exists, otherwise an exception
     */
    E& get_error() {
        if (_valid) {
            throw std::logic_error("Expected does not contain an error type");
        } else {
            return _error;
        }
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


/**
 * \brief monad bind for the expected type that allows chaining of multiple expected operations together
 * 
 * \tparam F next function in the chain to bind
 * \param exp an expected value to wrap into the next sequence
 * \param f function to wrap exp with
 * \retval R return type of F(T)
 * \todo: might need an overload that handles const qualifiers?
 */
template <typename T, typename E, typename F, typename R = typename std::result_of_t<F(T)>>
static R mbind(expected<T, E>& exp, F f) {
    if (!exp) {
        return R::error(exp.get_error());
    } else {
        return f(exp.get_value());
    }
}

/**
 * \brief overload of the monad bind for the expected type that allows chaining of multiple expected operations together
 *        for rvalue references
 * 
 * \tparam F next function in the chain to bind
 * \param exp an expected value to wrap into the next sequence
 * \param f function to wrap exp with
 * \retval R return type of F(T)
 * \todo: might need an overload that handles const qualifiers?
 */
template <typename T, typename E, typename F, typename R = typename std::result_of_t<F(T)>>
static R mbind(expected<T, E>&& exp, F f) {
    if (!exp) {
        return R::error(exp.get_error());
    } else {
        return f(exp.get_value());
    }
}
