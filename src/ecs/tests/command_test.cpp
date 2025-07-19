#include <gtest/gtest.h>

import ecs;
import core.type;

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

    Registry* mRegistry = nullptr;

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
    RegistryCommand::Get( mRegistry, mRegistry->GetCommandManager() )
        ->Entity()
            ->Destroy( InvalidEntity )
        ->Entity()
            ->Create()
                ->AddComponent(std::move(k) );
    // clang-format on
}

TEST_F( CommandTest, CommandsParam )
{
    ComponentC k = ComponentC();
    k.a = 0;

    Registry registry;
    CommandsState state;
    auto commands = Commands::From( &registry, state );

    // clang-format off
    commands()
        ->Entity()
            ->Destroy( InvalidEntity )
        ->Entity()
            ->Create()
                ->AddComponent(std::move(k) );
    // clang-format on
}