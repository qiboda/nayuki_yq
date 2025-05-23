
#include "ecs/registry.h"
#include "ecs/entity/entity_manager.h"
#include "ecs/commands/command_manager.h"

Registry::Registry()
    : mEntityManager( new EntityManager() )
    , mCommandManager( new CommandManager() )
{
}

// void Registry::AddComponent( Entity entity, Component *component )
// {
//     UNUSED_VAR( entity );
//     UNUSED_VAR( component );
//     // 根据实体找到archetype。添加类型信息。
//     // 如果没有找到，创建一个新的archetype

//     // 然后找到数据集，添加数据到数据集中
//     // 如果没有找到，创建一个新的数据集
// }

// void Registry::RemoveComponent( Entity entity, Component *component )
// {
//     UNUSED_VAR( entity );
//     UNUSED_VAR( component );
// }
