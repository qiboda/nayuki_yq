#pragma once

#include "core/concepts/tuple.h"
#include "core/macro/macro.h"
#include "ecs/archetype/define.h"
#include <core/memory/memory.h>
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include "ecs/components/component_concept.h"
#include "module_export.h"
#include <range/v3/view/enumerate.hpp>

// 对齐到 64 bit，对齐到缓存行大小，避免触发缓存一致性机制（MESI协议）[“伪共享”（False Sharing）]
class ECS_API ArchetypeChunk
{
  public:
    explicit ArchetypeChunk( usize MaxEntityNum, usize memorySize = 16 * 1024 )
        : MaxEntityNum( MaxEntityNum )
    {
        // std::cout << "ArchetypeChunk::ArchetypeChunk " << memorySize << std::endl;
        mArchetypeComponentData = new u8[memorySize]();
    }

    ~ArchetypeChunk()
    {
        if ( mArchetypeComponentData )
        {
            delete[] mArchetypeComponentData;
        }
    }

  public:
    inline bool CanAddEntity() const
    {
        return mEntities.size() < MaxEntityNum;
    }

  public:
    inline void AddEntity( Entity entity, const ArchetypeComponentMemoryInfo& memoryInfo )
    {
        mEntities.push_back( entity );
        AddEntityComponentData( entity, memoryInfo );
    }

    inline void DestroyEntity( Entity entity, const std::vector<ArchetypeComponentMemoryInfo>& memoryInfos )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        if ( it != mEntities.end() )
        {
            *it = std::move( mEntities.back() ); // 用最后一个元素覆盖它
            usize entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );
            usize entityNum = mEntities.size();
            mEntities.pop_back(); // 移除最后一个

