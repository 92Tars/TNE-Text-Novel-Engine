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
    /*
	
	DEBUG_ROUTE_PRINT();
	printf("Task Complete");

	fgetc(stdin);
	return 0;
    */

    Route_Init();
	restart:
	intro(); 
	Status.room = main_menu();
	while(true) if (Game_Loop() == 1) break;
	
	if (ending_menu() == 0) goto restart;

	fgetc(stdin);
	return 0;
	
}

/*          루트 파일 선택
이 부분을 수정하면 다른 이야기도 만들 수 있다.

	루트 매니저 array 접근 후
	room == target_room, select == select_route 에 해당하는 요소 검색 후 return_room 반환

*/
void Route_Init()
{
	//해당하는 대사 스크립트가 없을 시...
	if ((Game_Route = fopen("Script/Route.txt", "r")) == NULL)
	{
		printf("Route.txt가 발견되지 않았습니다! 게임을 실행할 수 없습니다!");
		fgetc(stdin);
		exit(1); 
	}

	int i = 0;
	Route_Framework Temp_RF;

	while ((buffer = fgetc(Game_Route)) != EOF) //End_Of_Line이 나올 때 까지 반복한다.
	{
		FLUSH;

		while ((buffer = fgetc(Game_Route)) != '`') //'`'표시가 나올 때 까지 Route_Buffer를 채운다.
		{
			Route_Buffer[i] = buffer;
		    //printf("%c", buffer);
			i++;
		}
		Route_Buffer[i + 1] = '\0'; //루트 버퍼 마무리
		
		FLUSH;

		while (Route_Buffer[i] != '\0') //루트 버퍼의 끝까지
		{
			//현재 방
			if (Route_Buffer[i] == '!')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Temp_RF.target_room = a + b + c;
				i += 4;
				//printf("현재방 = %d \n", Temp_RF.target_room);
			}

			//고른 선택지
			if (Route_Buffer[i] == '*') 
			{
				int a;
				a = (Route_Buffer[i + 1] - 48);
				Temp_RF.select_route = a;
				i += 2;
				//printf("선택지 = %d\n", Temp_RF.select_route);
			}

			//가게될 방
			if (Route_Buffer[i] == '>')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Temp_RF.return_room = a + b + c;
				i += 4;
				//printf("가게될방 = %d\n", Temp_RF.return_room);

				Route_List.push_back(Temp_RF);
				//printf("푸시\n");
				i++;
			}
			i++;
			Temp_RF.target_room = 0;
			Temp_RF.select_route = 0;
			Temp_RF.return_room = 0;
		}
	}
}

void DEBUG_ROUTE_PRINT()
{
	for(int i = 0; i < Route_List.size(); i++)
	{
		printf("Target Room : %d / Select_Route : %d / Return_Room : %d \n", Route_List[i].target_room, Route_List[i].select_route, Route_List[i].return_room);
	}
}

int Route_Select(int room, int select)
{
    for(int i = 0; i < Route_List.size(); i++)
    {
       if(Route_List[i].target_room == room && Route_List[i].select_route == select) return Route_List[i].return_room;
    }
    return 404;
}

//게임 작동부
int Game_Loop()
{
	//텍스트 파일 불러오는 곳
	char File_name[32] = "Script/Script_";
	sprintf(&Status.room_char, "%d", Status.room);
	strcat(File_name, &Status.room_char);
	strcat(File_name, ".txt");
	//-----------------------

	//해당하는 대사 스크립트가 없을 시...
	if ((Game_Script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' 라는 게임 대사 스크립트가 발견되지 않았습니다! 게임을 실행할 수 없습니다!", File_name);
		fgetc(stdin);
		exit(1); 
	}
    //printf("현재 방은 %s 입니다.", File_name);


	//스크립트 출력부
	while ((buffer = fgetc(Game_Script) != EOF)) //End_Of_Line이 나올 때 까지 반복한다.
	{   
        int i = 0; // Flush init

		while ((buffer = fgetc(Game_Script)) != '`') //'`'표시가 나올 때 까지 Script_Buffer를 채운다.
		{
			Script_Buffer[i] = buffer;
			i++;
		}
		Script_Buffer[i + 1] = '\0'; //스크립트 버퍼 마무리
		
		FLUSH;

		while (Script_Buffer[i] != '\0') //스크립트 버퍼의 끝까지
		{
			//출력속도 변경
			if (Script_Buffer[i] == '>')
			{
				int a, b;
				a = (Script_Buffer[i + 1] - 48) * 10;
				b = Script_Buffer[i + 2] - 48;
				script_speed = a + b;
				i += 3;
				//printf("속도를 %d 로 변경했습니다. ", script_speed); //디버그 옵션
			}

			//분기점 포인트
			if (Script_Buffer[i] == '$') 
			{
				if(Script_Buffer[i + 1] == '$') i++;
				else route_check();
			}
			//게임 끝! 
			if (Script_Buffer[i] == '_') return 1; 
			
			//버퍼출력
			printf("%c", Script_Buffer[i]); 
			i++;
			Sleep(script_speed); // 숫자를 바꿔서 빠르게도, 느리게도 출력이 가능함.
		}

		fgetc(stdin); //키입력 대기
	}
	fclose(Game_Script);
	return 0;
}

//분기점 체커
void route_check()
{
	retry:
	printf("\n당신의 선택은? >> ");
	scanf("%d", &my_select);
	Status.next_room = Route_Select(Status.room, my_select);

	//오타 예외 처리
	if (Status.next_room == 404)
	{
		printf("잘못된 선택입니다!\n\n");
		goto retry;
	}

	ENTER;
	WINDOW_CLEAR;
	printf("%d 번을 선택했습니다.\n", my_select);

	fgetc(stdin);
	Status.room = Status.next_room;
	Auto_Save(Status.room);
}

//게임 인트로 로고
void intro()
{
	int i = 0;
	if ((Game_Script = fopen("Script/Intro.txt", "r")) == NULL)
	{
		printf("인트로 파일이 존재하지 않습니다!!");
		fgetc(stdin);
		exit(1); 
	}

	while ((buffer = fgetc(Game_Script)) != EOF) printf("%c", buffer); //버퍼출력
}

//저장하기
void Auto_Save(int room_no)
{
	FILE *save_file;

	save_file = fopen("savedata_.bin", "wb");

	fwrite(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return;
}

//이어하기
int Load_Save()
{

	int room_no;

	FILE *save_file = fopen("savedata_.bin", "rb");

	fread(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return room_no;
}

//메인메뉴
int main_menu() 
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
			return Load_Save();
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
int ending_menu()
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

