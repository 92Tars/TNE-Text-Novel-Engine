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
	Copyright 2018~2021.
*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define DEFAULT_SCRIPT_SPEED 10

#define FLUSH i = 0
#define FLUSH_BUFFER cin.ignore(INT_MAX, '\n');

#define ENTER printf("\n")
#define DOUBLE_ENTER printf("\n\n")
#define WINDOW_CLEAR system("cls")

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <Windows.h>
#include <iostream>

using namespace std;
using std::vector;


struct game_framework
{
	int room = 0;
	int next_room = 0;
	char room_char;
};

struct route_framework
{
  int target_room; //현재 있는 방 번호
  int select_route; // 선택한 루트 번호
  int return_room; // 이동할 방 번호
};

char script_buffer[1024];
char route_buffer[128];
char buffer;
int my_select = 0;
int script_speed = DEFAULT_SCRIPT_SPEED;

FILE *game_script;
FILE *game_route;
game_framework status;
vector<route_framework> route_list;

int gameLoop();
int routeSelect(int room, int select);
int loadSave();
int mainMenu();
int endingMenu();

void routeInit();
void routeCheck();
void autoSave(int room_no);
void intro();
void breakFlagCheck();

void DEBUG_ROUTE_PRINT();

