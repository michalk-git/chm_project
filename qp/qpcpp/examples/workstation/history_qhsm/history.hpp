//$file${.::history.hpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: history.qm
// File:  ${.::history.hpp}
//
// This code has been generated by QM 4.5.1 (https://www.state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//$endhead${.::history.hpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef HISTORY_HPP
#define HISTORY_HPP

enum ToastOvenSignals {
    OPEN_SIG = QP::Q_USER_SIG,
    CLOSE_SIG,
    TOAST_SIG,
    BAKE_SIG,
    OFF_SIG,
    TERMINATE_SIG // terminate the application
};

extern QP::QHsm * const the_oven; // opaque pointer to the oven HSM

#endif // HISTORY_HPP