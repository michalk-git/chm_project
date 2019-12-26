/// @file
/// @brief QF/C++ port to ARM Cortex-R, cooperative QV kernel, GNU-ARM toolset
/// @cond
///***************************************************************************
/// Last updated for version 6.6.0
/// Last updated on  2019-07-30
///
///                    Q u a n t u m  L e a P s
///                    ------------------------
///                    Modern Embedded Software
///
/// Copyright (C) 2005-2019 Quantum Leaps. All rights reserved.
///
/// This program is open source software: you can redistribute it and/or
/// modify it under the terms of the GNU General Public License as published
/// by the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// Alternatively, this program may be distributed and modified under the
/// terms of Quantum Leaps commercial licenses, which expressly supersede
/// the GNU General Public License and are specifically designed for
/// licensees interested in retaining the proprietary status of their code.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <www.gnu.org/licenses>.
///
/// Contact information:
/// <www.state-machine.com>
/// <info@state-machine.com>
///***************************************************************************
/// @endcond

#ifndef QF_PORT_HPP
#define QF_PORT_HPP

// The maximum number of active objects in the application, see NOTE1
#define QF_MAX_ACTIVE          32

// The maximum number of system clock tick rates
#define QF_MAX_TICK_RATE       2

// QF interrupt disable/enable, see NOTE2
#ifdef __thumb__  // THUMB mode?

    #define QF_INT_DISABLE()    __asm volatile ("cpsid i")
    #define QF_INT_ENABLE()     __asm volatile ("cpsie i")
    #define QF_INT_ENABLE_ALL() __asm volatile ("cpsie if")

#elif (defined __arm__) // ARM mode?

    #define QF_INT_DISABLE()   \
        __asm volatile ("MSR cpsr_c,#(0x1F | 0x80)" ::: "cc")
    #define QF_INT_ENABLE() \
        __asm volatile ("MSR cpsr_c,#(0x1F)" ::: "cc")
    #define QF_INT_ENABLE_ALL() \
        __asm volatile ("MSR cpsr_c,#(0x1F | 0x80 | 0x40)" ::: "cc")
#else
    #error Incorrect CPU mode. Must be either __arm__ or __thumb__.
#endif

// Cortex-R provide the CLZ instruction for fast LOG2
#define QF_LOG2(n_) ((uint8_t)(32U - __builtin_clz(n_)))

// QF critical section entry/exit, see NOTE3
#define QF_CRIT_STAT_TYPE      uint32_t
#define QF_CRIT_ENTRY(stat_)   do { \
    __asm volatile ("MRS %0,cpsr" : "=r" (stat_) :: "cc"); \
    QF_INT_DISABLE(); \
} while (0)
#define QF_CRIT_EXIT(stat_) \
    __asm volatile ("MSR cpsr_c,%0" :: "r" (stat_) : "cc")
#define QF_CRIT_EXIT_NOP()     __asm volatile ("ISB")

#include "qep_port.hpp"   // QEP port
#include "qv_port.hpp"    // QV port
#include "qf.hpp"         // QF platform-independent public interface

//****************************************************************************
// NOTE1:
// The maximum number of active objects QF_MAX_ACTIVE can be increased
// up to 63, if necessary. Here it is set to a lower level to save some RAM.
//
// NOTE2:
// The FIQ-type interrupts are NEVER disabled in this port, so the FIQ is
// a "kernel-unaware" interrupt. If the FIQ is ever used in the application,
// it must be an "ARM FIQ"-type function. For this to work, the FIQ
// stack needs to be initialized.
//
// NOTE3:
// This port implements the "save and restore" interrupt status policy,
// which again never disables the FIQ-type interrupts. This policy allows
// for nesting critical sections, which is necessary inside IRQ-type
// interrupts that run with interrupts (IRQ) disabled.
//

#endif // QF_PORT_HPP
