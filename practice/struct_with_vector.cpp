#include <iostream>
#include <memory>
#include <vector>

struct Sizeme {
    std::vector<int> vec1;
};

int main(void) {

    std::vector<std::unique_ptr<Sizeme>> up_vec;
    up_vec.push_back(std::make_unique<Sizeme>());
    std::cout << "[capacity,address] of vec before push is: [" << (*up_vec.at(0)).vec1.capacity() << ", " << &((*up_vec.at(0)).vec1) << "]\n";
    std::cout << "size of vec: " << sizeof((*up_vec.at(0)).vec1) << "\n";
    (*up_vec.at(0)).vec1.push_back(1);
    std::cout << "[capacity,address] of vec after push is: [" << (*up_vec.at(0)).vec1.capacity() << ", " << &((*up_vec.at(0)).vec1) << "]\n";
    std::cout << "size of vec: " << sizeof((*up_vec.at(0)).vec1) << "\n";
    int cap = (*up_vec.at(0)).vec1.capacity();
    for (int i=0; i < cap*10 + 100; ++i) {
        std::cout << "i is: " << i << "\n";
        (*up_vec.at(0)).vec1.push_back(i);
        std::cout << "[capacity,address] of vec before push is: [" << (*up_vec.at(0)).vec1.capacity() << ", " << &((*up_vec.at(0)).vec1) << "]\n";
        std::cout << "size of vec: " << sizeof((*up_vec.at(0)).vec1) << "\n";
    }

}