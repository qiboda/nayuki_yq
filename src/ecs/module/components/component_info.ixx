module;

#include "module_export.h"
#include <core/macro/macro.h>

export module ecs.components.info;

// import core.misc;
import core;
import core.misc;
import core.registry;
import ecs.components.concepts;
import ecs.components.component;
import std;

export struct ECS_API ComponentId : public Id
{
};

export ECS_API inline bool operator==( const ComponentId& lhs, const ComponentId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export ECS_API inline auto operator<=>( const ComponentId& lhs, const ComponentId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

template <>
struct std::hash<ComponentId>
{
    usize operator()( const ComponentId& componentId ) const
    {
        return std::hash<u32>()( componentId.Index() );
    }
};

export struct ECS_API ComponentIdSet
{

  private:
    // 需要保持有序, 从小到大
    // TODO: 使用 SmallVector
    std::vector<ComponentId> ids;

  public:
    constexpr ComponentIdSet()
        : ids()
    {
    }

    constexpr explicit ComponentIdSet( std::vector<ComponentId>&& otherIds )
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

    // 包含关系
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

    // 互斥关系, 完全不包含
    bool Exclude( const ComponentIdSet& other ) const
    {
        for ( ComponentId id : other.ids )
        {
            if ( Contains( id ) )
            {
                return false;
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

    auto begin() const
    {
        return ids.begin();
    }

    auto end() const
    {
        return ids.end();
    }

    auto cbegin() const
    {
        return ids.cbegin();
    }

    auto cend() const
    {
        return ids.cend();
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

    bool Contains( ComponentId id ) const
    {
        auto it = std::lower_bound( ids.begin(), ids.end(), id );
        if ( it != ids.end() && *it == id )
        {
            return true;
        }
        return false;
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
                              {
                                  return std::ranges::find( rhsIds, needle ) == rhsIds.end();
                              } );
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
                              {
                                  return std::ranges::find( rhsIds, needle ) != rhsIds.end();
                              } );
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

export struct ECS_API ComponentInfo
{
    ComponentId id;
    usize size;
    usize align;
};

// 用于获取组件id
// 使用静态变量缓存，接口易用，性能更高。
export class ECS_API ComponentTypeRegistry : public IdRegistry<ComponentId, Component>
{
  public:
    template <IsComponentConcept T>
    static void Register()
    {
        Get<T>();
        GetComponentInfo<T>();
    }

    template <IsComponentConcept... T>
    constexpr static ComponentIdSet GetComponentIdSet()
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
    static inline std::unordered_map<ComponentId, ComponentInfo> sComponentInfoMap;
};