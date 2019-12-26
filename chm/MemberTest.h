#pragma once
#include "Command.h"
#include <typeinfo>
class MemberTest {
	Command** commands_vec;
	int curr_index;
	system_clock::time_point initial_time;

public:
	MemberTest() : commands_vec(NULL), curr_index(0), initial_time(system_clock::now()) {};
	~MemberTest() {
		curr_index = 0;
		do {
			delete commands_vec[curr_index];
		} while (typeid(*commands_vec[curr_index++]) != typeid(NOP));
	}
	void ExecuteCurrentCommand(int index) {
		system_clock::time_point curr_time = system_clock::now();
		// find the duration of time since initialization of class in duration
		duration<int> seconds = duration_cast<std::chrono::seconds>(curr_time - initial_time);
		// check if we reached end of tests. If we have, we need to update the system
		if (typeid(*commands_vec[curr_index]) == typeid(NOP)) {
	     	AO_Member[index]->postFIFO(Q_NEW(QP::QEvt, FINISHED_TEST_SIG), this);
		    return;
		}
		// check if we need to run a new command yet
		else if (seconds.count() >= (commands_vec[curr_index]->time)) {
			commands_vec[curr_index++]->ExecuteCmd(index);
		}
	}
	
	void UpdateTestStartTime(system_clock::time_point test_start_time) {
		initial_time = test_start_time;
	}
	void AddTest(Command* command_array[]) {
		commands_vec = command_array;
		
	}

};
