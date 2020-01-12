#pragma once

#include <chrono>
#include "Command.h"
#include "MemberTest.h"

class TestsHandler {
	MemberTest* tests_vec;
	int curr_test_index;
	int tests_num;
public:
	TestsHandler() :tests_vec(NULL), curr_test_index(0), tests_num(0) {};
	TestsHandler(MemberTest* tests, int tests_len): tests_vec(tests), curr_test_index(0), tests_num(tests_len) {};
	void ExecuteTest(int ao_index) {
		// check if there aren't any tests left to run
		if (curr_test_index == tests_num) return;
		// if there are, run the current test
		tests_vec[curr_test_index].ExecuteCurrentCommand(ao_index);
	}
	void StartNewTest(system_clock::time_point test_start_time) {
		// check if there aren't any tests left to run
		if (curr_test_index == tests_num) return;
		else {
			curr_test_index++;
			tests_vec[curr_test_index].UpdateTestStartTime(test_start_time);
		}
	}
};



class InitializationEvt : public QP::QEvt {
public:
	MemberTest* commands_test;

};




class InitSysEvt : public QP::QEvt {
public:
	char* tests_name;
	int test_num;


};
