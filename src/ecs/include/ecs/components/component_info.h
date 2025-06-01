#pragma once

#include <core/core.h>
#include <ecs/ecs.h>

#include "ecs/components/component_concept.h"

class ComponentTypeRegistry;

struct ECS_API ComponentId
{
    friend class ComponentTypeRegistry;

  public:
    ComponentId()
        : id( std::numeric_limits<u32>::max() )
    {
    }

    ComponentId( u32 id )
        : id( id )
    {
    }

  public:
    usize Index() const
    {
        return id;
    }

    // 会隐式生成 != 比较操作。
    bool operator==( const ComponentId& other ) const = default;

    bool operator<( const ComponentId& other ) const
    {
        return id < other.id;
    }

  private:
    u32 id = std::numeric_limits<u32>::max();

  private:
    u32 GenNext()
    {
        return id++;
    }
};

template <>
struct std::hash<ComponentId>
{
    usize operator()( const ComponentId& componentId ) const
    {
        return std::hash<usize>()( componentId.Index() );
    }
};

struct ECS_API ComponentIdSet
{

  private:
    // 需要保持有序, 从小到大
    // TODO: 使用 SmallVector
    std::vector<ComponentId> ids;

  public:
    ComponentIdSet()
        : ids()
    {
    }

    explicit ComponentIdSet( std::vector<ComponentId>&& otherIds )
        : ids( otherIds )
    {
    }

  public:
    void Add( ComponentId id )
    {
        auto it = std::lower_bound( ids.begin(), ids.end(), id );
        NY_ASSERT( it == ids.end() || *it != id );
        ids.insert( it, id );
    }

    void Remove( ComponentId id )
    {
        auto it = std::lower_bound( ids.begin(), ids.end(), id );
        if ( it != ids.end() && *it == id )
        {
            // 保持顺序
            ids.erase( it );
        }
    }

    void Merge( const ComponentIdSet& other )
    {
        for ( auto&& id : other.ids )
        {
            Add( id );
        }
    }

    void Subtract( const ComponentIdSet& other )
    {
        for ( auto&& id : other.ids )
        {
            Remove( id );
        }
    }

    bool Include( const ComponentIdSet& other ) const
    {
        auto begin = ids.cbegin();
        for ( auto&& otherId : other.ids )
        {
            if ( begin == ids.cend() )
            {
                return false;
            }
            if ( *begin == otherId )
            {
                begin++;
            }
            else
            {
                do
                {
                    begin++;
                    if ( begin == ids.cend() )
                    {
                        return false;
                    }
                    if ( *begin == otherId )
                    {
                        begin++;
                        break;
                    }
                } while ( true );
            }
        }
        return true;
    }

    const std::vector<ComponentId>& Get() const
    {
        return ids;
    }

    auto begin()
    {
        return ids.begin();
    }

    auto end()
    {
        return ids.end();
    }

    void Reserve( usize size )
    {
        ids.reserve( size );
    }

    usize Size() const
    {
        return ids.size();
    }

  public:
    bool operator==( const ComponentIdSet& other ) const
    {
        return ids == other.ids;
    }

    std::optional<usize> IndexOf( ComponentId id ) const
    {
        auto it = std::lower_bound( ids.begin(), ids.end(), id );
        if ( it != ids.end() )
        {
            return std::distance( ids.begin(), it );
        }
        return std::nullopt;
    }

    /// lhs - rhs
    /// 在 lhs 中存在，rhs 中不存在的元素
    static std::vector<ComponentId> Diff( const ComponentIdSet& lhs, const ComponentIdSet& rhs )
    {
        std::vector<ComponentId> lhsIds = lhs.Get();
        std::vector<ComponentId> rhsIds = rhs.Get();

        std::vector<ComponentId> result;
        std::ranges::copy_if( lhsIds,
                              std::back_inserter( result ),
                              [&rhsIds]( auto& needle )
                              { return std::ranges::find( rhsIds, needle ) == rhsIds.end(); } );
        return result;
    }

    /// lhs and rhs
    /// 在 lhs 和 rhs 中均存在的元素
    static std::vector<ComponentId> Union( const ComponentIdSet& lhs, const ComponentIdSet& rhs )
    {
        std::vector<ComponentId> lhsIds = lhs.Get();
        std::vector<ComponentId> rhsIds = rhs.Get();

        std::vector<ComponentId> result;
        std::ranges::copy_if( lhsIds,
                              std::back_inserter( result ),
                              [&rhsIds]( auto& needle )
                              { return std::ranges::find( rhsIds, needle ) != rhsIds.end(); } );
        return result;
    }

    /// clone
    ComponentIdSet Clone() const
    {
        ComponentIdSet clone;
        clone.ids = ids;
        return clone;
    }
};

template <>
struct std::hash<ComponentIdSet>
{
    usize operator()( const ComponentIdSet& componentIdSet ) const
    {
        usize hash = 0;
        for ( auto&& id : componentIdSet.Get() )
        {
            hash ^= std::hash<ComponentId>()( id );
        }
        return hash;
    }
};

struct ECS_API ComponentInfo
{
    ComponentId id;
    size_t size;
    size_t align;
};

// 用于获取组件id
// 使用静态变量缓存，接口易用，性能更高。
class ECS_API ComponentTypeRegistry
{
  public:
    template <IsComponentConcept T>
    static void Register()
    {
        Get<T>();
        GetComponentInfo<T>();
    }

    template <IsComponentConcept T>
    static ComponentId Get()
    {
        static const ComponentId sId = ComponentId( sCounter.GenNext() );
        return sId;
    }

    template <IsComponentConcept... T>
    static ComponentIdSet GetComponentIdSet()
    {
        ComponentIdSet componentIdSet;
        componentIdSet.Reserve( sizeof...( T ) );
        ( componentIdSet.Add( Get<T>() ), ... );
        return componentIdSet;
    }

    template <IsComponentConcept T>
    static ComponentInfo GetComponentInfo()
    {
        static ComponentInfo sComponentInfo = GenComponentInfo<T>();
        return sComponentInfo;
    }

    static ComponentInfo GetComponentInfo( ComponentId id )
    {
        auto it = sComponentInfoMap.find( id );
        if ( it != sComponentInfoMap.end() )
        {
            return it->second;
        }
        return {};
    }

  private:
    template <IsComponentConcept T>
    static ComponentInfo GenComponentInfo()
    {
        ComponentInfo componentInfo;
        componentInfo.id = Get<T>();
        componentInfo.align = ComponentBaseTrait<T>::Align;
        componentInfo.size = ComponentBaseTrait<T>::Size;
        sComponentInfoMap.insert( { componentInfo.id, componentInfo } );
        return componentInfo;
    }

  private:
    static inline ComponentId sCounter = ComponentId( 0 );
    static inline std::unordered_map<ComponentId, ComponentInfo> sComponentInfoMap;
};