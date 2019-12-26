@echo off
:: ==========================================================================
:: Product: qclean buld script for Win32 port with GNU (MinGW)
:: Last updated for version 6.6.0
:: Last updated on  2019-07-29
::
::                    Q u a n t u m  L e a P s
::                    ------------------------
::                    Modern Embedded Software
::
:: Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 3 of the License, or
:: (at your option) any later version.
::
:: Alternatively, this program may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GNU General Public License and are specifically designed for
:: licensees interested in retaining the proprietary status of their code.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: www.state-machine.com
:: info@state-machine.com
:: ==========================================================================
setlocal

:: NOTE:
:: -----
:: This Makefile assumes that the MinGW compiler is in the PATH.
:: The Qtools collection for Windows now includes the MinGW compiler,
:: so if you have added the Qtools\bin\ directory to your PATH,
:: you should be covered.

set CC=%QTOOLS%\MinGW\bin\gcc
set LINK=%QTOOLS%\MinGW\bin\gcc

:: ==========================================================================
if "%1"=="" (
    echo default selected
    set BINDIR=rel
    set CCFLAGS=-O2 -c -Wall -Wextra
)
if "%1"=="dbg" (
    echo dbg selected
    set BINDIR=dbg
    set CCFLAGS=-g -c -Wall -Wextra 
)

set LINKFLAGS=-static-libgcc

set SRCDIR=..\source
set CCINC=-I..\include

@echo on
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\main.o       %SRCDIR%\main.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\getopt.o     %SRCDIR%\getopt.c
%CC% %CCFLAGS% %CCINC% -o%BINDIR%\filesearch.o filesearch.c

%LINK% %LINKFLAGS% -o %BINDIR%\qclean.exe %BINDIR%\main.o %BINDIR%\getopt.o %BINDIR%\filesearch.o

@echo off
erase %BINDIR%\*.o

:end

endlocal