#pragma once

#include "Header.h"

#include <mysql.h>

#define MYSQL_HOST "localhost"// "192.168.0.103"
#define MYSQL_USER "root"
#define MYSQL_PASS "policeman"
#define MYSQL_BD "sedbschema"
#define MYSQL_PORT 3306

#define MYSQL_USER_INFO_TABLE "userinfo"
#define MYSQL_USER_COMPLETED_TASK_TABLE "completedtasks"
#define MYSQL_USER_CURRENT_TASK_TABLE "currenttasks"

#define MYSQL_TASK_INFO_TABLE "taskinfo"
#define MYSQL_TASK_SETNAMES_TABLE "setnames"
#define MYSQL_TASK_GIVET_TABLE "taskgiven"
#define MYSQL_TASK_TOFIND_TABLE "tasktofind"
#define MYSQL_TASK_ANSWER_TABLE "taskanswer"

string formating(string str);

int mysql_LoadUserInfo(string login, string pass, UserInfo* UI) noexcept(false)
{
    string query;
	MYSQL_ROW row;
	MYSQL_RES* result;
	// ----------------------------------------
	// Подключение к базе данных
	// ----------------------------------------
    MYSQL mysql_connect;
    mysql_init(&mysql_connect);
    if (!mysql_real_connect(&mysql_connect, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_BD, MYSQL_PORT, NULL, 0)) return ERR_SQL_NO_CONNECTION;
	// ----------------------------------------------------------------------------------------------------
	//								Получение ID пользователя
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
    query = "SELECT `id` FROM userinfo WHERE login = \'" + login + "\' AND pass = \'" + pass + "\'";
    mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
    if (!(row = mysql_fetch_row(result))) {
		mysql_close(&mysql_connect);
        return ERR_SQL_QUERY_NOT_FOUND;
    }
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
    if (!(UI->UserId = atoi(row[0]))) {
        mysql_close(&mysql_connect);
        return ERR_SQL_QUERY_INVALID_VALUE;
    }
    UI->UserName = login;
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
    mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение списка выполненных заданий
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
    query = "SELECT `taskID` FROM completedtasks WHERE userID = " + to_string(UI->UserId);
    mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CompletedTasks.push_back(atoi(row[0]));
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение списка текущих заданий
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `taskID`, `attempts` FROM currenttasks WHERE userID = " + to_string(UI->UserId);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CurrentTasks.push_back(Task(atoi(row[0]), atoi(row[1])));
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------
	// Закрытие подключения
	// ----------------------------------------
    mysql_close(&mysql_connect);

    return ERR_SQL_NO_ERROR;
}

int mysql_SaveUserInfo(UserInfo* UI) noexcept(false)
{
	string query;
	MYSQL_ROW row;
    MYSQL_RES* result;
	// ----------------------------------------
	// Подключение к базе данных
	// ----------------------------------------
	MYSQL mysql_connect;
    mysql_init(&mysql_connect);
    if(!mysql_real_connect(&mysql_connect, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_BD, MYSQL_PORT, NULL, 0)) return ERR_SQL_NO_CONNECTION;
	// ----------------------------------------------------------------------------------------------------
	//								Сохраниение списка выполненных заданий
	// ----------------------------------------------------------------------------------------------------
	// Получение списка запроса
	// ----------------------------------------
	query = "SELECT * FROM completedtasks WHERE userID = " + to_string(UI->UserId);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Удаление результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		query = "DELETE FROM completedtasks WHERE ID = ";
		query += row[0];
		mysql_query(&mysql_connect, query.data());
	}
	// ----------------------------------------
	// Сохранение новых занчений
	// ----------------------------------------
	for (size_t i = 0; i < UI->CompletedTasks.size(); i++) {
		query = "INSERT FROM completedtasks (`ID`, `userID`, `taskID`) VALUES('" +
		to_string(UI->UserId) + '_' + to_string(i) + "', '" + to_string(UI->UserId) + "', '" + to_string(UI->CompletedTasks[i]) + "')";
		mysql_query(&mysql_connect, query.data());
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Сохраниение списка текущих заданий
	// ----------------------------------------------------------------------------------------------------
	// Получение списка запроса
	// ----------------------------------------
	query = "SELECT * FROM currenttasks WHERE userID = " + to_string(UI->UserId);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Удаление результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		query = "DELETE FROM currenttasks WHERE ID = ";
		query += row[0];
		mysql_query(&mysql_connect, query.data());
	}
	// ----------------------------------------
	// Сохранение новых занчений
	// ----------------------------------------
	for (size_t i = 0; i < UI->CurrentTasks.size(); i++) {
		query = "INSERT FROM currenttasks (`ID`, `userID`, `taskID`, `attempts`) VALUES('" +
			to_string(UI->UserId) + '_' + to_string(i) + "', '" + to_string(UI->UserId) + "', '" + to_string(UI->CurrentTasks[i].ID) + "', '" + to_string(UI->CurrentTasks[i].Attempts) + "')";
		mysql_query(&mysql_connect, query.data());
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------
	// Закрытие подключения
	// ----------------------------------------
	mysql_close(&mysql_connect);

	return ERR_SQL_NO_ERROR;
}

