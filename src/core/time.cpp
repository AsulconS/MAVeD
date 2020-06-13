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

#include <core/time.hpp>

namespace mav
{
Timestamp::Timestamp()
    : m_timestampStr {}
{
}

Timestamp::~Timestamp()
{
}

void Timestamp::stamp()
{
    std::chrono::time_point<std::chrono::system_clock> now {std::chrono::system_clock::now()};
    std::time_t in_time_t {std::chrono::system_clock::to_time_t(now)};

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    m_timestampStr = ss.str();
}

const std::string& Timestamp::str()
{
    return m_timestampStr;
}

} // namespace mav
