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
		printf("Route.txt�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!");
		fgetc(stdin);
		exit(1); 
	}

	int i = 0;
	Route_Framework Temp_RF;

	while ((buffer = fgetc(Game_Route)) != EOF) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�.
	{
		FLUSH;

		while ((buffer = fgetc(Game_Route)) != '`') //'`'ǥ�ð� ���� �� ���� Route_Buffer�� ä���.
		{
			Route_Buffer[i] = buffer;
		    //printf("%c", buffer);
			i++;
		}
		Route_Buffer[i + 1] = '\0'; //��Ʈ ���� ������
		
		FLUSH;

		while (Route_Buffer[i] != '\0') //��Ʈ ������ ������
		{
			//���� ��
			if (Route_Buffer[i] == '!')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Temp_RF.target_room = a + b + c;
				i += 4;
				//printf("����� = %d \n", Temp_RF.target_room);
			}

			//�� ������
			if (Route_Buffer[i] == '*') 
			{
				int a;
				a = (Route_Buffer[i + 1] - 48);
				Temp_RF.select_route = a;
				i += 2;
				//printf("������ = %d\n", Temp_RF.select_route);
			}

			//���Ե� ��
			if (Route_Buffer[i] == '>')
			{
				int a, b, c;
				a = (Route_Buffer[i + 1] - 48) * 100;
				b = (Route_Buffer[i + 2] - 48) * 10;
				c = (Route_Buffer[i + 3] - 48);
				Temp_RF.return_room = a + b + c;
				i += 4;
				//printf("���Եɹ� = %d\n", Temp_RF.return_room);

				Route_List.push_back(Temp_RF);
				//printf("Ǫ��\n");
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
    //printf("���� ���� %s �Դϴ�.", File_name);


	//��ũ��Ʈ ��º�
	while ((buffer = fgetc(Game_Script) != EOF)) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�.
	{   
        int i = 0; // Flush init

		while ((buffer = fgetc(Game_Script)) != '`') //'`'ǥ�ð� ���� �� ���� Script_Buffer�� ä���.
		{
			Script_Buffer[i] = buffer;
			i++;
		}
		Script_Buffer[i + 1] = '\0'; //��ũ��Ʈ ���� ������
		
		FLUSH;

		while (Script_Buffer[i] != '\0') //��ũ��Ʈ ������ ������
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

