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

#ifndef MAVeD_NODE_HPP
#define MAVeD_NODE_HPP

#include <core/common.hpp>
#include <core/utils.hpp>
#include <core/attachedPair.hpp>

#define sizeof_BTNRelevantData(T, N) offsetof(unparentheses_type((BTreeNode<T, N>)), children)

namespace mav
{
template <typename T, int N>
struct BTreeNode
{
public:
    // Relevant Data
    // -------------
    AttachedPair<T> data[N - 1];
    std::size_t size;

    std::size_t index;
    std::size_t childrenIndices[N];

    bool isLeaf;
    // -------------

    BTreeNode<T, N>* children[N];
    std::string label;
};

template <typename T, int N>
inline void operator>>(BTreeNode<T, N>& node, const std::size_t index)
{
    for(std::size_t i = node.size + 1; i > index + 1; --i)
    {
        node.children[i] = node.children[i - 1];
        node.childrenIndices[i] = node.childrenIndices[i - 1];
    }
    for(std::size_t i = node.size; i > index; --i)
    {
        node.data[i] = node.data[i - 1];
    }
    ++node.size;
}

} // namespace mav

#endif // MAVeD_NODE_HPP
