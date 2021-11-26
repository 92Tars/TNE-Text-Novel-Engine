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
	while(true){ if(gameLoop() == 1) break; }
	
	if (endingMenu() == 1) goto restart;

	fgetc(stdin);
	return 0;
}

/*  [루트 파일 초기화]

	Script 파일에 있는 Route.txt 파일을 열어서 해당 게임의 분기점을 vector 형태로 저장

	! → 해당 방 번호 (room) 
	* → 선택지 번호  (select)
	> → 이동되는 방 번호 (return)
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

				route_list.push_back(Temp_RF); //route_list에 선택지 push
				i++;
			}
			i++;

			//Reset
			Temp_RF.target_room = 0;
			Temp_RF.select_route = 0;
			Temp_RF.return_room = 0;
		}
	}
}

/*	[게임 작동부]

	게임 스크립트를 받아와 출력한다.

	1. 현재 룸 번호를 받아와 Script 폴더 안에서 찾아 읽어들인다. (스크립트를 찾을 수 없는 경우 게임을 종료시킨다.)
	2. 브레이크 포인트가 나올 때 까지 script_buffer 배열을 채운다.
	3. 특수 기능들에 대한 예외처리를 한다. (`, $, $$, _, __, >nn)
	4. 버퍼를 출력하고 script_sleep 만큼 지연시킨다.
	
	gameLoop가 정상적으로 종료되면 1을, 비정상적으로 종료된다면 0을 반환한다.
*/
int gameLoop()
{
	//텍스트 파일 불러오는 곳
	char File_name[32] = "Script/Script_";
	sprintf(&status.room_char, "%d", status.room);
	strcat(File_name, &status.room_char);
	strcat(File_name, ".txt");

	//해당하는 대사 스크립트가 없을 시...
	if ((game_script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' 라는 게임 대사 스크립트가 발견되지 않았습니다! 게임을 실행할 수 없습니다!", File_name);

		breakFlagCheck();
		exit(1); 
	}

	//스크립트 출력부
	while ((buffer = fgetc(game_script) != EOF)) //End_Of_Line이 나올 때 까지 반복한다. (텍스트 전체 출력)
	{   
        int i = 0; // Index init

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
			}

			//분기점 포인트
			if (script_buffer[i] == '$') 
			{
				if(script_buffer[i + 1] == '$') i++;
				else 
				{
					i++;
					routeCheck();
				}
			}
			//게임 끝! 
			if (script_buffer[i] == '_') 
			{
				if(script_buffer[i + 1] == '_') i++;
				else return 1;
			}
			//버퍼출력
			printf("%c", script_buffer[i]); 
			i++;
			Sleep(script_speed); // 숫자를 바꿔서 빠르게도, 느리게도 출력이 가능함.
		}
		//키입력 대기
		fflush(stdout);
		breakFlagCheck();
	}
	fclose(game_script);
	return 0;
}

/*	[분기점 불러오기]

	루트 매니저 array 접근 후
	room → target_room, select → select_route 에 해당하는 요소 검색 후 return_room 반환
	만약 해당되는 요소가 없다면 -1 반환
	
*/
int routeSelect(int room, int select)
{
    for(int i = 0; i < route_list.size(); i++)
    {
       if(route_list[i].target_room == room && route_list[i].select_route == select) return route_list[i].return_room;
    }
    return -1;
}

/*	[분기점 선택기]

	선택지 입력받은 후 routeSelect를 다음 룸으로 지정.
	선택 후 룸을 옮기고 상태를 저장한다.
*/
void routeCheck()
{
	retry:
	printf("\n당신의 선택은? >> ");
	scanf("%d", &my_select);

	status.next_room = routeSelect(status.room, my_select);
	//오타 예외 처리
	if (status.next_room == -1)
	{
		printf("잘못된 선택입니다!\n\n");
		goto retry;
	}
	ENTER;
	WINDOW_CLEAR;

	printf("%d 번을 선택했습니다.\n\n", my_select);
	status.room = status.next_room;
	autoSave(status.room);
}

/*	[게임 인트로 로고]

	Script 파일에 있는 Intro.txt 파일을 열어서 출력
*/
void intro()
{
	int i = 0;
	if ((game_script = fopen("Script/Intro.txt", "r")) == NULL)
	{
		printf("인트로 파일이 존재하지 않습니다!!");
		exit(1); 
	}
	while ((buffer = fgetc(game_script)) != EOF) printf("%c", buffer); //버퍼출력
}

/*	[저장하기]

	savedata_.bin파일을 열어 현재 있는 방 번호를 저장한다
*/
void autoSave(int room_no)
{
	FILE *save_file;
	save_file = fopen("savedata_.bin", "wb");

	fwrite(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return;
}

/*	[이어하기]

	savedata_.bin파일을 열어 저장되어있는 방 번호를 반환한다
	만약 savedata_.bin이 없다면 안내 후 종료한다.
*/
int loadSave()
{
	int room_no;
	FILE *save_file = fopen("savedata_.bin", "rb");
	if (save_file == NULL) 
	{
		printf ("세이브 파일이 없습니다!!");
		breakFlagCheck();
		exit(1);
	}

	fread(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return room_no;
}

/*	[메인메뉴]

	메뉴값을 입력받아 기능을 실행시킨다.

	1. 새로하기
	2. 이어하기
	0. 종료
*/
int mainMenu() 
{
	int a = 0;
	while (true)
	{
		DOUBLE_ENTER;
		printf("메뉴 값을 입력하세요. >> ");

		scanf(" %d", &a);
		getchar();

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

/*	[종료화면]

	게임이 종료된 경우 메뉴값을 입력받아 기능을 실행시킨다.

	1. 메인화면
	2. 종료
*/
int endingMenu()
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("게임이 끝났습니다. 다시 진행하시겠습니까?");
		DOUBLE_ENTER;
		printf("1. 메인 화면으로	0. 종료");
		DOUBLE_ENTER;
		printf("메뉴 값을 입력하세요. >> ");

		scanf(" %d", &a);
		getchar();

		switch (a)
		{
		case 1:
			WINDOW_CLEAR;
			return 1;
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

/*	[입력스택 방지]
	
	입력버퍼를 초기화하고 키 입력을 대기받는다.
*/
void breakFlagCheck()
{
	FLUSH_BUFFER;
	while(!GetAsyncKeyState(VK_RETURN));
}
