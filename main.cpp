#include <random>
#include <iostream>

#include <maved.hpp>

int main()
{
    mav::BTree<int, 4> bTree {};

    int index {};
    bTree.find({361, 18}, index);

    bTree.exportToFile("Path B-Tree");
    bTree.exportToFile("Final B-Tree", true);

    return 0;
}
