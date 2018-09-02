/**
 * See http://www.boost.org/libs/any for Documentation.
 *
 * Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * what:  variant type boost::any
 * who:   contributed by Kevlin Henney,
 *        with features contributed and bugs found by
 *        Antony Polukhin, Ed Brey, Mark Rodgers, Peter Dimov and James Curran
 * when:  July 2001, April 2013 - May 2013
 */

#ifndef BOOST_ANY_INCLUDED
#define BOOST_ANY_INCLUDED

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>


namespace libanyboost
{

class any final
{
public:
    any() noexcept :
      content(0)
    { }


    template<typename ValueType>
    any(const ValueType& value) :
        content(new holder<typename std::remove_cv<typename std::decay<const ValueType>::type>::type>(value))
    { }


    any(const any& other) :
      content(other.content ? other.content->clone() : 0)
    { }


    any(any&& other) noexcept :
      content(other.content)
    {
        other.content = 0;
    }


    template<typename ValueType>
    any(ValueType&& value, typename std::enable_if<!std::is_same<any&, ValueType>::value>::type* = 0, typename std::enable_if<!std::is_const<ValueType>::value>::type* = 0) :
        content(new holder<typename std::decay<ValueType>::type>(static_cast<ValueType&&>(value)))
    { }


    ~any() noexcept
    {
        delete content;
    }


    any& swap(any& rhs) noexcept
    {
        std::swap(content, rhs.content);
        return *this;
    }


    any& operator=(const any& rhs)
    {
        any(rhs).swap(*this);
        return *this;
    }


    any& operator=(any&& rhs) noexcept
    {
        rhs.swap(*this);
        any().swap(rhs);
        return *this;
    }


    template <class ValueType>
    any& operator=(ValueType&& rhs)
    {
        any(static_cast<ValueType&&>(rhs)).swap(*this);
        return *this;
    }


    bool has_value() const noexcept
    {
        return content;
    }


    void reset() noexcept
    {
        any().swap(*this);
    }


    const std::type_info& type() const noexcept
    {
        return content ? content->type() : typeid(void);
    }


private:
    class placeholder
    {
    public:
        virtual ~placeholder()
        { }

    public:
        virtual const std::type_info& type() const noexcept = 0;

        virtual placeholder* clone() const = 0;

    };


    template<typename ValueType>
    class holder : public placeholder
    {
    public:
        holder(const ValueType& value) :
          held(value)
        { }


        holder(ValueType&& value) :
          held(static_cast<ValueType&&>(value))
        { }


        virtual const std::type_info& type() const noexcept
        {
            return typeid(ValueType);
        }


        virtual placeholder* clone() const
        {
            return new holder(held);
        }


        ValueType held;

    private:
        holder& operator=(const holder &);
    };


private:
    template<typename ValueType>
    friend ValueType* any_cast(any*) noexcept;

    placeholder* content;
};


inline void swap(any& lhs, any& rhs) noexcept
{
    lhs.swap(rhs);
}


class bad_any_cast : public std::bad_cast
{
public:
    virtual const char* what() const noexcept override
    {
        return "bad any_cast";
    }
};


template<typename ValueType>
ValueType* any_cast(any* operand) noexcept
{
    return operand && operand->type() == typeid(ValueType) ? std::addressof(static_cast<any::holder<typename std::remove_cv<ValueType>::type>*>(operand->content)->held) : 0;
}


template<typename ValueType>
inline const ValueType* any_cast(const any* operand) noexcept
{
    return any_cast<ValueType>(const_cast<any*>(operand));
}


template<typename ValueType>
ValueType any_cast(any& operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;

    nonref* result = any_cast<nonref>(std::addressof(operand));
    if(!result)
        throw bad_any_cast();

    typedef typename std::conditional<std::is_reference<ValueType>::value, ValueType, typename std::add_lvalue_reference<ValueType>::type>::type ref_type;

    return static_cast<ref_type>(*result);
}


template<typename ValueType>
inline ValueType any_cast(const any& operand)
{
    typedef typename std::remove_reference<ValueType>::type nonref;
    return any_cast<const nonref&>(const_cast<any&>(operand));
}


template<typename ValueType>
inline ValueType any_cast(any&& operand)
{
    static_assert(std::is_rvalue_reference<ValueType&&>::value || std::is_const< typename std::remove_reference<ValueType>::type >::value,
        "any_cast shall not be used for getting nonconst references to temporary objects");

    return any_cast<ValueType>(operand);
}

}


#endif /* BOOST_ANY_INCLUDED */
