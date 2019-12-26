//$file${.::dpp.hpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: dpp_qmsm.qm
// File:  ${.::dpp.hpp}
//
// This code has been generated by QM 4.5.1 (https://www.state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This code is covered by the following QP license:
// License #   : QPCPP-EVAL
// Issued to   : Institution or an individual evaluating the QP/C++ framework
// Framework(s): qpcpp
// Support ends: 2019-12-31
// Product(s)  :
// This license is available only for evaluation purposes and
// the generated code is still licensed under the terms of GPL.
// Please submit request for extension of the evaluaion period at:
// https://www.state-machine.com/licensing/#RequestForm
//
//$endhead${.::dpp.hpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef DPP_HPP
#define DPP_HPP

namespace DPP {

enum DPPSignals {
    EAT_SIG = QP::Q_USER_SIG, // published by Table to let a philosopher eat
    DONE_SIG,       // published by Philosopher when done eating
    PAUSE_SIG,      // published by BSP to pause the application
    SERVE_SIG,      // published by BSP to serve re-start serving forks
    TEST_SIG,       // published by BSP to test the application
    MAX_PUB_SIG,    // the last published signal

    HUNGRY_SIG,     // posted direclty to Table from hungry Philo
    MAX_SIG         // the last signal
};

} // namespace DPP

//$declare${Events::TableEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

//${Events::TableEvt} ........................................................
class TableEvt : public QP::QEvt {
public:
    uint8_t philoNum;
};

} // namespace DPP
//$enddecl${Events::TableEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// number of philosophers
#define N_PHILO ((uint8_t)5)

//$declare${AOs::AO_Philo[N_PHILO]} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

extern QP::QActive * const AO_Philo[N_PHILO];

} // namespace DPP
//$enddecl${AOs::AO_Philo[N_PHILO]} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$declare${AOs::AO_Table} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

extern QP::QActive * const AO_Table;

} // namespace DPP
//$enddecl${AOs::AO_Table} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#ifdef QXK_HPP
//$declare${AOs::XT_Test1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

extern QP::QXThread * const XT_Test1;

} // namespace DPP
//$enddecl${AOs::XT_Test1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$declare${AOs::XT_Test2} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace DPP {

extern QP::QXThread * const XT_Test2;

} // namespace DPP
//$enddecl${AOs::XT_Test2} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#endif // QXK_HPP

#endif // DPP_HPP