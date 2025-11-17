#include "../cpp/Platform.Interfaces/CDictionary.h"
#include "../cpp/Platform.Interfaces/CList.h"
#include "../cpp/Platform.Interfaces/CSet.h"
#include <map>
#include <vector>
#include <set>
#include <iostream>

using namespace Platform::Interfaces;

// Test that const qualified types work with readonly concepts
void test_readonly_concepts() {
    // Test with const std::map
    const std::map<int, std::string> const_map = {{1, "one"}, {2, "two"}};
    
    // This should work with CReadonlyDictionary
    static_assert(CReadonlyDictionary<const std::map<int, std::string>>);
    
    // Test with const std::vector
    const std::vector<int> const_vec = {1, 2, 3};
    
    // This should work with CReadonlyList
    static_assert(CReadonlyList<const std::vector<int>>);
    
    // Test with const std::set
    const std::set<int> const_set = {1, 2, 3};
    
    // This should work with CReadonlySet
    static_assert(CReadonlySet<const std::set<int>>);
    
    std::cout << "All readonly concept tests passed!" << std::endl;
}

// Test that regular concepts still work with non-const types
void test_mutable_concepts() {
    std::map<int, std::string> mutable_map;
    std::vector<int> mutable_vec;
    std::set<int> mutable_set;
    
    // These should work with regular concepts
    static_assert(CDictionary<std::map<int, std::string>>);
    static_assert(CList<std::vector<int>>);
    static_assert(CSet<std::set<int>>);
    
    std::cout << "All mutable concept tests passed!" << std::endl;
}

int main() {
    test_readonly_concepts();
    test_mutable_concepts();
    std::cout << "All tests completed successfully!" << std::endl;
    return 0;
}