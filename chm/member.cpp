
#include "system.h"
#include "test.h"
using namespace std;
Q_DEFINE_THIS_FILE



// Active object class -------------------------------------------------------
//$declare${AOs::Member} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace Core_Health {

//${AOs::Member} ..............................................................
class Member : public QP::QActive {
public:
    static Member inst[N_MEMBER];

private:

	int system_id;

	// used for testing 
	MemberTest* test;

	int num_deactivated_cycles;


	///////////////////
public:
	Member();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(active);

};

} // namespace Core_Health
//$enddecl${AOs::Member} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


namespace Core_Health {



// Local objects -------------------------------------------------------------
static Member l_Member[N_MEMBER];   // storage for all Members


} // namespace Core_Health

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 650U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.5.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::AO_Member[N_Member]} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace Core_Health {

//${AOs::AO_Member[N_Member]} ..................................................
QP::QActive * const AO_Member[N_MEMBER] = { // "opaque" pointers to Member AO
    & Member::inst[0],
    & Member::inst[1],
    & Member::inst[2],
    & Member::inst[3],
    & Member::inst[4]
};


// helper function to provide the ID of Member "me"
inline uint8_t MEMBER_ID(Member const* const me) {
	return static_cast<uint8_t>(me - &Member::inst[0]);
}

} // namespace Core_Health
//$enddef${AOs::AO_Member[N_Member]} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Member} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace Core_Health {

	//${AOs::Member} ..............................................................
	Member Member::inst[N_MEMBER];
	//${AOs::Member::Member} .......................................................
	Member::Member()
		: QActive(&initial), num_deactivated_cycles(0), system_id(-1){};
		
	

	//${AOs::Member::SM} ..........................................................
	Q_STATE_DEF(Member, initial) {
		//${AOs::Member::SM::initial}

		// handle initialization event 
		if (e->sig == INIT_SIG) {
			test = (Q_EVT_CAST(InitializationEvt)->commands_test);

			// subscribe to 'START_TESTS_SIG' (which when received signals the start of running the command array of the member AO)
			subscribe(START_TESTS_SIG);

		}
		return tran(&active);
	}

	//${AOs::Member::SM::active} ................................................
	Q_STATE_DEF(Member, active) {
		QP::QState status_;
		switch (e->sig) {

		case START_TESTS_SIG: {
			// when the member AO receives a 'START_TESTS_SIG' , it will prompt a subscription to the signal TICK_SIG
			subscribe(TICK_SIG);
			status_ = Q_RET_HANDLED;
			break;
		}
		case TICK_SIG: {
			// call the command handler each time the member AO receives a 'TICK_SIG' signal
			test->ExecuteCurrentCommand(MEMBER_ID(this));
			status_ = Q_RET_HANDLED;
			break;
		}
		case MEMBER_SUBSCRIBE_SIG: {
			if (system_id == -1) {
				// when the member AO receives a 'MEMBER_SUBSCRIBE_SIG' 
				((QP::QEvt*)e)->sig = SUBSCRIBE_SIG;
				AO_HealthMonitor->POST((SubscribeUserEvt*)e, this);
			}
			else PRINT_LOG("Subscribing didn't succeed\n");
			status_ = Q_RET_HANDLED;
			break;
		}
		case MEMBER_UNSUBSCRIBE_SIG: {
			UnSubscribeUserEvt* unsub = Q_NEW(UnSubscribeUserEvt, UNSUBSCRIBE_SIG);
			unsub->member_num = system_id;
			unsub->sender_id = (Q_EVT_CAST(UnSubscribeUserEvt)->sender_id);
			AO_HealthMonitor->POST(unsub, this);
			status_ = Q_RET_HANDLED;
			break;
		}
		case REQUEST_UPDATE_SIG: {
			//if a member AO recevied a REQUEST_UPDATE_SIG it needs to post an ALIVE_SIG to the HealthMonitor active object unless it has been deactivated
			if (num_deactivated_cycles == 0) {
				MemberEvt* alive_evt = Q_NEW(MemberEvt, ALIVE_SIG);
				alive_evt->member_num = system_id;
				AO_HealthMonitor->POST(alive_evt, this);
				PRINT_LOG("Member %d has sent ALIVE signal\n",(int)system_id);
			}
			else --num_deactivated_cycles;
			status_ = Q_RET_HANDLED;
			break;
		}

        
		case SUBSCRIBE_ACKNOWLEDGE_SIG: {
			//all users who wish to subscribe to health monitor system will subscribe to the REQUEST_UPDATE_SIG signal
			subscribe(REQUEST_UPDATE_SIG);
			system_id = (Q_EVT_CAST(MemberEvt)->member_num);
			PRINT_LOG("Member %d has subscribed\n", system_id);
			status_ = Q_RET_HANDLED;
			break;
		}
		case UNSUBSCRIBE_ACKNOWLEDGE_SIG: {
			//users who wish to unsubscribe will stop receiving REQUEST_UPDATE_SIG signal
			 unsubscribe(REQUEST_UPDATE_SIG);
			 PRINT_LOG("Member %d has unsubscribed\n", system_id);
			 system_id = -1;
			 num_deactivated_cycles = 0;
			 status_ = Q_RET_HANDLED;
			 break;
		}
		//used for testing purposes
		case DEACTIVATE_SIG: {
			if (system_id != -1) {
				num_deactivated_cycles = (Q_EVT_CAST(DeactivationEvt)->period_num);
				PRINT_LOG("Member %d is deactivated for %d cycles\n ", system_id, num_deactivated_cycles);
			}
			status_ = Q_RET_HANDLED;
			break;
		}
		//${AOs::Member::SM::active}
		case Q_EXIT_SIG: {
			
			status_ = Q_RET_HANDLED;
			break;
		}

		default: {
			status_ = super(&top);
			break;
		}
		}
		return status_;
	}

}
