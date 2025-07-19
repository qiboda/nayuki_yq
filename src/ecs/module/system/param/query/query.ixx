module;

#include "core/macro/macro.h"

export module ecs:system_param_query;

import :system_param_query_data;
import :system_param;
import :system_param_query_param_resolver;
import :component_info;
import :archetype;
import :registry;

import std;
import core.type;

export template <typename... T>
    requires QueryParamResolverConcept<T...>
class QueryState : public SystemParamState
{
  public:
    void Init( Registry* registry )
    {
        mRegistry = registry;
        using DataDecayedType = QueryParamResolver<T...>::QueryDataSetType;
        using FilterSetType = QueryParamResolver<T...>::QueryFilterSetType;
        archetypeIndices = registry->GetArchetypeManager()->FindMetArchetypeIndices<DataDecayedType, FilterSetType>();
    }

    class Iterator
    {
      public:
        Iterator( QueryState& state )
            : mState( state )
            , mIndex( 0 )
        {
        }

        Iterator( const Iterator& iterator )
            : mState( iterator.mState )
            , mIndex( iterator.mIndex )
            , mArchetype( iterator.mArchetype )
            , mComponentSetIterator( iterator.mComponentSetIterator )
        {
        }

        Iterator& begin()
        {
            if ( mState.archetypeIndices.size() > 0 )
            {
                mIndex = 0;
                mArchetype = mState.mRegistry->GetArchetypeManager()->GetArchetype( mState.archetypeIndices[0] );
                mComponentSetIterator = mArchetype->ComponentSetIter<std::decay_t<T>...>();
                mComponentSetIterator->begin();
            }
            else
            {
                end();
            }
            return *this;
        }

        Iterator& end()
        {
            mArchetype = nullptr;
            mComponentSetIterator = std::nullopt;
            mIndex = mState.archetypeIndices.size();
            return *this;
        }

        Iterator& operator++()
        {
            if ( mComponentSetIterator.has_value() )
            {
                auto iter = mComponentSetIterator.value();
                iter++;
                while ( iter.IsEnd() )
                {
                    mIndex++;
                    if ( mIndex < mState.archetypeIndices.size() )
                    {
                        mArchetype =
                            mState.mRegistry->GetArchetypeManager()->GetArchetype( mState.archetypeIndices[mIndex] );
                        mComponentSetIterator = mArchetype->ComponentSetIter<std::decay_t<T>...>();
                    }
                    else
                    {
                        end();
                        break;
                    }
                }
            }
            return *this;
        }

        bool operator==( const Iterator& other ) const
        {
            return mArchetype == other.mArchetype && mIndex == other.mIndex &&
                   mComponentSetIterator == other.mComponentSetIterator;
        }

        std::tuple<T...> operator*() const
        {
            return *mComponentSetIterator.value();
        }

      protected:
        QueryState<T...>& mState;
        usize mIndex = 0;

        Archetype* mArchetype = nullptr;
        std::optional<Archetype::ComponentSetIterator<std::decay_t<T>...>> mComponentSetIterator;
    };

    Iterator begin()
    {
        return Iterator( *this ).begin();
    }

    Iterator end()
    {
        return Iterator( *this ).end();
    }

    usize hint_size() const
    {
        usize totalSize = 0;
        for ( auto archetypeIndex : archetypeIndices )
        {
            auto mArchetype = mRegistry->GetArchetypeManager()->GetArchetype( archetypeIndex );
            totalSize += mArchetype->GetTotalEntityNum();
        }
        return totalSize;
    }

  protected:
    std::vector<usize> archetypeIndices;
    Registry* mRegistry = nullptr;
};

export template <typename... T>
    requires QueryParamResolverConcept<T...>
class Query : public SystemParam
{
  public:
    using State = QueryState<T...>;

  public:
    Query( State* state )
        : mState( state )
    {
    }

    static Query<T...> From( Registry* registry, State* state )
    {
        UNUSED_VARS( registry );
        return Query<T...>( state );
    }

  public:
    void foreach ( std::function<void( T... )> callback )
    {
        UNUSED_VARS( callback );
    }

    State::Iterator begin()
    {
        return mState->begin();
    }

    State::Iterator end()
    {
        return mState->end();
    }

    /**
     * @brief
     * 预估的数量。(在没有关于值，仅仅有关于结构的查询时，总是准确的。）
     */
    usize hint_size() const
    {
        return mState->hint_size();
    }

  protected:
    State* mState = nullptr;
};

export template <typename... T>
    requires QueryParamResolverConcept<T...>
struct SystemParamTrait<Query<T...>>
{
    using Type = Query<T...>;

    using QueryParamResolverType = QueryParamResolver<T...>;

    using QueryDataTypeTuple = QueryParamResolverType::QueryDataTypes;
    using QueryDataNonEntityTypeTuple = QueryParamResolverType::QueryDataNonEntityTypes;
    using QueryDataDecayedTypeTuple = QueryParamResolverType::QueryDataDecayedTypes;

    using QueryFilterTypeTuple = QueryParamResolverType::QueryFilterTypes;

#pragma region ReadWrite

    constexpr static inline usize NonEntityDataTypeCount = std::tuple_size_v<QueryDataNonEntityTypeTuple>;
    using ComponentsReadWriteType = std::array<std::pair<ComponentId, bool>, NonEntityDataTypeCount>;

    template <typename TTuple>
    struct ParamWritable;

    template <typename... P>
    struct ParamWritable<std::tuple<P...>>
    {
        using ParamType = std::tuple<P...>;

        static ComponentsReadWriteType GetParamReadWrite()
        {
            return { std::make_pair( ComponentTypeRegistry::Get<std::decay_t<P>>(), IsWritableQueryData<P> )... };
        }
    };

    // true 是可写的，false 是只读的
    static inline const ComponentsReadWriteType ComponentsReadWrite =
        ParamWritable<QueryDataNonEntityTypeTuple>::GetParamReadWrite();

#pragma endregion
};
