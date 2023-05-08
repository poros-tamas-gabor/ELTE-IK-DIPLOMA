#include "Mocks.h"
#include "pch.h"

TEST(ModelMessageSystemTest, Test_SubscribeUnsubscribe)
{
    ModelMessageSystem system;

    auto subscriber = std::make_shared<MockIModelSubscriber>();
    EXPECT_TRUE(system.Subscribe(subscriber));
    EXPECT_FALSE(system.Subscribe(subscriber)); // Should not be able to subscribe twice

    EXPECT_TRUE(system.Unsubscribe(subscriber));
    EXPECT_FALSE(system.Unsubscribe(subscriber)); // Should not be able to unsubscribe twice
}

TEST(ModelMessageSystemTest, Test_PublishModelState)
{
    ModelMessageSystem  system;
    MeshGroupState      meshGroupState;
    FlythroughState     flythroughState;
    Explore3DState      explore3DState;
    GeneralModelState   generalModelState;

   // using SubscriberType = ::testing::NiceMock<MockIModelSubscriber>;
    auto subscriber = std::make_shared<MockIModelSubscriber>();

    EXPECT_CALL(*subscriber, HandleIModelState(testing::Matcher<const MeshGroupState&>(testing::_))).Times(1);
    EXPECT_CALL(*subscriber, HandleIModelState(testing::Matcher<const FlythroughState&>(testing::_))).Times(1);
    EXPECT_CALL(*subscriber, HandleIModelState(testing::Matcher<const Explore3DState&>(testing::_))).Times(1);
    EXPECT_CALL(*subscriber, HandleIModelState(testing::Matcher<const GeneralModelState&>(testing::_))).Times(1);

    system.Subscribe(subscriber);

    system.PublishModelState(meshGroupState);

    system.PublishModelState(flythroughState);

    system.PublishModelState(explore3DState);

    system.PublishModelState(generalModelState);
}