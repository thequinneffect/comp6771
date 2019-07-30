#include <unordered_set>
#include <iostream>

int main(void) {

    std::unordered_set<int> uset {};
    

    uset.insert(1);
    uset.insert(2);
    uset.insert(3);
    uset.insert(4);
    uset.insert(5);
    
    for (auto it = uset.begin(); it != uset.end(); ++it) std::cout << *it << "\n";
    std::cout << "elements printed!\n";

}