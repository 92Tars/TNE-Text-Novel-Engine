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
	restart:
	intro(); 
	Status.room = main_menu();
	while(true) if (Game_Loop() == 1) break;
	
	if (ending_menu() == 0) goto restart;

	fgetc(stdin);
	return 0;
}

/*          ��Ʈ ���� ����
�� �κ��� �����ϸ� �ٸ� �̾߱⵵ ���� �� �ִ�.

	��Ʈ �Ŵ��� array ���� ��
	room == target_room, select == select_route �� �ش��ϴ� ��� �˻� �� return_room ��ȯ

*/
void Route_Init()
{
	//�ش��ϴ� ��� ��ũ��Ʈ�� ���� ��...
	if ((Game_Route = fopen("Script/Route.txt", "r")) == NULL)
	{
		printf(" Route.txt�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!");
		fgetc(stdin);
		exit(1); 
	}

	int i = 0;
	int Manager_index = 0; //Route_Manager index ��

	while ((buffer = fgetc(Game_Route)) != EOF) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�.
	{
		

		for (int i = 0; i < 50; i++) Route_Buffer[i] = NULL; //��� ���� �ʱ�ȭ

		FLUSH;

		while ((buffer = fgetc(Game_Route)) != '`') //'`'ǥ�ð� ���� �� ���� Route_Buffer�� ä���.
		{
			Route_Buffer[i] = buffer;
			i++;
		}
		Route_Buffer[i + 1] = NULL; //��ũ��Ʈ ���� ������
		
		FLUSH;

		while (Route_Buffer[i] != NULL) //��ũ��Ʈ ������ ������
		{
			//���� ��
			if (Route_Buffer[i] == '!')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Route_Manager[Manager_index].target_room = a + b + c;
				i += 4;
			}

			//�� ������
			if (Route_Buffer[i] == '*') 
			{
				int a;
				a = (Route_Buffer[i + 1] - 48);
				Route_Manager[Manager_index].select_route = a;
				i += 2;
			}

			//���Ե� ��
			if (Route_Buffer[i] == '>')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Route_Manager[Manager_index].return_room = a + b + c;
				i += 4;
				Manager_index++;
			}
		}

		fgetc(stdin); //Ű�Է� ���
	}
}

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

//���� �۵���
int Game_Loop()
{
	//�ؽ�Ʈ ���� �ҷ����� ��
	char File_name[32] = "Script/Script_";
	sprintf(&Status.room_char, "%d", Status.room);
	strcat(File_name, &Status.room_char);
	strcat(File_name, ".txt");
	//-----------------------

	//�ش��ϴ� ��� ��ũ��Ʈ�� ���� ��...
	if ((Game_Script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' ��� ���� ��� ��ũ��Ʈ�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!", File_name);
		fgetc(stdin);
		exit(1); 
	}

	int i = 0; // Flush init

	//��ũ��Ʈ ��º�
	while ((buffer = fgetc(Game_Script)) != EOF) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�.
	{
		for (int i = 0; i < 500; i++) Script_Buffer[i] = NULL; //��� ���� �ʱ�ȭ

		FLUSH;

		while ((buffer = fgetc(Game_Script)) != '`') //'`'ǥ�ð� ���� �� ���� Script_Buffer�� ä���.
		{
			Script_Buffer[i] = buffer;
			i++;
		}
		Script_Buffer[i + 1] = NULL; //��ũ��Ʈ ���� ������
		
		FLUSH;

		while (Script_Buffer[i] != NULL) //��ũ��Ʈ ������ ������
		{
			//��¼ӵ� ����
			if (Script_Buffer[i] == '>')
			{
				int a, b;
				a = (Script_Buffer[i + 1] - 48) * 10;
				b = Script_Buffer[i + 2] - 48;
				script_speed = a + b;
				i += 3;
				//printf("�ӵ��� %d �� �����߽��ϴ�. ", script_speed); //����� �ɼ�
			}

			//�б��� ����Ʈ
			if (Script_Buffer[i] == '$') 
			{
				if(Script_Buffer[i + 1] == '$') i++;
				else route_check();
			}
			//���� ��! 
			if (Script_Buffer[i] == '_') return 1; 
			
			//�������
			printf("%c", Script_Buffer[i]); 
			i++;
			Sleep(script_speed); // ���ڸ� �ٲ㼭 �����Ե�, �����Ե� ����� ������.
		}

		fgetc(stdin); //Ű�Է� ���
	}

	fclose(Game_Script);
	return 0;
}

//�б��� üĿ
void route_check()
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
	WINDOW_CLEAR;
	printf("%d ���� �����߽��ϴ�.\n", my_select);

	fgetc(stdin);
	Status.room = Status.next_room;
	Auto_Save(Status.room);
}

//���� ��Ʈ�� �ΰ�
void intro()
{
	int i = 0;
	if ((Game_Script = fopen("Script/Intro.txt", "r")) == NULL)
	{
		printf("��Ʈ�� ������ �������� �ʽ��ϴ�!!");
		fgetc(stdin);
		exit(1); 
	}

	while ((buffer = fgetc(Game_Script)) != EOF) printf("%c", buffer); //�������
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
		printf("�޴� ���� �Է��ϼ���. >> ");

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
			printf("�߸��� �����Դϴ�.");
			break;
		}
	}
}

//����ȭ��
int ending_menu()
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("������ �������ϴ�. �ٽ� �����Ͻðڽ��ϱ�?");
		DOUBLE_ENTER;
		printf("1. �ٽ� ����	2. ������");
		DOUBLE_ENTER;
		printf("�޴� ���� �Է��ϼ���. >> ");

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
			printf("�߸��� �����Դϴ�.");
			break;
		}
	}
}