            for ( auto&& memoryInfo : memoryInfos )
            {
                u8* removedComponentData =
                    mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
                u8* lastComponentData =
                    mArchetypeComponentData + memoryInfo.mTotalOffset + entityNum * memoryInfo.mComponentSize;

                auto* component = reinterpret_cast<Component*>( removedComponentData );
                component->~Component();

                std::memcpy( removedComponentData, lastComponentData, memoryInfo.mComponentSize );
            }
        }
    }

    inline bool ContainEntity( Entity entity )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        return it != mEntities.end();
    }

    inline usize GetEntityNum() const
    {
        return mEntities.size();
    }

  public:
    // 添加组件数据
    template <IsRegularComponentConcept T>
    void AddComponentData( Entity entity, const ArchetypeComponentMemoryInfo& memoryInfo, T&& component )
    {
        NY_ASSERT( mArchetypeComponentData != nullptr );

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        NY_ASSERT( it != mEntities.end() );
        usize entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );

        u8* componentData = mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
        new ( componentData ) T( std::forward<T>( component ) );
    }

  protected:
    // 添加组件数据
    void AddEntityComponentData( Entity entity, const ArchetypeComponentMemoryInfo& memoryInfo )
    {
        NY_ASSERT( mArchetypeComponentData != nullptr );

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        NY_ASSERT( it != mEntities.end() );
        usize entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );

        u8* componentData = mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
        new ( componentData ) Entity( std::forward<Entity>( entity ) );
    }

  public:
    template <IsRegularComponentConcept T>
    void ReplaceComponentData( Entity entity, ArchetypeComponentMemoryInfo& memoryInfo, T&& newComponent )
    {
        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        NY_ASSERT( it != mEntities.end() );
        usize entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );

        u8* componentData = mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;

        Component* component = reinterpret_cast<Component*>( componentData );
        component->~Component();

        new ( componentData ) T( std::forward<T>( newComponent ) );
    }

  public:
    template <IsComponentConcept T>
    T* GetComponents( const ArchetypeComponentMemoryInfo& memoryInfo )
    {
        u8* componentBaseData = mArchetypeComponentData + memoryInfo.mTotalOffset;
        return reinterpret_cast<T*>( componentBaseData );
    }

    std::vector<u8*> MoveOutEntity( Entity entity, const std::vector<ArchetypeComponentMemoryInfo>& memoryInfos )
    {
        usize entityTotalNum = mEntities.size();

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        if ( it == mEntities.end() )
        {
            return {};
        }

        size_t entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );

        *it = std::move( mEntities.back() );
        mEntities.pop_back();

        std::vector<u8*> movedComponents;
        movedComponents.reserve( memoryInfos.size() );
        for ( auto&& memoryInfo : memoryInfos )
        {
            u8* movedComponent =
                mArchetypeComponentData + memoryInfo.mTotalOffset + entityIndex * memoryInfo.mComponentSize;
            u8* lastComponent =
                mArchetypeComponentData + memoryInfo.mTotalOffset + ( entityTotalNum - 1 ) * memoryInfo.mComponentSize;

            Memory::Swap( movedComponent, lastComponent, memoryInfo.mComponentSize );
            movedComponents.push_back( lastComponent );
        }

        return movedComponents;
    }

    void MoveIntoEntityComponent( Entity entity,
                                  const std::vector<u8*>& srcComponents,
                                  const usize srcIndex,
                                  const usize destIndex,
                                  const std::vector<ArchetypeComponentMemoryInfo>& memoryInfos )
    {
        u8* srcComponentData = srcComponents[srcIndex];

        auto it = std::find( mEntities.begin(), mEntities.end(), entity );
        usize entityIndex = static_cast<usize>( std::distance( mEntities.begin(), it ) );
        u8* destComponentData = mArchetypeComponentData + memoryInfos[destIndex].mTotalOffset +
                                entityIndex * memoryInfos[destIndex].mComponentSize;

        std::memcpy( destComponentData, srcComponentData, memoryInfos[destIndex].mComponentSize );
    }

  public:
    template <IsComponentConcept Component>
    class ECS_API ComponentIterator
    {
      public:
        using ComponentType = Component;

      public:
        ComponentIterator( ArchetypeChunk* chunk, const ArchetypeComponentMemoryInfo& info )
            : mChunk( chunk )
            , mInfo( info )
        {
            mData = mChunk->GetComponents<Component>( mInfo );
        }

        ComponentIterator( const ComponentIterator& iterator )
            : mChunk( iterator.mChunk )
            , mInfo( iterator.mInfo )
            , mOffset( iterator.mOffset )
            , mData( iterator.mData )
        {
        }

        // ComponentIterator& operator=( const ComponentIterator& iterator )
        // {
        //     mChunk = iterator.mChunk;
        //     mInfo = iterator.mInfo;
        //     mOffset = iterator.mOffset;
        //     mData = iterator.mData;
        // }

        ComponentIterator<Component>& begin()
        {
            mOffset = 0;
            return *this;
        }

        ComponentIterator<Component>& end()
        {
            mOffset = mChunk->mEntities.size();
            return *this;
        }

      public:
        Component* operator->()
        {
            NY_ASSERT( mOffset >= 0 && mOffset < mChunk->mEntities.size() )
            return mData + mOffset;
        }

        Component& operator*()
        {
            NY_ASSERT( mOffset >= 0 && mOffset < mChunk->mEntities.size() )
            return *( mData + mOffset );
        }

        void operator++()
        {
            mOffset++;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        void operator--()
        {
            mOffset--;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        void operator+=( usize size )
        {
            mOffset += size;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        void operator-=( usize size )
        {
            mOffset -= size;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        bool operator==( const ComponentIterator& other ) const
        {
            return mOffset == other.mOffset;
        }

        bool IsEnd() const
        {
            return mOffset >= mChunk->mEntities.size();
        }

      protected:
        ArchetypeChunk* mChunk = nullptr;
        const ArchetypeComponentMemoryInfo& mInfo;
        Component* mData;
        usize mOffset = 0;
    };

    template <typename... T>
        requires IsComponentSetConcept<T...>
    class ECS_API ComponentSetIterator
    {
      public:
        using ComponentTypes = std::tuple<T...>;
        static const usize ComponentNum = sizeof...( T );
        using MemoryInfosType = std::array<ArchetypeComponentMemoryInfo, ComponentNum>;

      public:
        ComponentSetIterator()
            : mInfos()
        {
        }

        ComponentSetIterator( ArchetypeChunk* chunk, MemoryInfosType&& infos )
            : mChunk( chunk )
            , mInfos( std::forward<MemoryInfosType>( infos ) )
        {
            NY_ASSERT( mInfos.size() == ComponentNum );
            FillComponentImpl( std::make_index_sequence<ComponentNum>(), mInfos );
        }

        ComponentSetIterator( const ComponentSetIterator& iterator )
            : mChunk( iterator.mChunk )
            , mComponents( iterator.mComponents )
            , mInfos( iterator.mInfos )
            , mOffset( iterator.mOffset )
        {
        }

        ComponentSetIterator& operator=( const ComponentSetIterator& iterator )
        {
            mChunk = iterator.mChunk;
            mInfos = iterator.mInfos;
            mComponents = iterator.mComponents;
            mOffset = iterator.mOffset;
            return *this;
        }

        bool IsValid() const
        {
            return mChunk != nullptr;
        }

        ComponentIterator<T...>& begin()
        {
            mOffset = 0;
            return *this;
        }

        ComponentIterator<T...>& end()
        {
            mOffset = mChunk->mEntities.size();
            return *this;
        }

      private:
        template <usize... I>
        void FillComponentImpl( std::index_sequence<I...>, MemoryInfosType& infos )
        {
            ( ( mComponents[I] = reinterpret_cast<u8*>( mChunk->GetComponents<T>( infos[I] ) ) ), ... );
        }

      public:
        template <IsComponentConcept C>
        ComponentIterator<C> ComponentIter()
        {
            constexpr usize index = TupleIndex<ComponentTypes, C>();
            return ComponentIter<C>( mChunk, mInfos[index] );
        }

        /**
         * @brief 获得一个子集
         */
        template <typename... C>
            requires IsComponentSetConcept<C...> && IsSubsetOfTuple<ComponentTypes, std::tuple<C...>>
        ComponentSetIterator<C...> SubComponentSetIter()
        {
            constexpr std::vector<usize>& ComponentTypes =
                TupleSubsetIndex<ComponentSetIterator::ComponentTypes, std::tuple<C...>>;
            MemoryInfosType infos;
            for ( i32 i = 0; i < ComponentTypes.size(); ++i )
            {
                auto index = ComponentTypes[i];
                infos[i] = mInfos[index];
            }
            return ComponentSetIterator<C...>( mChunk, std::move( infos ) );
        }

        std::tuple<T&...> GetComponents()
        {
            NY_ASSERT( mOffset >= 0 && mOffset < mChunk->mEntities.size() )
            return GetComponentImpl( std::make_index_sequence<ComponentNum>() );
        }

        std::tuple<T&...> GetComponents() const
        {
            NY_ASSERT( mOffset >= 0 && mOffset < mChunk->mEntities.size() )
            return GetComponentImpl( std::make_index_sequence<ComponentNum>() );
        }

      protected:
        template <usize... I>
        std::tuple<T&...> GetComponentImpl( std::index_sequence<I...> ) const
        {
            return std::forward_as_tuple( ( ( *( reinterpret_cast<T*>( mComponents[I] ) + mOffset ) ), ... ) );
        }

      public:
        std::tuple<T&...> operator*()
        {
            return GetComponents();
        }

        // ++a
        ComponentSetIterator<T...>& operator++()
        {
            mOffset++;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
            return *this;
        }

        // a++
        ComponentSetIterator<T...> operator++( i32 )
        {
            ComponentSetIterator<T...> old = *this;
            mOffset++;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
            return old;
        }

        // --a
        ComponentSetIterator<T...>& operator--()
        {
            mOffset--;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
            return *this;
        }

        // a--
        ComponentSetIterator<T...>& operator--( i32 )
        {
            auto old = *this;
            mOffset--;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
            return old;
        }

        void operator+=( usize size )
        {
            mOffset += size;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        void operator-=( usize size )
        {
            mOffset -= size;
            NY_ASSERT( mOffset >= 0 && mOffset <= mChunk->mEntities.size() )
        }

        bool operator==( const ComponentSetIterator& other ) const
        {
            return mOffset == other.mOffset && mChunk == other.mChunk && mComponents == other.mComponents &&
                   mInfos == other.mInfos;
        }

        bool IsEnd() const
        {
            return mOffset >= mChunk->mEntities.size();
        }

        bool IsEnd()
        {
            return mOffset >= mChunk->mEntities.size();
        }

      protected:
        ArchetypeChunk* mChunk = nullptr;
        std::array<u8*, ComponentNum> mComponents;
        MemoryInfosType mInfos;
        usize mOffset = 0;
    };

  public:
    template <typename... T>
        requires IsComponentSetConcept<T...>
    ComponentSetIterator<T...> ComponentSetIter( const std::vector<ArchetypeComponentMemoryInfo>& infos )
    {
        typename ComponentSetIterator<T...>::MemoryInfosType newInfos;
        for ( auto&& [i, info] : infos | ranges::views::enumerate )
        {
            newInfos[i] = info;
        }
        return ComponentSetIterator<T...>( this, std::move( newInfos ) );
    }

  protected:
    // TODO: 之后使用运行时配置
    // alignas( 64 )
    u8* mArchetypeComponentData = nullptr;

    // 相同架构的所有实体
    std::vector<Entity> mEntities;
    usize MaxEntityNum = 0;
};
