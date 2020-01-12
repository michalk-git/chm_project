#include "Command.h"

void UnSubscribe::ExecuteCmd(int index)const {
	UnSubscribeUserEvt* user_evt = Q_NEW(UnSubscribeUserEvt, MEMBER_UNSUBSCRIBE_SIG);   
	user_evt->sender_id = index;
	AO_Member[index]->POST(user_evt, this);
}

void Subscribe::ExecuteCmd(int index)const{
	//create an event with the new user's id and post it to the appropriate Member Ao
	SubscribeUserEvt* user_evt = Q_NEW(SubscribeUserEvt, MEMBER_SUBSCRIBE_SIG);
	user_evt->id = user_id;
	user_evt->sender_id = index;
	AO_Member[index]->POST(user_evt, this);
}

void Deactivate::ExecuteCmd(int index)const{
	// create new event to notify the appropriate member and post it
	DeactivationEvt* evt = Q_NEW(DeactivationEvt, DEACTIVATE_SIG);
	evt->period_num = cycle_num;
	AO_Member[index]->POST(evt, this);

}