#pragma once
#include "system.h"

using namespace std;
#define INVALID_INPUT (-1)
#define SYSTEM_FULL (-1)
#define INVALID_ID (-1)
#define SUCCESS 1
#define USER_IN_SYS (-2)

class SubscriptionHandler {
	User  members[N_MEMBER];
	int members_num;


	
	// Find the index of the first empty cell encountered in members array unless:
	// 1. the members array is full. In that case the return value will be SYSTEM_FULL.
	// 2. the id (for which we need to search for an empty space) is already in the system. In that case will return USER_IN_SYS.
	// id - the id of the user we wish to subscribe to the system
	// return_index - the index of the first free cell encountered in the members array
	int FindFreeSpace(int id, int* return_index)const;

	// Check if 'index' is the correct index range of members array. If it is, retuen True. If not, False.
	bool IndexInRange(int index)const;

public:
	SubscriptionHandler() { members_num = 0; };

	// Check if the id = 'user_id' is new to the system. If so, check if there is free space in the system.
	// If the id is new to the system and there is free space, registers the new user and returns the index of the members array associated to the new user.
	// If the user is already in the system or the system is full, returns -1
	int SubscribeUser(int user_id);

	// updates the members array by removing the user associated with the system id = 'sys_id'. Will fail if:
	//1. sys_id isn't a valid index
	//2. sys_id (ie the index in members array) doesn't hold information of a subscribed user
	bool UnSubscribeUser(int sys_id);

	// check if the subscribers were responsive in the past cycle; If not, print to error log. In any case, reset the ALIVE signals sent to the system for next cycle
	void LogUnResponsiveUsersAndReset();

	// check if all the users were responsive during the past cycle; If so, return True
	bool AreAllMembersResponsive()const;

	// sets the 'keep_alive_received' parameter to true for the user associated with system id = 'sys_id'
	void UpdateAliveStatus(int sys_id);

	// returns the number of subscribers 
	int GetMembersNum()const;

};


