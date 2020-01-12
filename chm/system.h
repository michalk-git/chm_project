
#ifndef Core_Health_h
#define Core_Health_h
#include "qpcpp.h"
#include "bsp.h"
#include "singleton.h"
#include "CoreDebug.h"




#ifndef _DEBUG 
#define PRINT_LOG(x,...) 
#else
#define PRINT_LOG(x,...) do {singleton<CoreDebug>::getInstance().CorePrintf(x,##__VA_ARGS__);} while (0)
#endif

#define UNIDENTIFIED_ID (-1)




namespace Core_Health {
	QP::QTimeEvtCtr ConvertSecondsToTicks(unsigned int seconds);

	struct CHMConfig_t {
		static unsigned int T_WATCHDOG_RESET_SEC;
		static unsigned int T_AO_ALIVE_SEC;
		static unsigned int T_UPDATE_WATCHDOG_SEC;
	};

enum Core_HealthSignals {
    TIMEOUT_SIG = QP::Q_USER_SIG, // time event timeout
	REQUEST_UPDATE_SIG,           //AO_HealthMonitor publishes REQUEST_SIG to each subscribed member
	TERMINATE_SIG,                //signal that terminates the program
	TICK_SIG,                     // 
	START_TESTS_SIG,              // AO_HealthMonitor publishes 'START_TESTS_SIG' to signfy the start of the Member's tests
    MAX_PUB_SIG,                  // the last published signal
	 
	MEMBER_SUBSCRIBE_SIG,         //each member can subscribe by MEMBER_SUBSCRIBE_SIG
	MEMBER_UNSUBSCRIBE_SIG,       //each member can unsubscribe by MEMBER_UNSUBSCRIBE_SIG 
    INIT_SIG,                     // initialization signal of the Member active object
	DEACTIVATE_SIG,               //signal to an Member AO to elicit malfunctioning behaviour (no AlIVE signals for a specified amount of periods)
	SUBSCRIBE_SIG,                // signal for  AO_HealthMonitor to subscribe new user if possible
	UNSUBSCRIBE_SIG,              // signal for  AO_HealthMonitor to unsubscribe user 
	SUBSCRIBE_ACKNOWLEDGE_SIG,    // AO_HealthMonitor sends the requesting Member an acknowledgment if subscribing succeded
	UNSUBSCRIBE_ACKNOWLEDGE_SIG,  // AO_HealthMonitor sends the requesting Member an acknowledgment if unsubscribing succeded
	TIMER_KICK_SIG,               // AO_HealthMonitor recieves a TIMER_KICK_SIG to signal the time to (potentially) kick the watchdog 
	TIMER_UPDATE_SIG,             // AO_HealthMonitor recieves a TIMER_UPDATE_SIG to signal the time to request an update from the subscribed members
	ALIVE_SIG,                    //each subscribed member that receives an TIMER_UPDATE_SIG posts an ALIVE_SIG to AO_HealthMonitor in response
    MAX_SIG                       // the last signal
};




} // namespace Core_Health


struct User {
	int id;
	bool keep_alive_received;

	User() {
		id = UNIDENTIFIED_ID;
		keep_alive_received = false;
	}

};
namespace Core_Health {

	class MemberEvt : public QP::QEvt {
	public:
		int member_num;
	};
	class SubscribeUserEvt : public QP::QEvt {
	public:
		int sender_id;
		int id;
	};

	class UnSubscribeUserEvt : public QP::QEvt {
	public:
		int sender_id;
		int member_num;
	};

	class DeactivationEvt : public MemberEvt {
	public:
		uint8_t period_num;
	};



}; // namespace Core_Health



// number of members
#define N_MEMBER ((uint8_t)5)


namespace Core_Health {

extern QP::QActive * const AO_Member[N_MEMBER];

} 
namespace Core_Health {

	extern QP::QActive* const AO_HealthMonitor;


	// namespace Core_Health

}



#endif // Core_Health_h