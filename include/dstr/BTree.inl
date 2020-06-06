/********************************************************************************
 *                                                                              *
 * MADB (MilAnverDB)                                                            *
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

template <typename T, int N>
inline BTree<T, N>::BTree()
    : m_root       {nullptr},
      m_lockPhase  {true},
      m_nextIndex  {sizeof(std::size_t)},
      m_debugMode  {false},
      m_filesCount {0}
{
    createFileIfNotExists();
    m_file.open("data/data.bt", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    m_bkFile.open("data/dataBK.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_flagsFile.open("data/flags.meta", std::ios::in | std::ios::out | std::ios::binary);

    if(m_file.tellg() > 0)
    {
        std::cout << "Loading Existing Database" << std::endl;
        diskRead(m_root, getRootIndex());
        labelNode(m_root);
    }
}

template <typename T, int N>
inline BTree<T, N>::BTree(Node* t_root)
    : m_root       {t_root},
      m_lockPhase  {true},
      m_nextIndex  {sizeof(std::size_t)},
      m_debugMode  {false},
      m_filesCount {0}
{
    createFileIfNotExists();
    clearFile();
    m_file.open("data/data.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_bkFile.open("data/dataBK.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_flagsFile.open("data/flags.meta", std::ios::in | std::ios::out | std::ios::binary);

    insertKey(t_root);
}

template <typename T, int N>
inline BTree<T, N>::BTree(const std::vector<AttachedPair<T>>& data)
    : m_root       {nullptr},
      m_lockPhase  {true},
      m_nextIndex  {sizeof(std::size_t)},
      m_debugMode  {false},
      m_filesCount {0}
{
    createFileIfNotExists();
    clearFile();
    m_file.open("data/data.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_bkFile.open("data/dataBK.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_flagsFile.open("data/flags.meta", std::ios::in | std::ios::out | std::ios::binary);

    for(const auto& v : data)
    {
        insertKey(v);
    }
}

template <typename T, int N>
inline BTree<T, N>::BTree(const std::initializer_list<AttachedPair<T>>& l)
    : m_root       {nullptr},
      m_lockPhase  {true},
      m_nextIndex  {sizeof(std::size_t)},
      m_debugMode  {false},
      m_filesCount {0}
{
    createFileIfNotExists();
    clearFile();
    m_file.open("data/data.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_bkFile.open("data/dataBK.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_flagsFile.open("data/flags.meta", std::ios::in | std::ios::out | std::ios::binary);

    for(const auto& v : l)
    {
        insertKey(v);
    }
}

template <typename T, int N>
inline BTree<T, N>::~BTree()
{
    makeEmpty();
    std::cout << "Memory-Loaded BTree destroyed successfully" << std::endl;
    m_file.close();
}

template <typename T, int N>
inline void BTree<T, N>::makeEmpty()
{
    makeEmpty(m_root);
}

template <typename T, int N>
inline void BTree<T, N>::makeEmpty(Node* root)
{
    if(root == nullptr)
    {
        return;
    }

    if(root->isLeaf)
    {
        delete root;
        return;
    }

    for(std::size_t i = 0; i < root->size + 1; ++i)
    {
        if(root->children[i] != nullptr)
        {
            makeEmpty(root->children[i]);
        }
    }
}

template <typename T, int N>
inline BTreeNode<T, N>* BTree<T, N>::find(const AttachedPair<T>& value, int& index)
{
    return find(value, m_root, index);
}

template <typename T, int N>
inline BTreeNode<T, N>* BTree<T, N>::find(const AttachedPair<T>& value, Node*& root, int& index)
{
    if(root == nullptr)
    {
        return nullptr;
    }

    std::size_t i = 0;
    while(i < root->size && value > root->data[i]) ++i;

    if(i < root->size && value == root->data[i])
    {
        index = i;
        std::cout << '\"' << std::to_string(value) << '\"' << " found at DRA: 0x" << std::hex << std::uppercase << root->index << std::endl;
        return root;
    }
    else if(root->isLeaf)
    {
        index = -1;
        std::cout << '\"' << std::to_string(value) << '\"' << " not found. Index set as -1. Aborting" << std::endl;
        return nullptr;
    }
    else
    {
        diskRead(root->children[i], root->childrenIndices[i]);
        return find(value, root->children[i], index);
    }
}

template <typename T, int N>
inline void BTree<T, N>::insertKey(const AttachedPair<T>& value)
{
    setTransactionState(1);
        m_writeTarget = &m_file;
        insertKey(value, m_root);
    setTransactionState(2);
        m_writeTarget = &m_bkFile;
        insertKey(value, m_root);
    setTransactionState(0);
        m_writeTarget = nullptr;
}

template <typename T, int N>
inline void BTree<T, N>::insertKey(const AttachedPair<T>& value, Node*& root)
{
    if(root == nullptr)
    {
        root = new Node { {value}, 1, allocate(), {}, true, {}, "" };
        diskWrite(root);
        setRootIndex(root->index);
        labelNode(root);
        return;
    }

    if(root->size >= N - 1)
    {
        Node* newRoot = new Node { {}, 0, allocate(), {root->index}, false, {root}, "" };
        root = newRoot;
        setRootIndex(root->index);
        splitChild(root, 0);
        insertKeyNonFull(value, root);
    }
    else
    {
        insertKeyNonFull(value, root);
    }
}

template <typename T, int N>
inline void BTree<T, N>::updateKey(const Node* node)
{
    if(node != nullptr)
    {
        setTransactionState(1);
            m_writeTarget = &m_file;
            diskWrite(node);
        setTransactionState(2);
            m_writeTarget = &m_bkFile;
            diskWrite(node);
        setTransactionState(0);
            m_writeTarget = nullptr;
    }
}

template <typename T, int N>
inline void BTree<T, N>::deleteKey(const AttachedPair<T>& value)
{
    setTransactionState(1);
        m_writeTarget = &m_file;
        deleteKey(value, m_root);
    setTransactionState(2);
        m_writeTarget = &m_bkFile;
        deleteKey(value, m_root);
    setTransactionState(0);
        m_writeTarget = nullptr;
}

template <typename T, int N>
inline void BTree<T, N>::deleteKey(const AttachedPair<T>& value, Node*& root)
{
    //
}

template <typename T, int N>
inline void BTree<T, N>::loadAll()
{
    loadAll(m_root);
}

template <typename T, int N>
inline void BTree<T, N>::loadAll(Node*& root)
{
    if(root == nullptr)
    {
        return;
    }

    if(!root->isLeaf)
    {
        for(std::size_t i = 0; i < root->size + 1; ++i)
        {
            diskRead(root->children[i], root->childrenIndices[i]);
            labelNode(root->children[i]);
            loadAll(root->children[i]);
        }
    }
}

template <typename T, int N>
inline void BTree<T, N>::exportToFile(const std::string& title, bool isFinal)
{
    if(m_root == nullptr)
    {
        return;
    }

    std::string fn {thisName() + std::to_string(m_filesCount++) + ".dot"};
    std::fstream of;
    of.open(fn, std::ios::out | std::ios::binary);

    std::string header1
    {
        "graph IBST\r\n"
        "{\r\n"
        "    // Title Properties\r\n"
        "    // --------------------------------------------------------------------------------\r\n"
        "    labelloc = \"t\";\r\n"
        "    label = "
    };
    header1 += '\"' + title + "\";\r\n";

    std::string header2
    {
        "\r\n"
        "    // Binary Search Tree Properties\r\n"
        "    // --------------------------------------------------------------------------------\r\n"
        "    graph [ dpi = 300, nodesep = 0.25, ranksep = 0.5, splines = line, margin = 0.25 ];\r\n"
        "    node  [ fontname = \"Bitstream Vera Sans\",\r\n"
        "            fontsize = 14, fontcolor = white,\r\n"
        "            style = filled, fillcolor = azure4,\r\n"
        "            shape = Mrecord, fixedsize = false ];\r\n"
        "    // --------------------------------------------------------------------------------\r\n"
    };

    std::string header {header1 + header2};

    std::string labels
    {
        "    // Labels\r\n"
        "    // --------------------------------------------------------------------------------\r\n"
    };

    std::string relations
    {
        "    // Relations\r\n"
        "    // --------------------------------------------------------------------------------\r\n"
    };

    Node* root {m_root};
    labels += "    cd" + std::to_string(root->data[0].pk);
    labels += " [ " + root->label + " ];\r\n";
    labels += "\r\n";

    Stack<Node*> stack;
    stack.push(root);
    while(!stack.empty())
    {
        root = stack.pop();
        if(root != nullptr && !root->isLeaf)
        {
            // --------------------------------------------------------------------
            std::string parentLabel { "cd" + std::to_string(root->data[0].pk) };
            for(std::size_t i = 0; i < root->size + 1; ++i)
            {
                if(isFinal)
                {
                    diskRead(root->children[i], root->childrenIndices[i]);
                }

                if(root->children[i] != nullptr)
                {
                    labelNode(root->children[i]);
                    std::string childLabel = "cd"  + std::to_string(root->children[i]->data[0].pk);
                    labels += "    " + childLabel + " [ " + root->children[i]->label + " ];\r\n";
                    relations += "    ";
                    relations += '\"' + parentLabel + "\":c" + std::to_string(i) + " -- ";
                    relations += '\"' + childLabel + "\";\r\n";
                    stack.push(root->children[i]);
                }
            }
            // --------------------------------------------------------------------
        }
    }

    std::string content {header + "\r\n" + labels + "\r\n" + relations + "}\r\n"};
    of << content;
    of.close();
}

template <typename T, int N>
inline void BTree<T, N>::labelNode(Node* node)
{
    if(node != nullptr)
    {
        std::stringstream ss;
        ss << std::uppercase << std::hex << node->index;
        std::string label { "label = \"{ DRA: 0x" + ss.str() + " | {" };
        for(std::size_t i = 0; i < node->size; ++i)
        {
            label += " <c" + std::to_string(i) + "> | "
                           + std::to_string(node->data[i]) + " |";
        }
        label += " <c" + std::to_string(node->size) + "> } }\"";
        node->label = std::move(label);
    }
}

template <typename T, int N>
inline void BTree<T, N>::splitChild(Node* node, const std::size_t index)
{
    std::size_t middle = std::ceil(static_cast<float>(N) / 2.0f) - 1;

    Node* y = node->children[index];
    Node* z = new Node { {}, N - (middle + 2), allocate(), {}, y->isLeaf, {}, "" };

    for(std::size_t i = 0; i < z->size; ++i)
    {
        z->data[i] = y->data[middle + 1 + i];
    }
    if(!y->isLeaf)
    {
        for(std::size_t i = 0; i < z->size + 1; ++i)
        {
            z->children[i] = y->children[middle + 1 + i];
            z->childrenIndices[i] = y->childrenIndices[middle + 1 + i];
        }
    }
    y->size -= z->size + 1;
    *node >> index;
    node->children[index + 1] = z;
    node->childrenIndices[index + 1] = z->index;
    node->data[index] = y->data[middle];
    labelNode(y);
    diskWrite(y);
    labelNode(z);
    diskWrite(z);
    labelNode(node);
    diskWrite(node);
    if(m_debugMode)
    {
        this->exportToFile("Splitted", true);
    }
}

template <typename T, int N>
inline void BTree<T, N>::insertKeyNonFull(const AttachedPair<T>& value, Node*& root)
{
    int i = root->size - 1;
    for(std::size_t j = 0; j < root->size; ++j)
    {
        if(root->data[j] == value)
        {
            std::cout << "Value already exists" << std::endl;
            return;
        }
    }
    if(root->isLeaf)
    {
        while(i >= 0 && value < root->data[i])
        {
            root->data[i + 1] = root->data[i];
            --i;
        }
        root->data[i + 1] = value;
        ++root->size;
        labelNode(root);
        diskWrite(root);
    }
    else
    {
        while(i >= 0 && value < root->data[i]) --i;
        ++i;
        diskRead(root->children[i], root->childrenIndices[i]);
        if(root->children[i]->size >= N - 1)
        {
            splitChild(root, i);
            if(value > root->data[i])
            {
                ++i;
            }
        }
        insertKeyNonFull(value, root->children[i]);
    }
}

template <typename T, int N>
inline void BTree<T, N>::clearFile()
{
    m_file.open("data/data.bt", std::ios::out | std::ios::binary | std::ios::trunc);
    m_file.close();

    m_bkFile.open("data/data.bt", std::ios::out | std::ios::binary | std::ios::trunc);
    m_bkFile.close();
}

template <typename T, int N>
inline void BTree<T, N>::createFileIfNotExists()
{
    m_file.open("data/data.bt", std::ios::in | std::ios::out | std::ios::binary);
    m_bkFile.open("data/dataBK.bt", std::ios::in | std::ios::out | std::ios::binary);

    if(!m_file.good())
    {
        if(m_bkFile.good())
        {
            m_file.open("data/data.bt", std::ios::out | std::ios::binary);
            m_file << m_bkFile.rdbuf();
        }
        else
        {
            m_file.open("data/data.bt", std::ios::out | std::ios::binary);
            m_bkFile.open("data/dataBK.bt", std::ios::out | std::ios::binary);
        }
    }
    else if(!m_bkFile.good())
    {
        m_bkFile.open("data/dataBK.bt", std::ios::out | std::ios::binary);
        m_bkFile << m_file.rdbuf();
    }

    m_flagsFile.open("data/flags.meta", std::ios::in | std::ios::binary);
    char f {};
    m_flagsFile.read(&f, sizeof(char));
    switch(f)
    {
        case 1:
            {
                m_file << m_bkFile.rdbuf();
            }
            break;

        case 2:
            {
                m_bkFile << m_file.rdbuf();
            }
            break;

        default:
            break;
    }
    m_flagsFile.close();

    m_file.close();
    m_bkFile.close();
}

template <typename T, int N>
inline void BTree<T, N>::diskWrite(const Node* node)
{
    const char* buffer = reinterpret_cast<const char*>(node);

    m_writeTarget->seekp(node->index, std::ios::beg);
    m_writeTarget->write(buffer, sizeof_BTNRelevantData(T, N));
}

template <typename T, int N>
inline void BTree<T, N>::diskRead(Node*& node, const std::size_t index)
{
    node = new Node { {}, 0, 0, {}, true, {}, "" };
    char* buffer = reinterpret_cast<char*>(node);

    m_file.seekg(index, std::ios::beg);
    m_file.read(buffer, sizeof_BTNRelevantData(T, N));
}

template <typename T, int N>
inline std::size_t BTree<T, N>::allocate()
{
    char* buffer = new char[sizeof_BTNRelevantData(T, N)];

    m_writeTarget->seekp(m_nextIndex, std::ios::beg);
    m_writeTarget->write(buffer, sizeof_BTNRelevantData(T, N));

    delete[] buffer;

    std::size_t index {m_nextIndex};
    m_nextIndex += sizeof_BTNRelevantData(T, N);

    return index;
}

template <typename T, int N>
inline std::size_t BTree<T, N>::getRootIndex()
{
    std::size_t rootIndex {};
    char* buffer = reinterpret_cast<char*>(&rootIndex);

    m_file.seekg(0, std::ios::beg);
    m_file.read(buffer, sizeof(std::size_t));
    return rootIndex;
}

template <typename T, int N>
inline void BTree<T, N>::setRootIndex(const std::size_t index)
{
    const char* buffer = reinterpret_cast<const char*>(&index);

    m_writeTarget->seekp(0, std::ios::beg);
    m_writeTarget->write(buffer, sizeof(std::size_t));
}

template <typename T, int N>
inline void BTree<T, N>::setTransactionState(const char state)
{
    m_flagsFile.seekp(0, std::ios::beg);
    m_flagsFile.write(&state, sizeof(char));
}
