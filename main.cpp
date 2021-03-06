#include "rb_tree.hpp"
#include "tests.hpp"

#include <map>
#include <set>
#include <iterator>
#include <chrono>

void demo() {
    rb_tree<int> test_tree;
    std::map<int, const node_base<int> *> nodes_to_delete;
    for (int i = 0; i < 20; ++i) {
        auto value = rand() % 90 + 10;
        std::cout << "insert " << value << std::endl;
        auto node = test_tree.insert(value);
        test_tree.print_by_level(std::cout);
        check<int>(test_tree.get_root());
        nodes_to_delete.insert(std::make_pair(value, node));
    }
    for (int i = 0; i < 10; ++i) {
        auto it = nodes_to_delete.begin();
        std::advance(it, i);
        std::cout << "erase " << it->second->get_value() << std::endl;
        test_tree.erase(it->second);
        test_tree.print_by_level(std::cout);
        check<int>(test_tree.get_root());
        nodes_to_delete.erase(it->first);
    }

    // iterator demo
    auto nil = test_tree.get_root()->parent;
    auto first = test_tree.get_root();
    while (first->left != nil) first = first->left;
    const_rb_tree_iterator<int> begin(first, nil);
    const_rb_tree_iterator<int> end(nil, nil);
    while (begin != end) {
        std::cout << begin->get_value() << " ";
        ++begin;
    }
    std::cout << std::endl;

    begin = const_rb_tree_iterator<int>(first, nil);
    do {
        --end;
        std::cout << end->get_value() << " ";
    } while (begin != end);
    std::cout << std::endl;

    // or just..
    for (auto & node : test_tree) {
        std::cout << node.get_value() << " ";
    }
    std::cout << std::endl;
}

void measure_n(unsigned n) {
    using namespace std::chrono;
    std::cout << std::setw(7) << n << ",";
    {
        rb_tree<int> x;
        auto start = std::chrono::high_resolution_clock::now();
        for (auto i = 1u; i < n; ++i) {
            x.insert(rand());
        }
        auto duration = std::chrono::high_resolution_clock::now() - start;
        std::cout << std::setw(14)
                  << duration_cast<milliseconds>(duration).count()
                  << ",";
    }
    {
        std::set<int> x;
        auto start = std::chrono::high_resolution_clock::now();
        for (auto i = 1u; i < n; ++i) {
            x.insert(rand());
        }
        auto duration = std::chrono::high_resolution_clock::now() - start;
        std::cout << std::setw(14)
                  << duration_cast<milliseconds>(duration).count()
                  << "," << std::endl;
    }
}

void measure() {
    std::cout << std::setw(23) << "my impl(ms)"
              << std::setw(15) << "std::set(ms)" << std::endl;
    for (unsigned n = 100000; n < 1000000; n += 100000) {
        measure_n(n);
    }
}

int main() {
    //test();
    demo();
    return 0;
}

