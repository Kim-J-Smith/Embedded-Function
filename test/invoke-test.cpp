#include "test.hpp"
#include "embed/embed_function.hpp"

TEST_FUNCTION_DECLARE(InvokeTest, AssertSameTest);
TEST_FUNCTION_DECLARE(InvokeTest, SelfCall);
TEST_FUNCTION_DECLARE(InvokeTest, ReferenceParamSignatureTest);
TEST_FUNCTION_DECLARE(InvokeTest, MemberLambdaInvokeTest);
TEST_FUNCTION_DECLARE(InvokeTest, DiamondInheritanceTset);
TEST_FUNCTION_DECLARE(InvokeTest, VirtualMethodTest);
TEST_FUNCTION_DECLARE(InvokeTest, CVRefQualifierTest);

TEST_SUBSYS(InvokeTest, main) {
    TEST_RUN(InvokeTest, AssertSameTest);
    TEST_RUN(InvokeTest, SelfCall);
    TEST_RUN(InvokeTest, ReferenceParamSignatureTest);
    TEST_RUN(InvokeTest, MemberLambdaInvokeTest);
    TEST_RUN(InvokeTest, DiamondInheritanceTset);
    TEST_RUN(InvokeTest, VirtualMethodTest);
    TEST_RUN(InvokeTest, CVRefQualifierTest);
}

static void testUse__normal_func(void) noexcept { }

TEST(InvokeTest, AssertSameTest) {
    auto fn = embed::make_function(testUse__normal_func);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    fn();

    return 0;
}

static void testUse__self_call(void) noexcept {
    auto fn = embed::make_function(testUse__normal_func);
    fn();
}

TEST(InvokeTest, SelfCall) {
    auto fn = embed::make_function(testUse__self_call);

    ASSERT_EQ(static_cast<bool>(fn), true, "%d");
    fn();

    return 0;
}

static int testUse__foo(int& i) { return i; }

TEST(InvokeTest, ReferenceParamSignatureTest) {
    auto fn1 = embed::make_function(testUse__foo);
    embed::function<int(int&)> fn2 = testUse__foo;

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    int test = 2183719;

    ASSERT_EQ(fn1(test), test, "%d");
    ASSERT_EQ(fn2(test), test, "%d");

    return 0;
}

class testUse__ClassMemberLambdaInvokeTest {
public:
    using CallbackT = embed::function<int(void), sizeof(int)>;
    void bar(void) {
        int x = 10;
        CallbackT c = [=]() { return x; };
    }
};

TEST(InvokeTest, MemberLambdaInvokeTest) {
    testUse__ClassMemberLambdaInvokeTest a;
    a.bar();
    return 0;
}


class testUse__ClassDiamondInheritanceTset
: public testUse__Base1, public testUse__Base2
{
public:
    void foo() override {}
    int foo(int a) override { return a + 100; }
};

TEST(InvokeTest, DiamondInheritanceTset) {
    using test_t = testUse__ClassDiamondInheritanceTset;

    test_t c;

    auto fn1 = embed::make_function(
        static_cast<void(test_t::*)()>(&test_t::foo)
    );
    auto fn2 = embed::make_function(
        static_cast<int(test_t::*)(int)>(&test_t::foo)
    );

    ASSERT_EQ(static_cast<bool>(fn1), true, "%d");
    ASSERT_EQ(static_cast<bool>(fn2), true, "%d");

    ASSERT_EQ(fn1.buffer_size > sizeof(void*), true, "%d");
    ASSERT_EQ(fn2.buffer_size > sizeof(void*), true, "%d");

    ASSERT_EQ(fn2(c, 100), 200, "%d");

    return 0;
}

struct testUse__InvokeTestA {
    virtual int operator()() { return 1; }
    virtual ~testUse__InvokeTestA() = default;
};

struct testUse__InvokeTestB : public testUse__InvokeTestA {
    int operator()() override { return 2; }
};

TEST(InvokeTest, VirtualMethodTest) {
    embed::function<int()> fn = testUse__InvokeTestA{};

    ASSERT_EQ(fn == nullptr, false, "%d");
    ASSERT_EQ(fn != nullptr, true, "%d");

    int x = fn();

    fn = testUse__InvokeTestB{};

    int y = fn();
    
    ASSERT_EQ(x, 1, "%d");
    ASSERT_EQ(y, 2, "%d");

    return 0;
}

struct InvokeTest_CVRefQualifierTest__struct
{
    int operator()() const volatile {
        volatile int a = 123456;
        return a;
    }

    float operator()(int) const && {
        volatile float f = 3.1415926535f;
        return f;
    }

    double operator()(int, int) {
        volatile double d = 2.70000000;
        a++;
        return d;
    }

    int a = 0;
};

TEST(InvokeTest, CVRefQualifierTest) {

    embed::function<int() const, 2*sizeof(void*)> fn1 = InvokeTest_CVRefQualifierTest__struct{};
    embed::function<int(), 2*sizeof(void*)> fn2 = fn1;
    embed::function<int() &, 2*sizeof(void*)> fn3 = fn1;
    embed::function<int() &&, 2*sizeof(void*)> fn4 = fn1;
    embed::function<int() volatile, 2*sizeof(void*)> fn5 = 
        InvokeTest_CVRefQualifierTest__struct{};

    fn2 = fn1;
    fn3 = fn1;
    fn4 = fn1;

    ASSERT_EQ(fn2(), 123456, "%d");
    ASSERT_EQ(fn3(), 123456, "%d");
    ASSERT_EQ(std::move(fn2)(), 123456, "%d");
    ASSERT_EQ(fn3(), 123456, "%d");
    ASSERT_EQ(std::move(fn4)(), 123456, "%d");
    ASSERT_EQ(fn5(), 123456, "%d");

    embed::function<float(int) const &&> fn6 = InvokeTest_CVRefQualifierTest__struct{};
    ASSERT_EQ_F(std::move(fn6)(1), 3.1415926535f, 0.00001);

    embed::function<float(int) &&> fn7 = std::move(fn6);
    ASSERT_EQ_F(std::move(fn7)(1), 3.1415926535f, 0.00001);

    return 0;
}

