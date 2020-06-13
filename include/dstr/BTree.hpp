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

#ifndef MAVeD_BTREE_HPP
#define MAVeD_BTREE_HPP

#include <core/common.hpp>
#include <core/attachedPair.hpp>
#include <core/node.hpp>
#include <core/strCast.hpp>

#include <dstr/stack.hpp>

#include <queue>
#include <unordered_map>

#include <string>
#include <fstream>
#include <iostream>

namespace mav
{
template <typename T, int N>
class BTree
{
    using BNode = Node<T, N>;

public:
    BTree();
    BTree(BNode* t_root);
    BTree(const std::vector<AttachedPair<T>>& data);
    BTree(const std::initializer_list<AttachedPair<T>>& l);
    virtual ~BTree();

    void makeEmpty();
    void makeEmpty(BNode* root);

    virtual BNode* find(const AttachedPair<T>& value, int& index);
    virtual BNode* find(const AttachedPair<T>& value, BNode*& root, int& index);

    virtual void insertKey(const AttachedPair<T>& value);
    virtual void insertKey(const AttachedPair<T>& value, BNode*& root);

    virtual void updateKey(const BNode* node);

    virtual void deleteKey(const AttachedPair<T>& value);
    virtual void deleteKey(const AttachedPair<T>& value, BNode*& root);

    void loadAll();
    void loadAll(BNode*& root);

    void exportToFile(const std::string& title, bool isFinal = false);

    static void setDebugMode(const bool mode) { m_debugMode = mode; }

    static uint32 getFilesCount() { return m_filesCount; };

protected:
    virtual void labelNode(BNode* node);

    void splitChild(BNode* node, const std::size_t index);
    void insertKeyNonFull(const AttachedPair<T>& value, BNode*& root);

    void clearFile();
    void createFileIfNotExists();

    void diskWrite(const BNode* node);
    void diskRead(BNode*& node, const std::size_t index);

    std::size_t allocate();
    std::size_t getRootIndex();

    void setRootIndex(const std::size_t index);
    void setTransactionState(const char state);

    BNode* m_root;
    Stack<BNode*> m_nodes;
    std::fstream m_file;
    std::fstream m_bkFile;
    std::fstream m_flagsFile;

    std::fstream* m_writeTarget;

    bool m_lockPhase;
    std::unordered_map<int, std::queue<std::pair<char, int>>> m_lockTable;

    std::size_t m_nextIndex;

    bool m_debugMode;
    uint32 m_filesCount;

private:
    constexpr const char* thisName() { return "BTree"; };
};

} // namespace mav

#include <dstr/BTree.inl>

#endif // MAVeD_BTREE_HPP
