/*
 * Copyright (c) 2017 Evgeniy Alekseev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 *
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */


#ifndef _RESULT_HPP_
#define _RESULT_HPP_

#include <string>
#include <variant>

/**
 * @addtogroup Result
 * @brief Result namespace
 */
namespace Result
{
/**
 * @enum Content
 * @brief Result content type enumeration
 * @var Content::Empty
 * invalid content
 * @var Content::Value
 * Result contains value
 * @var Content::Error
 * Result contains error
 */
enum class Content { Empty, Value, Error };

/**
 * @brief Error representation
 * @tparam ErrorEnum
 * error code enumeration
 */
template <typename ErrorEnum> class Error
{
public:
    /**
     * @brief default class constructor
     */
    Error() = default;
    /**
     * @brief Error class constructor
     * @param message
     * human readable error message
     * @param code
     * machine readable error code
     */
    Error(const std::string message, const ErrorEnum code)
        : m_code(code)
    {
        m_message = std::move(message);
    };
    /**
     * @brief Error class constructor with default error code
     * @param message
     * human readable error message
     */
    Error(const ErrorEnum code)
        : Error("", code){};
    /**
     * @brief Error class constructor with empty error message
     * @param code
     * machine readable error code
     */
    Error(const std::string message)
        : Error(message, ErrorEnum()){};
    /**
     * @brief Error class destructor
     */
    ~Error() = default;

    /**
     * @brief error message
     * @return human readable error message
     */
    std::string message() const { return m_message; };
    /**
     * @brief error code
     * @return machine readable error code
     */
    ErrorEnum code() const { return m_code; };

private:
    /**
     * @brief human readable error message
     */
    std::string m_message;
    /**
     * @brief machine readable error code
     */
    ErrorEnum m_code;
};

/**
 * @brief Result main class
 * @tparam T
 * value class name
 * @tparam ErrorEnum
 * error code enumeration
 */
template <typename T, typename ErrorEnum>
class Result : public std::variant<T, Error<ErrorEnum>>
{
public:
    /**
     * @brief default class constructor
     */
    Result() = default;
    /**
     * @brief Result constructor with value
     * @param value
     * result value
     */
    Result(const T value)
        : std::variant<T, Error<ErrorEnum>>(value){};
    /**
     * @brief Result constructor with error
     * @param error
     * result error
     */
    Result(const Error<ErrorEnum> error)
        : std::variant<T, Error<ErrorEnum>>(error){};

    /**
     * @brief get result value
     * @throw std::bad_variant_access
     * @return result value if holded
     */
    T get() const { return std::get<T>(*this); };
    /**
     * @brief get result error
     * @throw std::bad_variant_access
     * @return result error if holded
     */
    Error<ErrorEnum> error() const
    {
        return std::get<Error<ErrorEnum>>(*this);
    };
    /**
     * @brief get result content
     * @return result object content type
     */
    Content type() const
    {
        switch (this->index()) {
        case 0:
            return Content::Value;
        case 1:
            return Content::Error;
        case std::variant_npos:
        default:
            return Content::Empty;
        };
    };
    /**
     * @brief match result function
     * @tparam UnaryFunctionValue
     * function type which will be called if result contains value
     * @tparam UnaryFunctionError
     * function type which will be called if result contains error
     * @param apply_value
     * function which will be called if result contains value
     * @param apply_error
     * function which will be called if result contains error
     */
    template <typename UnaryFunctionValue, typename UnaryFunctionError>
    void match(UnaryFunctionValue apply_value,
               UnaryFunctionError apply_error) const
    {
        switch (type()) {
        case Content::Value:
            apply_value(get());
            break;
        case Content::Error:
            apply_error(error());
            break;
        case Content::Empty:
            break;
        }
    };
};

// additional functions
/**
 * @brief match result function
 * @tparam T
 * value class name
 * @tparam ErrorEnum
 * error code enumeration
 * @tparam UnaryFunctionValue
 * function type which will be called if result contains value
 * @tparam UnaryFunctionError
 * function type which will be called if result contains error
 * @param result
 * result object
 * @param apply_value
 * function which will be called if result contains value
 * @param apply_error
 * function which will be called if result contains error
 */
template <typename T, typename ErrorEnum, typename UnaryFunctionValue,
          typename UnaryFunctionError>
void match(const Result<T, ErrorEnum> &result, UnaryFunctionValue apply_value,
           UnaryFunctionError apply_error)
{
    return result.match(apply_value, apply_error);
};
};

#endif /* _RESULT_HPP_ */
