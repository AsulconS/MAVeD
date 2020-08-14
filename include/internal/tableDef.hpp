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

#ifndef MAVeD_TABLE_DEF_HPP
#define MAVeD_TABLE_DEF_HPP

#include <internal/tuple3.hpp>

#include <vector>
#include <string>

namespace mav
{
class TableDef
{
public:
    TableDef() : recordSize {}, columnsDef{} {}
    ~TableDef() {}

    std::size_t recordSize;
    std::vector<Tuple3<std::string, std::size_t, std::size_t>> columnsDef;
};

} // namespace mav

#endif // MAVeD_TABLE_DEF_HPP
