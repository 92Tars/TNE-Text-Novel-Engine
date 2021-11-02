/*
  _______        _     _   _                _   ______             _             
 |__   __|      | |   | \ | |              | | |  ____|           (_)            
    | | _____  _| |_  |  \| | _____   _____| | | |__   _ __   __ _ _ _ __   ___  
    | |/ _ \ \/ / __| | . ` |/ _ \ \ / / _ \ | |  __| | '_ \ / _` | | '_ \ / _ \ 
    | |  __/>  <| |_  | |\  | (_) \ V /  __/ | | |____| | | | (_| | | | | |  __/ 
    |_|\___/_/\_\\__| |_| \_|\___/ \_/ \___|_| |______|_| |_|\__, |_|_| |_|\___| 
                                                              __/ |              
                                                             |___/               

	TNE (Text Novel Engine) v1.0 by 92Tars
	Copyright 2018.
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define FLUSH i = 0
#define ENTER printf("\n");
#define DOUBLE_ENTER printf("\n\n");

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

struct Game_Framework
{
	int room = 0;
	int next_room = 0;
	char room_char;
};

char Script_Buffer[500];
char buffer;
int my_select = 0;

int script_speed;

FILE *Game_Script;
Game_Framework Status;

int Game_Loop();
int Route_Select(int room, int select);
void Auto_Save(int room_no);
int Load_Save();
int main_menu();
void intro();