int mysql_LoadTaskInfo(int id, UserInfo* UI) noexcept(false)
{
	if (UI->CurrentTasks[id].IsInit) return ERR_SQL_NO_ERROR;

	string query;
	MYSQL_ROW row;
	MYSQL_RES* result;
	// ----------------------------------------
	// Подключение к базе данных
	// ----------------------------------------
	MYSQL mysql_connect;
	mysql_init(&mysql_connect);
	if (!mysql_real_connect(&mysql_connect, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_BD, MYSQL_PORT, NULL, 0)) return ERR_SQL_NO_CONNECTION;
	// ----------------------------------------------------------------------------------------------------
	//								Получение текста задания
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `text` FROM taskinfo WHERE id = " + to_string(UI->CurrentTasks[id].ID);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	if (!(row = mysql_fetch_row(result))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_QUERY_NOT_FOUND;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	UI->CurrentTasks[id].TaskText = row[0];
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение стандартных множеств
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `SetName` FROM setnames WHERE taskID = " + to_string(UI->CurrentTasks[id].ID);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CurrentTasks[id].SetNames.push_back(row[0][0]);
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение 'Дано'
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `given` FROM taskgiven WHERE taskID = " + to_string(UI->CurrentTasks[id].ID);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CurrentTasks[id].GivenExpressions.push_back(row[0]);
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение 'Найти'
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `ToFind` FROM tasktofind WHERE taskID = " + to_string(UI->CurrentTasks[id].ID);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CurrentTasks[id].ToFindExpressions.push_back(row[0]);
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------------------------------------------------------------------
	//								Получение 'Ответ'
	// ----------------------------------------------------------------------------------------------------
	// Генерация запроса
	// ----------------------------------------
	query = "SELECT `Answer` FROM taskanswer WHERE taskID = " + to_string(UI->CurrentTasks[id].ID);
	mysql_query(&mysql_connect, query.data());
	// ----------------------------------------
	// Обработка результата запроса
	// ----------------------------------------
	if (!(result = mysql_store_result(&mysql_connect))) {
		mysql_close(&mysql_connect);
		return ERR_SQL_DATABASE_NOT_AVAILABLE;
	}
	// ----------------------------------------
	// Запись результата запроса
	// ----------------------------------------
	while ((row = mysql_fetch_row(result))) {
		UI->CurrentTasks[id].AnswerExpressions.push_back(row[0]);
	}
	// ----------------------------------------
	// Очистка результата
	// ----------------------------------------
	mysql_free_result(result);
	// ----------------------------------------
	// Закрытие подключения
	// ----------------------------------------
	mysql_close(&mysql_connect);

	return ERR_SQL_NO_ERROR;
}

string formating(string str)
{
    string res;
    for (int i = 0, j = 0; i < (int)str.size(); i++)
    {
        if (i > 0 && str[i] == 'n' && str[i - 1] == '\\')
			res.back() = '\n';
        else 
			res += str[i];
    }
    return res;
}