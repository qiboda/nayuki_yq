#pragma once

#include "core/macro/macro.h"
#include "ecs/registry.h"
#include "ecs/systems/params/query/query_data.h"
#include "ecs/systems/system_param.h"
#include "ecs/systems/params/query/query_param_resolver.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

template <typename... T>
    requires QueryParamResolverConcept<T...>
class QueryState : public SystemParamState
{
  public:
    void Init( Registry* registry )
    {
        mRegistry = registry;
        using DataDecayedType = QueryParamResolver<T...>::QueryDataSetType;
        using FilterSetType = QueryParamResolver<T...>::QueryFilterSetType;
        archetypeIndices = registry->mArchetypeManager->FindMetArchetypeIndices<DataDecayedType, FilterSetType>();
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
                mArchetype = mState.mRegistry->mArchetypeManager->GetArchetype( mState.archetypeIndices[0] );
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
                            mState.mRegistry->mArchetypeManager->GetArchetype( mState.archetypeIndices[mIndex] );
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

  protected:
    std::vector<usize> archetypeIndices;
    Registry* mRegistry = nullptr;
};

template <typename... T>
    requires QueryParamResolverConcept<T...>
class Query : public SystemParam
{
  public:
    using State = QueryState<T...>;
    using Trait = SystemParamTrait<Query<T...>>;

  public:
    Query( State& state )
        : mState( state )
    {
    }

    static Query<T...> From( Registry* registry, State& state )
    {
        UNUSED_VAR( registry );
        return Query<T...>( state );
    }

  public:
    void foreach ( std::function<void( T... )> callback )
    {
        UNUSED_VAR( callback );
    }

    State::Iterator begin()
    {
        return mState.begin();
    }

    State::Iterator end()
    {
        return mState.end();
    }

  protected:
    State& mState;
};

template <typename... T>
    requires QueryParamResolverConcept<T...>
struct SystemParamTrait<Query<T...>>
{
    using Type = Query<T...>;

    using QueryParamResolverType = QueryParamResolver<T...>;

    using QueryDataTypeTuple = QueryParamResolverType::QueryDataTypes;
    using QueryDataDecayedTypeTuple = QueryParamResolverType::QueryDataDecayedTypes;

    using QueryFilterTypeTuple = QueryParamResolverType::QueryFilterTypes;

#pragma region ReadWrite

    template <typename TTuple>
    struct ParamReadonly;

    template <typename... P>
    struct ParamReadonly<std::tuple<P...>>
    {
        template <usize... I>
        static consteval std::vector<bool> GetParamReadonly( std::index_sequence<I...> )
        {
            std::vector<bool> result( sizeof...( P ) );
            ( ( result[I] = IsReadOnlyQueryData<std::decay_t<T>> ), ... );
            return result;
        }
    };

    template <typename TTuple>
    struct ParamReadWrite;

    template <typename... P>
    struct ParamReadWrite<std::tuple<P...>>
    {
        template <usize... I>
        static consteval std::vector<bool> GetParamReadWrite( std::index_sequence<I...> )
        {
            std::vector<bool> result( sizeof...( P ) );
            ( ( result[I] = IsReadWriteQueryData<std::decay_t<T>> ), ... );
            return result;
        }
    };

    static constexpr std::vector<bool> Readonly = ParamReadonly<QueryDataTypeTuple>::GetParamReadonly(
        std::make_index_sequence<std::tuple_size_v<QueryDataTypeTuple>>() );

    static constexpr std::vector<bool> ReadWrite = ParamReadWrite<QueryDataTypeTuple>::GetParamReadWrite(
        std::make_index_sequence<std::tuple_size_v<QueryDataTypeTuple>>() );

#pragma endregion
};
