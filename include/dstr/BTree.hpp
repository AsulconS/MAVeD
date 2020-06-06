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

#ifndef MAVeD_BTREE_HPP
#define MAVeD_BTREE_HPP

#include <fstream>

#include <queue>
#include <unordered_map>

#include <core/node.hpp>
#include <dstr/stack.hpp>

namespace mav
{
template <typename T, int N>
class BTree
{
    using Node = BTreeNode<T, N>;

public:
    BTree();
    BTree(Node* t_root);
    BTree(const std::vector<AttachedPair<T>>& data);
    BTree(const std::initializer_list<AttachedPair<T>>& l);
    virtual ~BTree();

    void makeEmpty();
    void makeEmpty(Node* root);

    virtual Node* find(const AttachedPair<T>& value, int& index);
    virtual Node* find(const AttachedPair<T>& value, Node*& root, int& index);

    virtual void insertKey(const AttachedPair<T>& value);
    virtual void insertKey(const AttachedPair<T>& value, Node*& root);

    virtual void updateKey(const Node* node);

    virtual void deleteKey(const AttachedPair<T>& value);
    virtual void deleteKey(const AttachedPair<T>& value, Node*& root);

    void loadAll();
    void loadAll(Node*& root);

    void exportToFile(const std::string& title, bool isFinal = false);

    static void setDebugMode(const bool mode) { m_debugMode = mode; }

    static uint32 getFilesCount() { return m_filesCount; };

protected:
    virtual void labelNode(Node* node);

    void splitChild(Node* node, const std::size_t index);
    void insertKeyNonFull(const AttachedPair<T>& value, Node*& root);

    void clearFile();
    void createFileIfNotExists();

    void diskWrite(const Node* node);
    void diskRead(Node*& node, const std::size_t index);

    std::size_t allocate();
    std::size_t getRootIndex();

    void setRootIndex(const std::size_t index);
    void setTransactionState(const char state);

    Node* m_root;
    Stack<Node*> m_nodes;
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

#include <dstr/BTree.inl>

} // namespace mav

#endif // MAVeD_BTREE_HPP
