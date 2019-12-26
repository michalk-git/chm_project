#pragma once
#include <chrono>
#include "system.h"
using namespace std::chrono;
using namespace Core_Health;

class Command {
public:
	int time;
	Command(int cmd_time) : time(cmd_time) {};
	virtual ~Command() = default;
	virtual void ExecuteCmd(int index)const = 0 ;
};

class UnSubscribe : public Command {
public:
	UnSubscribe(int cmd_time) : Command(cmd_time) {};
	virtual void ExecuteCmd(int index)const override;
};

class Subscribe : public Command {
	int user_id;
public:
	Subscribe(int cmd_time, int new_user_id) : Command(cmd_time), user_id(new_user_id) {};
	virtual void ExecuteCmd(int index)const override;
};

class Deactivate : public Command {
	int cycle_num;
public:
	Deactivate(int cmd_time, int deactivation_cycles) : Command(cmd_time), cycle_num(deactivation_cycles) {};
	virtual void ExecuteCmd(int index)const override;
};

class NOP : public Command {
public:
	NOP() : Command(0) {};
	virtual void ExecuteCmd(int index)const override {};
};