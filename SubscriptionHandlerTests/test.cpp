#include "pch.h"
#include "../chm/SubscriptionHandler.h"


struct FindFreeSpaceTest : testing::Test
{
	SubscriptionHandler subscription_handler;
	int index;
	FindFreeSpaceTest()
	{
		for (int i = 0; i < N_MEMBER; i++) {
			subscription_handler.SubscribeUser(i);
		}
	}

};

//
///***** FindFreeSpace  ******/
TEST(FindFreeSpace, FindSpaceInEmptySys) {                 // test if the handler successfully finds free space in empty array
	assert(N_MEMBER > 0);
	int index;
	SubscriptionHandler subscription_handler;
	EXPECT_EQ(subscription_handler.FindFreeSpace(0,&index), 1);
	EXPECT_EQ(index, 0);
}

TEST(FindFreeSpace, FindSpaceWithOneSubscribed) {          // test if the handler finds free space in array with one subscriber
	assert(N_MEMBER > 1);
	int index;
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	EXPECT_EQ(subscription_handler.FindFreeSpace(1, &index), 1);
	EXPECT_EQ(index, 1);
}

TEST_F(FindFreeSpaceTest, FindSpaceWithFullArray) {             // test if the handler finds free space in  array with no free cells
	assert(N_MEMBER > 1);
	EXPECT_EQ(subscription_handler.FindFreeSpace(N_MEMBER, &index), -1);
}

TEST_F(FindFreeSpaceTest, FindSpaceWithNearFullArray) {         // test if the handler finds free space in  array which has only one empty cell : index=2
	assert(N_MEMBER > 1);
	subscription_handler.UnSubscribeUser(1);

	EXPECT_EQ(subscription_handler.FindFreeSpace(N_MEMBER, &index), 1);
	EXPECT_EQ(index, 1);
}

TEST_F(FindFreeSpaceTest, FindSpaceWithTwoEmptySpaces) {        // test if the handler finds free space in  array which has two empty cells : index=0 and index=1
	assert(N_MEMBER > 1);

	subscription_handler.UnSubscribeUser(0);
	subscription_handler.UnSubscribeUser(1);
	EXPECT_EQ(subscription_handler.FindFreeSpace(N_MEMBER, &index), 1);
	EXPECT_EQ(index, 0);
}

TEST_F(FindFreeSpaceTest, FindSpaceWithEmptySpaceAndAllUnResponsive) {     // test if the handler finds free space in  array which has one empty cell : index=1 and 
	assert(N_MEMBER > 1);                                            //all subscribed are unresponsive

	subscription_handler.UnSubscribeUser(1);
	subscription_handler.LogUnResponsiveUsersAndReset();
	EXPECT_EQ(subscription_handler.FindFreeSpace(N_MEMBER, &index), 1);
	EXPECT_EQ(index, 1);
}

TEST_F(FindFreeSpaceTest, FindSpaceWithFirstUnResponsiveAndEmpySpace) {    // test if the handler finds free space in  array which has one empty cell : index=1 and 
	assert(N_MEMBER > 1);                                            //all subscribers are responsive except user with sys_id=0
	subscription_handler.LogUnResponsiveUsersAndReset();

	for (int i = 1; i < N_MEMBER; i++) {
		subscription_handler.UpdateAliveStatus(i);
	}
	subscription_handler.UnSubscribeUser(1);
	EXPECT_EQ(subscription_handler.FindFreeSpace(N_MEMBER, &index), 1);
	EXPECT_EQ(index, 1);

}


///*********************************************/

/***** SubscribeUser  ******/
TEST(SubscribeUser, SubscribeUser) {                                 // test if the handler successfully subscribes a user with id=0 as first user
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	EXPECT_EQ(subscription_handler.SubscribeUser(0), 0);
}

TEST(SubscribeUser, NegativeId) {                                   // test if the handler fails to subscribe a user with negative id
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	EXPECT_EQ(subscription_handler.SubscribeUser(-1), -1);
}

TEST(SubscribeUser, SubscribeUserTwice) {                           // test if the handler fails when trying to subscribe same user twice
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	EXPECT_EQ(subscription_handler.SubscribeUser(0), -1);
}

TEST(SubscribeUser, SubscribeUserSystemFull) {                      // test if the handler fails when trying to subscribe new user when the system is full
	SubscriptionHandler subscription_handler;
	for (int i = 0; i < N_MEMBER; i++) {
		subscription_handler.SubscribeUser(i);
	}
	EXPECT_EQ(subscription_handler.SubscribeUser(N_MEMBER + 1), -1);
}

TEST(SubscribeUser, SubscribeUnsubribedUser) {                       // test if the handler succeeds in subscribing an unsubscribed user
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	subscription_handler.UnSubscribeUser(0);
	EXPECT_EQ(subscription_handler.SubscribeUser(0), 0);
}


TEST(SubscribeUser, SubscribeInIndex3) {                            // test if the handler succeeds in subscribing new user with index=1 cell free
	assert(N_MEMBER > 1);
	SubscriptionHandler subscription_handler;
	for (int i = 0; i < N_MEMBER; i++) {
		subscription_handler.SubscribeUser(i);
	}
	subscription_handler.UnSubscribeUser(1);
	EXPECT_EQ(subscription_handler.SubscribeUser(N_MEMBER + 1), 1);
}

