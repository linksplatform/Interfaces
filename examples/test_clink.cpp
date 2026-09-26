#include "../cpp/Platform.Interfaces/Platform.Interfaces.h"
#include <iostream>
#include <cassert>

using namespace Platform::Interfaces;

// Test implementation that satisfies CLink concept
struct TestLink {
    using value_type = int;
    
    value_type begin;
    value_type end;
    
    TestLink(int b, int e) : begin(b), end(e) {}
    
    bool empty() const {
        return begin == end;
    }
};

// Test implementation that does NOT satisfy CLink concept (missing empty method)
struct BadTestLink {
    using value_type = int;
    value_type begin;
    value_type end;
};

int main() {
    // Test that TestLink satisfies CLink concept
    static_assert(CLink<TestLink>, "TestLink should satisfy CLink concept");
    
    // Test that BadTestLink does not satisfy CLink concept
    static_assert(!CLink<BadTestLink>, "BadTestLink should NOT satisfy CLink concept");
    
    // Test runtime behavior
    TestLink link1(5, 10);
    TestLink link2(5, 5);
    
    assert(!link1.empty());
    assert(link2.empty());
    assert(link1.begin == 5);
    assert(link1.end == 10);
    
    // Test Link helper struct
    static_assert(std::same_as<Link<TestLink>::value_type, int>, 
                  "Link helper should provide correct value_type");
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}