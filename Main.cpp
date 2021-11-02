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

#define _CRT_SECURE_NO_WARNINGS
#include "Game_Scene.h"

int main()
{
	intro();
	
	Status.room = main_menu();

	while(true) if (Game_Loop() == 1) break;
	
	printf("\n\n게임 끝...");
	fgetc(stdin);
	return 0;
}

//게임 작동부
int Game_Loop()
{
	//Game_Script_Changer
	char File_name[32] = "Script/Script_";
	sprintf(&Status.room_char, "%d", Status.room);
	strcat(File_name, &Status.room_char);
	strcat(File_name, ".txt");
	//-----------------------
	if ((Game_Script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' 라는 게임 대사 스크립트가 발견되지 않았습니다! 게임을 실행할 수 없습니다!", File_name);
		fgetc(stdin);
		exit(1);
	}

	int i = 0; // Flush init

	while ((buffer = fgetc(Game_Script)) != EOF)//End_Of_Line이 나올 때 까지 출력한다.
	{
		for (int i = 0; i < 500; i++) Script_Buffer[i] = NULL; //출력 버퍼 초기화;

		FLUSH;
		while ((buffer = fgetc(Game_Script)) != '@') //'@'표시가 나올 때 까지 Script_Buffer를 채운다.
		{
			Script_Buffer[i] = buffer;
			i++;
		}
		Script_Buffer[i + 1] = NULL; //끝냄표시


		FLUSH;
		while (Script_Buffer[i] != NULL) //스크립트 버퍼의 끝까지
		{
			printf("%c", Script_Buffer[i]); //버퍼출력
			i++;
			Sleep(10);

			if (Script_Buffer[i] == '$')
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

				printf("%d 번을 선택했습니다.\n", my_select);

				fgetc(stdin);

				Status.room = Status.next_room;
				Auto_Save(Status.room);
			}

			if (Script_Buffer[i] == '_') return 1; //게임 끝! 
		}

		fgetc(stdin); //키입력 대기
	}

	fclose(Game_Script);
	return 0;
}

//루트 파일 선택
//이 부분을 수정하면 다른 이야기도 만들 수 있다.
int Route_Select(int room, int select)
{
	//Room 1
	if (room == 0 && select == 4) return 20;
	else if (room == 0 && select == 1) return 11;
	else if (room == 0 && select == 2) return 12;
	else if (room == 0 && select == 3) return 13;
	else if ( (room == 11 && select == 1) || (room == 12 && select == 1) || (room == 13 && select == 1)) return 0;

	//Room 2
	if (room == 20 && select == 1) return 30;
	else if (room == 20 && select == 2) return 21;
	else if (room == 21 && select == 1) return 22;
	else if (room == 21 && select == 2) return 23;

	//Room 2_1
	if (room == 22 && select == 1) return 200;
	else if (room == 22 && select == 2) return 201;
	else if (room == 22 && select == 3) return 202;
	else if (room == 22 && select == 4) return 203;
	else if (room == 22 && select == 5) return 204;
	else if ( (room == 200 && select == 1) || (room == 201 && select == 1) || (room == 202 && select == 1) || (room == 203 && select == 1) ) return 22;

	//Room 2_2
	if (room == 204 && select == 1) return 210;
	else if (room == 204 && select == 2) return 205;

	//Room 3
	if (room == 30 && select == 1) return 40;
	else if (room == 30 && select == 2) return 31;

	//Room 4
	if (room == 40 && select == 1) return 41;
	else if (room == 40 && select == 2) return 50;
	
	//Room 5
	if (room == 50 && select == 1) return 60;
	else if (room == 50 && select == 2) return 51;

	//Room 6
	if (room == 60 && select == 1) return 70;
	else if (room == 60 && select == 2) return 61;

	//Room 7
	if (room == 70 && select == 1) return 80;
	else if (room == 70 && select == 2) return 71;

	//Room 8
	if (room == 80 && select == 1) return 81;
	else if (room == 80 && select == 2) return 90;

	//Room 8_1
	if (room == 81 && select == 2) return 82;
	else if (room == 82 && select == 1) return 83;
	else if (room == 83 && select == 1) return 84;
	else if (room == 84 && select == 2) return 85;
	else if ( (room == 81 && select == 1) || (room == 82 && select == 2) || (room == 83 && select == 2) || (room == 84 && select == 1)) return 90;

	//Room 9
	if (room == 90 && select == 1) return 100;
	else if (room == 90 && select == 2) return 91;

	//Room 10
	if (room == 100 && select == 1) return 101;
	else if (room == 100 && select == 2) return 110;

	//Room 11
	if (room == 110 && select == 1) return 111;
	else if (room == 110 && select == 2) return 120;

	//Room 12
	if (room == 120 && select == 1) return 130;
	else if (room == 120 && select == 2) return 121;

	//Exeption Filter
	else return 404;
}


//게임 인트로 로고
void intro()
{
	printf("┌────『Memorial: 마녀의 기억』─────────────────────────────────────────────────────────┐\n");
	printf("│                                                                                      │\n");
	printf("│  **        **                                                                    **  │\n");
	printf("│  ***      ***                                                                    **  │\n");
	printf("│  ** *    * **    *****    **  ***  ***        ***     **  ***   **    *****      **  │\n");
	printf("│  **  *  *  **   **   **   ** **  **   **    **   **   ** ** **             **    **  │\n");
	printf("│  **   *    **  *********  ***    **    **  **     **  ***       **   ********    **  │\n");
	printf("│  **        **   **        **     **    **   **   **   **        **  **   ** **   **  │\n");
	printf("│  **        **    *****    **     **    **     ***     **        **   ******  **  **  │\n");
	printf("│                                                                                      │\n");
	printf("│                                                         「Made by 어록희 & 한규리」  │\n");
	printf("└──────────────────────────────────────────────────────────────────────────────────────┘\n");
	DOUBLE_ENTER;
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■                                                                                    ■\n");
	printf("■        1. 새로하기                  2. 이어하기                  0.끝내기          ■\n");
	printf("■                                                                                    ■\n");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	DOUBLE_ENTER;
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
		printf("메뉴 값을 입력하세요. >>");

		scanf("%d", &a);

		switch (a)
		{
		case 1:
			return 0;
			break;
		case 2:
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
