


#include "MemberTest.h"
#include "watchdog.h"
#include "CoreDebug.h"



#define TESTS_NUM 18
using namespace Core_Health;





//............................................................................
int main(int argc, char* argv[]) {
	static QP::QEvt const* chmQueueSto[N_MEMBER];
	static QP::QEvt const* memberQueueSto[N_MEMBER][200*N_MEMBER];
	static QP::QSubscrList subscrSto[MAX_PUB_SIG];
	static QF_MPOOL_EL(MemberEvt) smlPoolSto[10 * N_MEMBER];
	static QF_MPOOL_EL(SubscribeUserEvt ) medPoolSto[3* N_MEMBER];
	QP::QF::init();  // initialize the framework and the underlying RT kernel

	Core_Health::BSP::init(argc, argv); // initialize the BSP

	QP::QF::psInit(subscrSto, Q_DIM(subscrSto)); // init publish-subscribe

	// initialize event pools...
	QP::QF::poolInit(smlPoolSto,
		sizeof(smlPoolSto), sizeof(smlPoolSto[0]));
	QP::QF::poolInit(medPoolSto ,sizeof(medPoolSto), sizeof(medPoolSto[0]));

	

	// initialize tests matrix which holds all the tests for each member active object
	MemberTest tests[N_MEMBER][TESTS_NUM];


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TEST #0 :
//  Tests case where there is one subscriber
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	int test_num = 0;
	Command* T00_M00[] = { new Subscribe(0, 1), new NOP() };                                                    tests[0][test_num].AddTest(T00_M00);
	Command* T00_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T00_M01);
	Command* T00_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T00_M02);
	Command* T00_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T00_M03);
	Command* T00_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T00_M04);
	// ________________________________________________

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TEST #1 :
//  Tests case where there is one subscriber who unsubscribes
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 1;
	Command* T01_M00[] = { new Subscribe(0, 1), new UnSubscribe(25), new NOP() };                               tests[0][test_num].AddTest(T01_M00);
	Command* T01_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T01_M01);
	Command* T01_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T01_M02);
	Command* T01_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T01_M03);
	Command* T01_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T01_M04);
	// ________________________________________________

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// TEST #2 :
//  Tests case where there is one subscriber who deactivates
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 2;
	Command* T02_M00[] = { new Subscribe(0, 1), new Deactivate(25,4), new NOP() };                              tests[0][test_num].AddTest(T02_M00);
	Command* T02_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T02_M01);
	Command* T02_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T02_M02);
	Command* T02_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T02_M03);
	Command* T02_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T02_M04);
	// ________________________________________________

	// TEST #3 :
//  Tests case of trying to unsubscribe a non-existant user
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 3;
	Command* T03_M00[] = { new UnSubscribe(5), new NOP() };                                                     tests[0][test_num].AddTest(T03_M00);
	Command* T03_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T03_M01);
	Command* T03_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T03_M02);
	Command* T03_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T03_M03);
	Command* T03_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T03_M04);
	// ________________________________________________

// TEST #4 :
//  Tests case of trying to deactivate a non-existant user
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 4;
	Command* T04_M00[] = { new Deactivate(15,4), new NOP() };                                                   tests[0][test_num].AddTest(T04_M00);
	Command* T04_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T04_M01);
	Command* T04_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T04_M02);
	Command* T04_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T04_M03);
	Command* T04_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T04_M04);
	// ________________________________________________

// TEST #5 :
//  Tests case of trying to subscribe a user twice
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 5;
	Command* T05_M00[] = { new Subscribe(0, 1), new UnSubscribe(5), new UnSubscribe(10), new NOP() };           tests[0][test_num].AddTest(T05_M00);
	Command* T05_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T05_M01);
	Command* T05_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T05_M02);
	Command* T05_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T05_M03);
	Command* T05_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T05_M04);
	// ________________________________________________

// TEST #6 :
//  Tests case of trying to deactivate an unsubscribed user
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 6;
	Command* T06_M00[] = { new Subscribe(0, 1), new UnSubscribe(5), new Deactivate(15,4), new NOP() };          tests[0][test_num].AddTest(T06_M00);
	Command* T06_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T06_M01);
	Command* T06_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T06_M02);
	Command* T06_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T06_M03);
	Command* T06_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T06_M04);
	// ________________________________________________

// TEST #7 :
//  Tests case of trying to subscribe same user twice to same ao
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 7;
	Command* T07_M00[] = { new Subscribe(0, 1),new Subscribe(5,1), new NOP() };                                 tests[0][test_num].AddTest(T07_M00);
	Command* T07_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T07_M01);
	Command* T07_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T07_M02);
	Command* T07_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T07_M03);
	Command* T07_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T07_M04);
	// ________________________________________________

// TEST #8 :
//  Tests case of trying to to subscribe different users to same ao
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 8;
	Command* T08_M00[] = { new Subscribe(0, 1), new Subscribe(5,2), new NOP() };                                tests[0][test_num].AddTest(T08_M00);
	Command* T08_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T08_M01);
	Command* T08_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T08_M02);
	Command* T08_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T08_M03);
	Command* T08_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T08_M04);
	// ________________________________________________

	// TEST #9 :
//  Tests case of trying to to subscribe same user after deactivation
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 9;
	Command* T09_M00[] = { new Subscribe(0, 1),new Deactivate(5,4), new Subscribe(10,1), new NOP() };           tests[0][test_num].AddTest(T09_M00);
	Command* T09_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T09_M01);
	Command* T09_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T09_M02);
	Command* T09_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T09_M03);
	Command* T09_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T09_M04);
	// ________________________________________________

