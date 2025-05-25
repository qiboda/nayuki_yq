#include <gtest/gtest.h>

#include <ecs/commands/entity_command.h>

#include <ecs/commands/command_manager.h>
#include <ecs/commands/registry_command.h>
#include <ecs/commands/command.h>
#include <ecs/commands/command_queue.h>
#include <ecs/commands/entity_instance_command.h>
#include <ecs/registry.h>
#include <ecs/entity/entity_manager.h>
#include <ecs/entity/entity.h>
#include <ecs/components/component.h>

class CommandTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        mRegistry = new Registry();
    }

    void TearDown() override
    {
        delete mRegistry;
    }

    Registry *mRegistry = nullptr;

    struct ComponentC : public Component
    {
        i32 a = 0;
    };
};

TEST_F( CommandTest, CreateEntity )
{
    ComponentC k = ComponentC();
    k.a = 0;

    // clang-format off
    RegistryCommand::Get( mRegistry )
        ->Entity()
            ->Destroy( InvalidEntity )
        ->Entity()
            ->Create()
                ->AddComponent(std::move(k) );
    // clang-format on
}