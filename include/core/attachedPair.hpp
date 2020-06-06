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

#ifndef MAVeD_ATTACHED_PAIR_HPP
#define MAVeD_ATTACHED_PAIR_HPP

#include <core/common.hpp>

namespace mav
{
template <typename T>
struct AttachedPair
{
public:
    int pk;
    T body;
};

template <typename T>
inline bool operator==(const AttachedPair<T>& tl, const AttachedPair<T>& tr)
{
    return (tl.pk == tr.pk) && (tl.body == tr.body);
}

template <typename T>
inline bool operator!=(const AttachedPair<T>& tl, const AttachedPair<T>& tr)
{
    return (tl.pk != tr.pk) || (tl.body != tr.body);
}

template <typename T>
inline bool operator<(const AttachedPair<T>& tl, const AttachedPair<T>& tr)
{
    return tl.pk < tr.pk;
}

template <typename T>
inline bool operator>(const AttachedPair<T>& tl, const AttachedPair<T>& tr)
{
    return tl.pk > tr.pk;
}

} // namespace mav

namespace std
{
template <typename T>
std::string to_string(const mav::AttachedPair<T>& ap)
{
    return std::to_string(ap.pk) + ": " + std::to_string(ap.body);
}

} // namespace std

#endif // MAVeD_ATTACHED_PAIR_HPP