// TEST #10 :
//  Tests case of trying to to subscribe different user after deactivation
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 10;
	Command* T10_M00[] = { new Subscribe(0, 1),new Deactivate(5,4), new Subscribe(10,2), new NOP() };           tests[0][test_num].AddTest(T10_M00);
	Command* T10_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T10_M01);
	Command* T10_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T10_M02);
	Command* T10_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T10_M03);
	Command* T10_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T10_M04);
	// ________________________________________________


	// TEST #11 :
	//  Tests subscribing of user on different ao after unsubscribing
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 11;
	Command* T11_M00[] = { new Subscribe(5, 4), new UnSubscribe(10), new NOP() };                               tests[0][test_num].AddTest(T11_M00);
	Command* T11_M01[] = { new Subscribe(8, 4), new Subscribe(13,4) ,new NOP() };                               tests[1][test_num].AddTest(T11_M01);
	Command* T11_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T11_M02);
	Command* T11_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T11_M03);
	Command* T11_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T11_M04);
	// ________________________________________________

	// TEST #12 :
	//  Tests unsubscribing deactivated member
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 12;
	Command* T12_M00[] = { new Subscribe(5, 1), new Deactivate(13,6),  new UnSubscribe(40), new NOP() };        tests[0][test_num].AddTest(T12_M00);
	Command* T12_M01[] = { new NOP() };                                                                         tests[1][test_num].AddTest(T12_M01);
	Command* T12_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T12_M02);
	Command* T12_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T12_M03);
	Command* T12_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T12_M04);
	// ________________________________________________

    // TEST #13 :
	//  Tests trying to subscribe same user on different aos
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 13;
	Command* T13_M00[] = { new Subscribe(0, 1), new NOP() };                                                    tests[0][test_num].AddTest(T13_M00);
	Command* T13_M01[] = { new Subscribe(5, 1), new NOP() };                                                    tests[1][test_num].AddTest(T13_M01);
	Command* T13_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T13_M02);
	Command* T13_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T13_M03);
	Command* T13_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T13_M04);
	// ________________________________________________

	// TEST #14 :
	//  Tests subscribing two users, one deactivated
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 14;
	Command* T14_M00[] = { new Subscribe(0, 1), new NOP() };                                                    tests[0][test_num].AddTest(T14_M00);
	Command* T14_M01[] = { new Subscribe(5, 2), new Deactivate(13,3), new NOP() };                              tests[1][test_num].AddTest(T14_M01);
	Command* T14_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T14_M02);
	Command* T14_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T14_M03);
	Command* T14_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T14_M04);
	// ________________________________________________

	// TEST #15 :
	//  Tests subscribing two users, one deactivated and then unsubscribed
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 15;
	Command* T15_M00[] = { new Subscribe(0, 1), new NOP() };                                                    tests[0][test_num].AddTest(T15_M00);
	Command* T15_M01[] = { new Subscribe(5, 2), new Deactivate(13,3), new UnSubscribe(50), new NOP() };         tests[1][test_num].AddTest(T15_M01);
	Command* T15_M02[] = { new NOP() };                                                                         tests[2][test_num].AddTest(T15_M02);
	Command* T15_M03[] = { new NOP() };                                                                         tests[3][test_num].AddTest(T15_M03);
	Command* T15_M04[] = { new NOP() };                                                                         tests[4][test_num].AddTest(T15_M04);
	// ________________________________________________

		// TEST #16 :
	//  Tests subscribing two users, one deactivated and then unsubscribed
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 16;
	Command* T16_M00[] = { new NOP() };                                                                                            tests[0][test_num].AddTest(T16_M00);
	Command* T16_M01[] = { new Subscribe(5, 1), new Deactivate(11,3), new UnSubscribe(14),new Subscribe(16, 1), new NOP() };       tests[1][test_num].AddTest(T16_M01);
	Command* T16_M02[] = { new NOP() };                                                                                            tests[2][test_num].AddTest(T16_M02);
	Command* T16_M03[] = { new NOP() };                                                                                            tests[3][test_num].AddTest(T16_M03);
	Command* T16_M04[] = { new NOP() };                                                                                            tests[4][test_num].AddTest(T16_M04);
	// ________________________________________________

		// TEST #17 :
	//  Tests subscribing two users, one deactivated and then unsubscribed
	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	test_num = 17;
	Command* T17_M00[] = { new Subscribe(4, 1), new NOP() };                                                                         tests[0][test_num].AddTest(T17_M00);
	Command* T17_M01[] = { new Subscribe(5, 2), new UnSubscribe(10), new NOP() };                                                    tests[1][test_num].AddTest(T17_M01);
	Command* T17_M02[] = { new Subscribe(6, 3) ,new NOP() };                                                                         tests[2][test_num].AddTest(T17_M02);
	Command* T17_M03[] = { new Subscribe(7, 4),new UnSubscribe(12),new Subscribe(14, 10), new NOP() };                               tests[3][test_num].AddTest(T17_M03);
	Command* T17_M04[] = { new Subscribe(8, 5), new NOP() };                                                                         tests[4][test_num].AddTest(T17_M04);
	// ________________________________________________



	// start the active objects...
	for (uint8_t n = 0U; n < N_MEMBER; ++n) {
		// create initialization event for AO_Member active object associated with INIT_SIG signal
		InitializationEvt* init_evt = Q_NEW(InitializationEvt, INIT_SIG);
		init_evt->commands_test = &tests[n][11];
		AO_Member[n]->start((uint8_t)(n + 1U),
			memberQueueSto[n], Q_DIM(memberQueueSto[n]),
			(void*)0, 0U, init_evt);
	}

	// create initialization event for AO_HealthMonitor
	AO_HealthMonitor->start((uint8_t)(N_MEMBER + 1U),
		chmQueueSto, Q_DIM(chmQueueSto),
		(void*)0, 0U);

	QP::QF::run(); // run the QF application



	return 0;
}
