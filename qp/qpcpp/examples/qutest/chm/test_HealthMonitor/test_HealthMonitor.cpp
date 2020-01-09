//****************************************************************************
// Product: QUTEST fixture for the DPP components
// Last Updated for Version: 6.3.6
// Date of the Last Update:  2018-10-04
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// https://www.state-machine.com
// mailto:info@state-machine.com
//****************************************************************************
#include "qpcpp.hpp"
#include "bsp.h"
#include "system.h"
using namespace Core_Health;

// instantiate dummy collaborator AOs...
static QP::QActiveDummy l_dummyMember[N_MEMBER];
QP::QActive * const Core_Health::AO_Member[N_MEMBER] = {
    &l_dummyMember[0],
    &l_dummyMember[1],
    &l_dummyMember[2],
    &l_dummyMember[3],
    &l_dummyMember[4]
};



//............................................................................
int main(int argc, char *argv[]) {
    static QP::QEvt const *HealthMonitorQueueSto[N_MEMBER];
    static QP::QSubscrList subscrSto[MAX_PUB_SIG];
    static QF_MPOOL_EL(SubscribeUserEvt) smlPoolSto[2*N_MEMBER];

    QP::QF::init();  // initialize the framework and the underlying RT kernel

    Core_Health::BSP::init(argc, argv); // initialize the BSP

    // object dictionaries...
    QS_OBJ_DICTIONARY(AO_HealthMonitor);
    QS_OBJ_DICTIONARY(AO_Member[0]);
    QS_OBJ_DICTIONARY(AO_Member[1]);
    QS_OBJ_DICTIONARY(AO_Member[2]);
    QS_OBJ_DICTIONARY(AO_Member[3]);
    QS_OBJ_DICTIONARY(AO_Member[4]);

    // signal dictionaries
    QS_SIG_DICTIONARY(TIMER_KICK_SIG,      (void *)0);
    QS_SIG_DICTIONARY(TIMER_UPDATE_SIG,       (void *)0);
    QS_SIG_DICTIONARY(ALIVE_SIG,     (void *)0);
    QS_SIG_DICTIONARY(SUBSCRIBE_SIG,     (void *)0);
    QS_SIG_DICTIONARY(UNSUBSCRIBE_SIG,      (void *)0);
    QS_SIG_DICTIONARY(DEACTIVATE_SIG,    (void *)0);
	QS_SIG_DICTIONARY(MEMBER_SUBSCRIBE_SIG,     (void *)0);
    QS_SIG_DICTIONARY(MEMBER_UNSUBSCRIBE_SIG,      (void *)0);
    QS_SIG_DICTIONARY(SUBSCRIBE_ACKNOWLEDGE_SIG,   (void *)0);
    QS_SIG_DICTIONARY(UNSUBSCRIBE_ACKNOWLEDGE_SIG,   (void *)0);
	QS_SIG_DICTIONARY(REQUEST_UPDATE_SIG,   (void *)0);

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

    // initialize publish-subscribe...
    QP::QF::psInit(subscrSto, Q_DIM(subscrSto));

    // initialize event pools...
    QP::QF::poolInit(smlPoolSto,
                     sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start and setup dummy AOs...
    // NOTE: You need to start dummy AOs, if you wish to subscribe
    //       them to events.
    for (int_t n = 0; n < N_MEMBER; ++n) {
        AO_Member[n]->start((uint_fast8_t)(n + 1U), // priority
                    (QP::QEvt const **)0, 0U, (void *)0, 0U);
		AO_Member[n]->subscribe(REQUEST_UPDATE_SIG);			
    }

    // start the active object under test (AOUT)...
    AO_HealthMonitor->start((uint_fast8_t)(N_MEMBER + 1U), // priority
                    HealthMonitorQueueSto, Q_DIM(HealthMonitorQueueSto),
                    (void *)0, 0U);

    return QP::QF::run(); // run the QF application
}


namespace QP {

//............................................................................
void QS::onTestSetup(void) {
}
//............................................................................
void QS::onTestTeardown(void) {
}

//............................................................................
// callback function to execute user commands
void QS::onCommand(uint8_t cmdId,
                   uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;
/*
    switch (cmdId) {
       case 0U: {
           QEvt const e = { PAUSE_SIG, 0U, 0U };
           AO_Table->dispatch(&e);
           break;
       }
       case 1U: {
           QEvt const e = { SERVE_SIG, 0U, 0U };
           AO_Table->dispatch(&e);
           break;
       }
       default:
           break;
    }
	*/
}

//****************************************************************************
// callback function to "massage" the event, if necessary
void QS::onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // this test is compiled for an embedded Target system
#endif
}
//............................................................................
// callback function to output the posted QP events (not used here)
void QS::onTestPost(void const *sender, QActive *recipient,
                    QEvt const *e, bool status)
{
    (void)sender;
    (void)status;
/*
    switch (e->sig) {
        case EAT_SIG:
        case DONE_SIG:
        case HUNGRY_SIG:
            QS_BEGIN(QUTEST_ON_POST, (void *)0) // application-specific record
                QS_SIG(e->sig, recipient);
                QS_U8(0, Q_EVT_CAST(TableEvt)->philoNum);
            QS_END()
            break;
        default:
            break;
    }
	*/
}

} // namespace QP
