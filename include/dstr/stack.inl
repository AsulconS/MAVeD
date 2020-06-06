/********************************************************************************
 *                                                                              *
 * MAVeD (MilAnverDatabase)                                                     *
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

template <typename T>
inline Stack<T>::Stack()
    : m_size     {0},
      m_capacity {2}
{
    m_data = new T[2];
}

template <typename T>
inline Stack<T>::Stack(const std::size_t t_capacity)
    : m_size     {0},
      m_capacity {t_capacity}
{
    m_data = new T[m_capacity];
}

template <typename T>
inline Stack<T>::~Stack()
{
    delete[] m_data;
}

template <typename T>
inline void Stack<T>::push(const T& val)
{
    if(m_size >= m_capacity)
    {
        realloc();
    }
    m_data[m_size++] = val;
}

template <typename T>
inline T Stack<T>::top()
{
    if(!empty())
    {
        return m_data[m_size - 1];
    }
    return T {};
}

template <typename T>
inline T Stack<T>::pop()
{
    if(!empty())
    {
        return m_data[--m_size];
    }
    return T {};
}

template <typename T>
inline std::string Stack<T>::getStr()
{
    std::stringstream sStr;
    sStr << m_size << " [ ";
    for(int i = 0; i < m_size; ++i)
    {
        sStr << (int)m_data[i] << ' ';
    }
    sStr << " ]";

    return sStr.str();
}

template <typename T>
inline std::string Stack<T>::getStrN()
{
    std::stringstream sStr;
    sStr << m_size << " [ ";
    for(int i = 0; i < m_size; ++i)
    {
        sStr << m_data[i]->data << ' ';
    }
    sStr << " ]";

    return sStr.str();
}

template <typename T>
inline void Stack<T>::realloc()
{
    m_capacity <<= 1;
    T* data = new T[m_capacity];
    for(size_t i = 0; i < m_size; ++i)
        data[i] = m_data[i];
    delete[] m_data;
    m_data = data;
}
