//****************************************************************************
// Product: DPP example (console)
// Last Updated for Version: 6.3.6
// Date of the Last Update:  2018-10-14
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

#include <stdio.h>
#include <stdlib.h>

Q_DEFINE_THIS_FILE

//****************************************************************************
namespace Core_Health {

// Local objects -------------------------------------------------------------
static uint32_t l_rnd; // random seed

#ifdef Q_SPY

    static uint8_t const l_clock_tick = 0U;
#endif

//............................................................................
void BSP::init(int argc, char **argv) {

    BSP::randomSeed(1234U);

    (void)argc;
    (void)argv;
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));
    QS_OBJ_DICTIONARY(&l_clock_tick); // must be called *after* QF::init()

    // setup the QS filters...
    QS_FILTER_ON(QP::QS_ALL_RECORDS);
    QS_FILTER_OFF(QP::QS_QF_TICK);
}
//............................................................................
void BSP::terminate(int16_t result) {
    (void)result;
    QP::QF::stop();
}
//............................................................................
void BSP::displayPaused(uint8_t paused) {
    printf("Paused is %s\n", paused ? "ON" : "OFF");
}
//............................................................................
uint32_t BSP::random(void) { // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);
    return l_rnd >> 8;
}
//............................................................................
void BSP::randomSeed(uint32_t seed) {
    l_rnd = seed;
}

} // namespace DPP


//****************************************************************************
namespace QP {

//............................................................................
void QF::onStartup(void) { // QS startup callback
    QF_consoleSetup();
    QF_setTickRate(Core_Health::BSP::TICKS_PER_SEC, 30); // desired tick rate/prio
}
//............................................................................
void QF::onCleanup(void) {  // cleanup callback
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
    QF::TICK_X(0U, &Core_Health::l_clock_tick); // process time events at rate 0

    QS_RX_INPUT(); // handle the QS-RX input
    QS_OUTPUT();   // handle the QS output

    }
}

//----------------------------------------------------------------------------
#ifdef Q_SPY

//............................................................................
//! callback function to execute a user command (to be implemented in BSP)
void QS::onCommand(uint8_t cmdId,
                   uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }

    // unused parameters
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif // Q_SPY
//----------------------------------------------------------------------------

 // namespace QP

//............................................................................
 void Q_onAssert(char const * const module, int_t loc) {
    QS_ASSERTION(module, loc, (uint32_t)10000U); // report assertion to QS
    fprintf(stderr, "Assertion failed in %s:%d", module, loc);
    QP::QF::onCleanup();
    exit(-1);
}

