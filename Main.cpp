/*
  _______        _     _   _                _   ______             _
 |__   __|      | |   | \ | |              | | |  ____|           (_)
	| | _____  _| |_  |  \| | _____   _____| | | |__   _ __   __ _ _ _ __   ___
	| |/ _ \ \/ / __| | . ` |/ _ \ \ / / _ \ | |  __| | '_ \ / _` | | '_ \ / _ \
	| |  __/>  <| |_  | |\  | (_) \ V /  __/ | | |____| | | | (_| | | | | |  __/
	|_|\___/_/\_\\__| |_| \_|\___/ \_/ \___|_| |______|_| |_|\__, |_|_| |_|\___|
															  __/ |
															 |___/

	TNE (Text Novel Engine) v1.1 by 92Tars
	Copyright 2018~2021.

*/

#define _CRT_SECURE_NO_WARNINGS
#include "Game_Scene.h"

int main()
{
    routeInit();
	restart:
	intro(); 
	status.room = mainMenu();
	while(true) if (gameLoop() == 1) break;
	
	if (endingMenu() == 0) goto restart;

	fgetc(stdin);
	return 0;
	
}

/*          루트 파일 선택
이 부분을 수정하면 다른 이야기도 만들 수 있다.

	루트 매니저 array 접근 후
	room == target_room, select == select_route 에 해당하는 요소 검색 후 return_room 반환

*/
void routeInit()
{
	//해당하는 대사 스크립트가 없을 시...
	if ((game_route = fopen("Script/Route.txt", "r")) == NULL)
	{
		printf("Route.txt가 발견되지 않았습니다! 게임을 실행할 수 없습니다!");
		fgetc(stdin);
		exit(1); 
	}

	int i = 0;
	route_framework Temp_RF;

	while ((buffer = fgetc(game_route)) != EOF) //End_Of_Line이 나올 때 까지 반복한다.
	{
		FLUSH;

		while ((buffer = fgetc(game_route)) != '`') //'`'표시가 나올 때 까지 route_buffer를 채운다.
		{
			route_buffer[i] = buffer;
			i++;
		}
		route_buffer[i + 1] = '\0'; //루트 버퍼 마무리
		
		FLUSH;

		while (route_buffer[i] != '\0') //루트 버퍼의 끝까지
		{
			//현재 방
			if (route_buffer[i] == '!')
			{
				int a, b, c;
				a = (route_buffer[i + 1] - 48) * 100;
				b = (route_buffer[i + 2] - 48) * 10;
				c = (route_buffer[i + 3] - 48);
				Temp_RF.target_room = a + b + c;
				i += 4;
			}

			//고른 선택지
			if (route_buffer[i] == '*') 
			{
				int a;
				a = (route_buffer[i + 1] - 48);
				Temp_RF.select_route = a;
				i += 2;
			}

			//가게될 방
			if (route_buffer[i] == '>')
			{
				int a, b, c;
				a = (route_buffer[i + 1] - 48) * 100;
				b = (route_buffer[i + 2] - 48) * 10;
				c = (route_buffer[i + 3] - 48);
				Temp_RF.return_room = a + b + c;
				i += 4;

				route_list.push_back(Temp_RF);
				i++;
			}

			i++;
			Temp_RF.target_room = 0;
			Temp_RF.select_route = 0;
			Temp_RF.return_room = 0;
		}
	}
}


int routeSelect(int room, int select)
{
    for(int i = 0; i < route_list.size(); i++)
    {
       if(route_list[i].target_room == room && route_list[i].select_route == select) return route_list[i].return_room;
    }
    return 404;
}

