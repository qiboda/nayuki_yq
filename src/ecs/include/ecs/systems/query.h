#pragma once

#include "core/macro/macro.h"
#include "ecs/registry.h"
#include "ecs/systems/query_param.h"
#include "ecs/systems/system_param.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <functional>
#include <vector>

template <typename... T>
    requires IsQueryArgsSetConcept<T...>
class QueryState : public SystemParamState
{
  public:
    void Init( Registry* registry )
    {
        mRegistry = registry;
        archetypeIndices = registry->mArchetypeManager->FindMetArchetypeIndices<std::decay_t<T>...>();
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
    requires IsQueryArgsSetConcept<T...>
class Query : public SystemParam
{
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
    requires IsQueryArgsSetConcept<T...>
struct SystemParamTrait<Query<T...>>
{
    using Type = Query<T...>;

    using QueryArgTypesTuple = std::tuple<T...>;
    using QueryParamTypesTuple = std::tuple<std::decay_t<T>...>;

    static constexpr std::vector<bool> readonly = ParamReadonly( std::make_index_sequence<sizeof...( T )>() );

    template <usize... I>
    consteval std::vector<bool> ParamReadonly( std::index_sequence<I...> )
    {
        std::vector<bool> result( sizeof...( T ) );
        ( ( result[I] = IsReadOnlyQueryParam<T> ), ... );
        return result;
    }
};
