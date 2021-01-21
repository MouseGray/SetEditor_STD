#pragma once

#include <string>
#include <vector>

using namespace std;

class Task
{
public:
	Task(){}
	Task(int id, int attempts) {
		ID = id;
		Attempts = attempts;
	}
	bool IsInit = false;

	int ID = 0;

	int Attempts = 0;

	string TaskText;

	vector<char> SetNames;

	vector<string> GivenExpressions;
	vector<string> ToFindExpressions;
	vector<string> AnswerExpressions;
};

class UserInfo
{
public:
	int UserId = -1;

	string UserName;

	vector<int> CompletedTasks;
	vector<Task> CurrentTasks;

	int CurrentTask = 0;

	inline Task* GetCurrentTask() {
		return &CurrentTasks[CurrentTask];
	}
};