//게임 작동부
int gameLoop()
{
	//텍스트 파일 불러오는 곳
	char File_name[32] = "Script/Script_";
	sprintf(&status.room_char, "%d", status.room);
	strcat(File_name, &status.room_char);
	strcat(File_name, ".txt");
	//-----------------------

	//해당하는 대사 스크립트가 없을 시...
	if ((game_script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' 라는 게임 대사 스크립트가 발견되지 않았습니다! 게임을 실행할 수 없습니다!", File_name);
		fgetc(stdin);
		exit(1); 
	}
    //printf("현재 방은 %s 입니다.", File_name);


	//스크립트 출력부
	while ((buffer = fgetc(game_script) != EOF)) //End_Of_Line이 나올 때 까지 반복한다.
	{   
        int i = 0; // Flush init

		while ((buffer = fgetc(game_script)) != '`') //'`'표시가 나올 때 까지 script_buffer를 채운다.
		{
			script_buffer[i] = buffer;
			i++;
		}
		script_buffer[i + 1] = '\0'; //스크립트 버퍼 마무리
		
		FLUSH;

		while (script_buffer[i] != '\0') //스크립트 버퍼의 끝까지
		{
			//출력속도 변경
			if (script_buffer[i] == '>')
			{
				int a, b;
				a = (script_buffer[i + 1] - 48) * 10;
				b = script_buffer[i + 2] - 48;
				script_speed = a + b;
				i += 3;
				//printf("속도를 %d 로 변경했습니다. ", script_speed); //디버그 옵션
			}

			//분기점 포인트
			if (script_buffer[i] == '$') 
			{
				if(script_buffer[i + 1] == '$') i++;
				else routeCheck();
			}
			//게임 끝! 
			if (script_buffer[i] == '_') return 1; 
			
			//버퍼출력
			printf("%c", script_buffer[i]); 
			i++;
			Sleep(script_speed); // 숫자를 바꿔서 빠르게도, 느리게도 출력이 가능함.
		}

		fgetc(stdin); //키입력 대기
	}
	fclose(game_script);
	return 0;
}

//분기점 체커
void routeCheck()
{
	retry:
	printf("\n당신의 선택은? >> ");
	scanf("%d", &my_select);
	status.next_room = routeSelect(status.room, my_select);

	//오타 예외 처리
	if (status.next_room == 404)
	{
		printf("잘못된 선택입니다!\n\n");
		goto retry;
	}

	ENTER;
	WINDOW_CLEAR;
	printf("%d 번을 선택했습니다.\n", my_select);

	fgetc(stdin);
	status.room = status.next_room;
	autoSave(status.room);
}

//게임 인트로 로고
void intro()
{
	int i = 0;
	if ((game_script = fopen("Script/Intro.txt", "r")) == NULL)
	{
		printf("인트로 파일이 존재하지 않습니다!!");
		fgetc(stdin);
		exit(1); 
	}

	while ((buffer = fgetc(game_script)) != EOF) printf("%c", buffer); //버퍼출력
}

//저장하기
void autoSave(int room_no)
{
	FILE *save_file;

	save_file = fopen("savedata_.bin", "wb");

	fwrite(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return;
}

//이어하기
int loadSave()
{

	int room_no;

	FILE *save_file = fopen("savedata_.bin", "rb");

	fread(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return room_no;
}

//메인메뉴
int mainMenu() 
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("메뉴 값을 입력하세요. >> ");

		scanf("%d", &a);

		switch (a)
		{
		case 1:
			WINDOW_CLEAR;
			return 0;
			break;
		case 2:
			WINDOW_CLEAR;
			return loadSave();
			break;
		case 0:
			exit(0);
			break;
		default:
			printf("잘못된 선택입니다.");
			break;
		}
	}
}

//종료화면
int endingMenu()
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("게임이 끝났습니다. 다시 진행하시겠습니까?");
		DOUBLE_ENTER;
		printf("1. 다시 시작	2. 끝내기");
		DOUBLE_ENTER;
		printf("메뉴 값을 입력하세요. >> ");

		scanf("%d", &a);

		switch (a)
		{
		case 1:
			WINDOW_CLEAR;
			return 0;
			break;

		case 2:
			exit(0);
			break;

		default:
			printf("잘못된 선택입니다.");
			break;
		}
	}
}

void DEBUG_ROUTE_PRINT()
{
	for(int i = 0; i < route_list.size(); i++)
	{
		printf("Target Room : %d / Select_Route : %d / Return_Room : %d \n", route_list[i].target_room, route_list[i].select_route, route_list[i].return_room);
	}
}