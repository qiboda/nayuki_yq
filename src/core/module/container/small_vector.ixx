export module core.container.small_vector;

import core;
import std;
import core.misc.non_copyable;

export template <typename T, usize N>
class SmallVector : public NonCopyable
{
  public:
    static constexpr usize MAX_SIZE = 32;
    static constexpr usize SIZE = N;
    static_assert( N <= MAX_SIZE, "N must be less than or equal to MAX_SIZE" );

  private:
    alignas( T ) u8 InlineStorage[sizeof( T ) * N];
    T* mData = reinterpret_cast<T*>( InlineStorage );

    usize mSize = 0;
    usize mCapacity = N;
    bool UsingInline = true;

    /**
     * @brief 当大于N时，重新分配内存
     * @param new_capacity
     */
    void reallocate( usize new_capacity )
    {
        // TODO 优化，使用inline专用的内存块，这样可以原地扩展内存？
        T* newData = reinterpret_cast<T*>( ::operator new( sizeof( T ) * new_capacity ) );
        for ( usize i = 0; i < mSize; ++i )
            new ( newData + i ) T( std::move( mData[i] ) );

        destroy_elements();
        if ( !UsingInline )
            ::operator delete( mData );

        mData = newData;
        mCapacity = new_capacity;
        UsingInline = false;
    }

    void destroy_elements()
    {
        for ( std::size_t i = 0; i < mSize; ++i )
            mData[i].~T();
        mSize = 0;
    }

  public:
    SmallVector() = default;

    ~SmallVector()
    {
        destroy_elements();
        if ( !UsingInline )
            ::operator delete( mData );
    }

    SmallVector( SmallVector&& other )
        : mData( std::move( other.mData ) )
        , mSize( std::move( other.mSize ) )
        , mCapacity( std::move( other.mCapacity ) )
        , UsingInline( std::move( other.UsingInline ) )
    {
        other.mData = nullptr;
        other.mSize = 0;
        other.mCapacity = 0;
        other.UsingInline = true;
    }

    SmallVector operator=( SmallVector&& other )
    {
        mData = std::move( other.mData );
        mSize = std::move( other.mSize );
        mCapacity = std::move( other.mCapacity );
        UsingInline = std::move( other.UsingInline );

        other.mData = nullptr;
        other.mSize = 0;
        other.mCapacity = 0;
        other.UsingInline = true;
    }

    void push_back( const T& val )
    {
        if ( mSize >= mCapacity )
            reallocate( mCapacity * 2 );
        new ( mData + mSize ) T( val );
        ++mSize;
    }

    void push_back( T&& val )
    {
        if ( mSize >= mCapacity )
            reallocate( mCapacity * 2 );
        new ( mData + mSize ) T( std::move( val ) );
        ++mSize;
    }

    void reserve( usize size )
    {
        if ( mSize >= mCapacity )
            reallocate( size );
    }

    T& operator[]( usize index )
    {
        return mData[index];
    }

    const T& operator[]( usize index ) const
    {
        return mData[index];
    }

    usize size() const
    {
        return mSize;
    }

    T* begin()
    {
        return mData;
    }

    T* end()
    {
        return mData + mSize;
    }

    const T* begin() const
    {
        return mData;
    }

    const T* end() const
    {
        return mData + mSize;
    }
};