TEST(SubscribeUser, SubscribeInIndex1) {                            // test if the handler succeeds in subscribing new user with index=3 and index=1 cells free
	assert(N_MEMBER > 1);
	SubscriptionHandler subscription_handler;
	for (int i = 0; i < N_MEMBER; i++) {
		subscription_handler.SubscribeUser(i);
	}
	subscription_handler.UnSubscribeUser(0);
	subscription_handler.UnSubscribeUser(1);
	EXPECT_EQ(subscription_handler.SubscribeUser(N_MEMBER + 1), 0);
}

TEST(SubscribeUser, SubscribeUnresponsiveUser) {                   // test if the handler fails at subscribing a user who is already subscribed but unresponsive
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	subscription_handler.LogUnResponsiveUsersAndReset();
	EXPECT_EQ(subscription_handler.SubscribeUser(0), -1);
}

/*********************************************/

/***** UnSubscribeUser  ******/

TEST(UnSubscribeUser, UnsubscribeNonExistantUser) {                // test if the handler fails at unsubscribing a non-existant user
	SubscriptionHandler subscription_handler;
	EXPECT_FALSE(subscription_handler.UnSubscribeUser(0));
}

TEST(UnSubscribeUser, UnsubscribeExistingUser) {                   // test if the handler succeeds at subscribing a new user
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	EXPECT_TRUE(subscription_handler.UnSubscribeUser(0));
}

TEST(UnSubscribeUser, UnsubscribeUserNegativeSysID) {              // test if the handler fails at unsubscribing a user with negative system id
	SubscriptionHandler subscription_handler;
	EXPECT_FALSE(subscription_handler.UnSubscribeUser(-1));
}

TEST(UnSubscribeUser, UnsubscribeUserTwice) {                      // test if the handler fails at unsubscribing a user twice
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	subscription_handler.UnSubscribeUser(0);
	EXPECT_FALSE(subscription_handler.UnSubscribeUser(0));
}

TEST(UnSubscribeUser, UnsubscribeUnresponsiveUser) {              // test if the handler succeeds in unsubscribing unresponsive user
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(0);
	subscription_handler.LogUnResponsiveUsersAndReset();
	EXPECT_TRUE(subscription_handler.UnSubscribeUser(0));
}

/*********************************************/

/***** AreAllMembersResponsive ******/
TEST(AreAllMembersResponsive, CheckResponsivnessNoMembers) {                          // test if the handler returns true when checking responsiveness of system with no users
	SubscriptionHandler subscription_handler;
	EXPECT_TRUE(subscription_handler.AreAllMembersResponsive());
}

TEST(AreAllMembersResponsive, CheckResponsivnessOneResponsiveMember) {               // test if the handler returns true when checking responsiveness of system with one user who just subscribed
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(1);
	EXPECT_TRUE(subscription_handler.AreAllMembersResponsive());
}

TEST(AreAllMembersResponsive, CheckResponsivnessOneUnResponsiveMember) {             // test if the handler returns false when checking responsiveness of system with one unresponsive user
	assert(N_MEMBER > 0);
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(1);
	subscription_handler.LogUnResponsiveUsersAndReset();
	EXPECT_FALSE(subscription_handler.AreAllMembersResponsive());
}

TEST(AreAllMembersResponsive, CheckResponsivnessAfterResetAndAlive) {                // test if the handler returns true when checking responsivness of system with one user after 
	assert(N_MEMBER > 0);                                                            // reset and updated alive 
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(1);
	subscription_handler.LogUnResponsiveUsersAndReset();
	subscription_handler.UpdateAliveStatus(0);
	EXPECT_TRUE(subscription_handler.AreAllMembersResponsive());
}

TEST(AreAllMembersResponsive, CheckResponsivnessAfterResetAndOneAlive) {             // test if the handler returns false when checking responsivness of system with two users after reset and only 
	assert(N_MEMBER > 1);                                                            // one updated alive
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(1);
	subscription_handler.SubscribeUser(2);
	subscription_handler.LogUnResponsiveUsersAndReset();
	subscription_handler.UpdateAliveStatus(0);
	EXPECT_FALSE(subscription_handler.AreAllMembersResponsive());
}

TEST(AreAllMembersResponsive, CheckResponsivnessAfterResetOneAliveAndUnSubscribe) {  // test if the handler returns true when checking responsivness of system with two users after reset , only 
	assert(N_MEMBER > 1);                                                             // one updated alive and one unregistered user
	SubscriptionHandler subscription_handler;
	subscription_handler.SubscribeUser(1);
	subscription_handler.SubscribeUser(2);
	subscription_handler.LogUnResponsiveUsersAndReset();
	subscription_handler.UpdateAliveStatus(0);
	subscription_handler.UnSubscribeUser(1);
	EXPECT_TRUE(subscription_handler.AreAllMembersResponsive());
}

/*********************************************/
