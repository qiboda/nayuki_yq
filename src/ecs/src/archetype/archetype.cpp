
#include "ecs/archetype/archetype.h"

Archetype::Archetype()
{
}

void Archetype::MoveEntity( Entity entity, Archetype *destArchetype, ArchetypeChunk *destArchetypeChunk )
{
    ArchetypeChunk *curArchetypeChunk = GetEntityArchetypeChunk( entity );
    if ( curArchetypeChunk )
    {
        std::vector<u8 *> moveOuttedComponentsData = curArchetypeChunk->MoveOutEntity( entity, mComponentMemoryInfo );

        // 获得 公有的组件 id
        std::vector<ComponentId> unionComponentIds =
            ComponentIdSet::Union( destArchetype->mComponentIdSet, mComponentIdSet );

        for ( auto &componentId : unionComponentIds )
        {
            usize srcIndex = mComponentIdSet.IndexOf( componentId ).value();
            usize destIndex = destArchetype->mComponentIdSet.IndexOf( componentId ).value();

            destArchetypeChunk->MoveIntoEntityComponent( entity,
                                                         moveOuttedComponentsData,
                                                         srcIndex,
                                                         destIndex,
                                                         mComponentMemoryInfo );
        }

        // 如果src的component更多，那么需要把那些数据析构掉。
        std::vector<ComponentId> toDeconstrutor =
            ComponentIdSet::Diff( mComponentIdSet, ComponentIdSet( std::move( unionComponentIds ) ) );
        for ( auto &&componentId : toDeconstrutor )
        {
            usize srcIndex = mComponentIdSet.IndexOf( componentId ).value();

            u8 *componentData = moveOuttedComponentsData[srcIndex];
            auto *component = reinterpret_cast<Component *>( componentData );
            component->~Component();
        }
    }
}
