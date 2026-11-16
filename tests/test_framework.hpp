#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>

struct TestCase { std::string name; std::function<bool()> fn; };
inline std::vector<TestCase>& test_registry() { static std::vector<TestCase> r; return r; }
struct TestRegistrar { TestRegistrar(std::string n, std::function<bool()> f) { test_registry().push_back({std::move(n), std::move(f)}); } };

#define TEST(name) \
    static bool test_##name(); \
    static TestRegistrar reg_##name(#name, test_##name); \
    static bool test_##name()

#define ASSERT_TRUE(x) do { if(!(x)) { std::cerr << "  ASSERT_TRUE failed: " #x " at line " << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_FALSE(x) do { if(x) { std::cerr << "  ASSERT_FALSE failed: " #x " at line " << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_EQ(a,b) do { if(!((a)==(b))) { std::cerr << "  ASSERT_EQ failed at line " << __LINE__ << std::endl; return false; } } while(0)
#define ASSERT_NE(a,b) do { if((a)==(b)) { std::cerr << "  ASSERT_NE failed at line " << __LINE__ << std::endl; return false; } } while(0)

inline int run_all_tests() {
    int passed = 0;
    int failed = 0;
    for (auto& t : test_registry()) {
        std::cout << "[RUN ] " << t.name << std::endl;
        bool ok = false;
        try { ok = t.fn(); } catch (const std::exception& e) {
            std::cerr << "  EXCEPTION: " << e.what() << std::endl; ok = false;
        } catch (...) { std::cerr << "  UNKNOWN EXCEPTION" << std::endl; ok = false; }
        if (ok) { std::cout << "[ OK ] " << t.name << std::endl; ++passed; }
        else { std::cout << "[FAIL] " << t.name << std::endl; ++failed; }
    }
    std::cout << "\nResults: " << passed << " passed, " << failed << " failed, " << (passed+failed) << " total" << std::endl;
    return failed == 0 ? 0 : 1;
}
