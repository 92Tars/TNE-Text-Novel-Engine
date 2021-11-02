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
	
	printf("\n\n���� ��...");
	fgetc(stdin);
	return 0;
}

//���� �۵���
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
		printf(" '%s' ��� ���� ��� ��ũ��Ʈ�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!", File_name);
		fgetc(stdin);
		exit(1);
	}

	int i = 0; // Flush init

	while ((buffer = fgetc(Game_Script)) != EOF)//End_Of_Line�� ���� �� ���� ����Ѵ�.
	{
		for (int i = 0; i < 500; i++) Script_Buffer[i] = NULL; //��� ���� �ʱ�ȭ;

		FLUSH;
		while ((buffer = fgetc(Game_Script)) != '@') //'@'ǥ�ð� ���� �� ���� Script_Buffer�� ä���.
		{
			Script_Buffer[i] = buffer;
			i++;
		}
		Script_Buffer[i + 1] = NULL; //����ǥ��


		FLUSH;
		while (Script_Buffer[i] != NULL) //��ũ��Ʈ ������ ������
		{
			printf("%c", Script_Buffer[i]); //�������
			i++;
			Sleep(10);

			if (Script_Buffer[i] == '$')
			{
				retry:
				printf("\n����� ������? >> ");
				scanf("%d", &my_select);

				Status.next_room = Route_Select(Status.room, my_select);

				//��Ÿ ���� ó��
				if (Status.next_room == 404)
				{
					printf("�߸��� �����Դϴ�!\n\n");
					goto retry;
				}

				ENTER;

				printf("%d ���� �����߽��ϴ�.\n", my_select);

				fgetc(stdin);

				Status.room = Status.next_room;
				Auto_Save(Status.room);
			}

			if (Script_Buffer[i] == '_') return 1; //���� ��! 
		}

		fgetc(stdin); //Ű�Է� ���
	}

	fclose(Game_Script);
	return 0;
}

//��Ʈ ���� ����
//�� �κ��� �����ϸ� �ٸ� �̾߱⵵ ���� �� �ִ�.
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


//���� ��Ʈ�� �ΰ�
void intro()
{
	printf("������������Memorial: ������ ����������������������������������������������������������������������������������������������������������������������\n");
	printf("��                                                                                      ��\n");
	printf("��  **        **                                                                    **  ��\n");
	printf("��  ***      ***                                                                    **  ��\n");
	printf("��  ** *    * **    *****    **  ***  ***        ***     **  ***   **    *****      **  ��\n");
	printf("��  **  *  *  **   **   **   ** **  **   **    **   **   ** ** **             **    **  ��\n");
	printf("��  **   *    **  *********  ***    **    **  **     **  ***       **   ********    **  ��\n");
	printf("��  **        **   **        **     **    **   **   **   **        **  **   ** **   **  ��\n");
	printf("��  **        **    *****    **     **    **     ***     **        **   ******  **  **  ��\n");
	printf("��                                                                                      ��\n");
	printf("��                                                         ��Made by ����� & �ѱԸ���  ��\n");
	printf("��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
	DOUBLE_ENTER;
	printf("���������������������������������������������\n");
	printf("��                                                                                    ��\n");
	printf("��        1. �����ϱ�                  2. �̾��ϱ�                  0.������          ��\n");
	printf("��                                                                                    ��\n");
	printf("���������������������������������������������\n");
	DOUBLE_ENTER;
}

//�����ϱ�
void Auto_Save(int room_no)
{
	FILE *save_file;

	save_file = fopen("savedata_.bin", "wb");

	fwrite(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return;
}

//�̾��ϱ�
int Load_Save()
{

	int room_no;

	FILE *save_file = fopen("savedata_.bin", "rb");

	fread(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return room_no;
}

//���θ޴�
int main_menu() 
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("�޴� ���� �Է��ϼ���. >>");

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
			printf("�߸��� �����Դϴ�.");
			break;
		}
	}
}
