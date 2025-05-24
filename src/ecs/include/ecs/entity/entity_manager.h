#pragma once

#include "core/container/sparse_array.h"
#include "core/misc/non_copyable.h"
#include "ecs/entity/entity.h"
#include <core/minimal.h>
#include <ecs/minimal.h>
#include <oneapi/tbb/concurrent_hash_map.h>
#include <vector>

class ECS_API EntityManager : public NonCopyable
{
    using EntityIdType = typename Entity::IdType;
    using EntityVersionType = typename Entity::VersionType;

  public:
    EntityManager()
    {
    }

    // 申请entity
    // 申请的entity是未激活的
    // 最终在主线程中激活
    // 从未激活的entity中申请一个新的entity, version + 1

  public:
    // 可以从不同线程执行，但同时只能由一个线程执行
    Entity RequestCreateEntity_AnyThread()
    {
        if ( mUnactiveEntities.empty() )
        {
            usize capacity = mActiveEntities.Capacity();
            Entity entity = Entity( static_cast<Entity::IdType>(capacity), 0 );
            return entity;
        }
        else
        {
            Entity entity = mUnactiveEntities.back();
            mUnactiveEntities.pop_back();
            entity.UpgradeVersion();
            return entity;
        }
    }

  public:
    // 只能从主线程执行
    void AddEntity( Entity entity )
    {
        mActiveEntities.Add( entity.GetId(), entity );
    }

    void RemoveEntity( Entity entity )
    {
        auto ptr = mActiveEntities.Remove( entity.GetId() );
        if ( ptr )
        {
            mUnactiveEntities.emplace_back( *ptr );
        }
    }

  protected:
    SparseArray<EntityIdType, Entity> mActiveEntities;

    std::vector<Entity> mUnactiveEntities;
};
