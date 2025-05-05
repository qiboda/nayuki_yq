#include <gtest/gtest.h>

#include <Core/Delegate/Delegate.h>

class DelegateTest
    : public ::testing::Test
{
protected:
    DelegateTest() {}
    ~DelegateTest() override {}

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( DelegateTest, MultipleDelegateOnce )
{
    MultipleDelegate<void, std::string&> delegate;
    delegate.Add(
        []( std::string& str )
        {
            str.append( "add" );
        } );
    std::string str( "add" );
    delegate.Broadcast( str );
    ASSERT_EQ( str, "addadd" );
}

TEST_F( DelegateTest, MutipleDelegateAddOrderExec )
{
    MultipleDelegate<void, std::string&> delegate;
    DelegateHandle Handle1 = delegate.Add(
        []( std::string& str )
        {
            str.append( "aadd" );
        } );
    DelegateHandle Handle2 = delegate.Add(
        []( std::string& str )
        {
            str.append( "add" );
        } );
    std::string str( "add" );
    ASSERT_TRUE( delegate.Remove( Handle1 ) );
    delegate.Broadcast( str );
    ASSERT_EQ( str, "addadd" );
}

class A
{
public:
    void addStr( std::string& str )
    {
        str.append( "app" );
    }
};

TEST_F( DelegateTest, MutipleDelegateObject )
{
    A a;
    MultipleDelegate<void, std::string&> delegate;
    DelegateHandle Handle = delegate.AddMem( &A::addStr, &a );
    std::string str( "add" );
    delegate.Broadcast( str );
    ASSERT_EQ( str, "addapp" );
}

TEST_F( DelegateTest, DelegateSPObject )
{
    std::shared_ptr<A> a = std::make_shared<A>();
    MultipleDelegate<void, std::string&> delegate;
    DelegateHandle Handle = delegate.AddSP( &A::addStr, a );
    std::string str( "add" );
    delegate.Broadcast( str );
    ASSERT_EQ( str, "addapp" );
}

TEST_F( DelegateTest, SingleDelegateReturnVal )
{
    SingleDelegate<i32, f64> delegate;
    delegate.Bind(
        []( f64 d )->i32
        {
            if ( d > 0.0 ) { return 10; }
            return 0;
        } );
    ASSERT_EQ( delegate.Execute( 3 ), 10 );
    ASSERT_EQ( delegate.Execute( -1.0f ), 0 );
}

TEST_F( DelegateTest, SingleDelegateBindOnlyOnce )
{
    SingleDelegate<i32, f64> delegate;
    delegate.Bind(
        []( f64 d )->i32
        {
            if ( d > 0.0 ) { return 10; }
            return 0;
        } );

    ASSERT_DEATH(
        delegate.Bind(
            []( f64 d )->i32
            {
                if ( d > 0.0 ) { return 10; }
                return 0;
            } ),
        "mDelegateInstance == nullptr"
    );
}

TEST_F( DelegateTest, SingleDelegateUnbindAndInvoking )
{
    SingleDelegate<i32, f64> delegate;
    delegate.Bind(
        []( f64 d )->i32
        {
            if ( d > 0.0 ) { return 10; }
            return 0;
        } );
    delegate.Unbind();

    ASSERT_DEATH(
        delegate.Execute( 32.0 ),
        "mDelegateInstance != nullptr"
    );
}

TEST_F( DelegateTest, DelegateMove )
{
    SingleDelegate<void> delegate1;
    delegate1.Bind( []() {} );
    SingleDelegate<void> delegate2( std::move( delegate1 ) );

    ASSERT_DEATH(
        delegate1.Execute(),
        ""
    );
    bool is_same = std::is_same_v<decltype( delegate2.Execute() ), void>;
    ASSERT_TRUE( is_same );
}