#pragma once

// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

// This is a helper macro for both declaring the signature of a callback (nee event) and
// defining the body. Winrt callbacks need a method for adding a delegate to the
// callback and removing the delegate. This macro will both declare the method
// signatures and define them both for you, because they don't really vary from
// event to event.
// Use this in a class's header if you have a "delegate" type in your IDL.
#define WINRT_CALLBACK(name, args)               \
public:                                          \
    winrt::event_token name(const args& handler) \
    {                                            \
        return _##name##Handlers.add(handler);   \
    }                                            \
    void name(const winrt::event_token& token)   \
    {                                            \
        _##name##Handlers.remove(token);         \
    }                                            \
                                                 \
protected:                                       \
    winrt::event<args> _##name##Handlers;        \
                                                 \
public:

// Use this macro to quick implement both the getter and setter for a property.
// This should only be used for simple types where there's no logic in the
// getter/setter beyond just accessing/updating the value.
#define WINRT_PROPERTY(type, name, ...)   \
public:                                   \
    type name() const noexcept            \
    {                                     \
        return _##name;                   \
    }                                     \
    void name(const type& value) noexcept \
    {                                     \
        _##name = value;                  \
    }                                     \
                                          \
private:                                  \
    type _##name{ __VA_ARGS__ };          \
                                          \
public:

// Use this macro to quick implement the getter for a property.
// This should only be used for simple types where there's no logic in the
// getter beyond just accessing the value.
#define WINRT_READONLY_PROPERTY(type, name, ...)   \
public:                                   \
    type name() const noexcept            \
    {                                     \
        return _##name;                   \
    }                                     \
                                          \
private:                                  \
    type _##name{ __VA_ARGS__ };          \
                                          \
public:

// Use this macro to quickly implement both the getter and setter for an
// observable property. This is similar to the WINRT_PROPERTY macro above,
// except this will also raise a PropertyChanged event with the name of the
// property that has changed inside of the setter. This also implements a
// private _setName() method, that the class can internally use to change the
// value when it _knows_ it doesn't need to raise the PropertyChanged event
// (like when the class is being initialized).
#define WINRT_OBSERVABLE_PROPERTY(type, name, event, ...)                                          \
public:                                                                                            \
    type name() const noexcept                                                                     \
    {                                                                                              \
        return _##name;                                                                            \
    };                                                                                             \
    void name(const type& value)                                                                   \
    {                                                                                              \
        if (_##name != value)                                                                      \
        {                                                                                          \
            _##name = value;                                                                       \
            event(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L## #name }); \
        }                                                                                          \
    };                                                                                             \
                                                                                                   \
private:                                                                                           \
    type _##name{ __VA_ARGS__ };                                                                   \
    void _set##name(const type& value)                                                             \
    {                                                                                              \
        _##name = value;                                                                           \
    };                                                                                             \
                                                                                                   \
public:
