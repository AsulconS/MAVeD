/********************************************************************************
 *                                                                              *
 * MAVeD - MilanverDatabase                                                     *
 * Copyright (c) 2020 Adrian Bedregal (AsulconS)                                *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#ifndef MAVeD_STACK_HPP
#define MAVeD_STACK_HPP

#include <sstream>

#include <core/common.hpp>

namespace mav
{
template <typename T>
class Stack
{
public:
    Stack();
    Stack(const std::size_t t_capacity);
    virtual ~Stack();

    void clear() { m_size = 0; }

    inline bool empty() { return !m_size; }
    inline std::size_t getSize() { return m_size; }

    void push(const T& val);

    T top();
    T pop();

    std::string getStr();
    std::string getStrN();

private:
    T* m_data;

    std::size_t m_size;
    std::size_t m_capacity;

    void realloc();
};

#include <dstr/stack.inl>

} // namspace mav

#endif // MAVeD_STACK_HPP
