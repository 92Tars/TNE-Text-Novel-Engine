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

/*  [��Ʈ ���� �ʱ�ȭ]

	Script ���Ͽ� �ִ� Route.txt ������ ��� �ش� ������ �б����� vector ���·� ����

	! �� �ش� �� ��ȣ (room) 
	* �� ������ ��ȣ  (select)
	> �� �̵��Ǵ� �� ��ȣ (return)
*/
void routeInit()
{
	//�ش��ϴ� ��� ��ũ��Ʈ�� ���� ��...
	if ((game_route = fopen("Script/Route.txt", "r")) == NULL)
	{
		printf("Route.txt�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!");
		fgetc(stdin);
		exit(1); 
	}

	int i = 0;
	route_framework Temp_RF;

	while ((buffer = fgetc(game_route)) != EOF) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�.
	{
		FLUSH;
		while ((buffer = fgetc(game_route)) != '`') //'`'ǥ�ð� ���� �� ���� route_buffer�� ä���.
		{
			route_buffer[i] = buffer;
			i++;
		}
		route_buffer[i + 1] = '\0'; //��Ʈ ���� ������

		FLUSH;
		while (route_buffer[i] != '\0') //��Ʈ ������ ������
		{
			//���� ��
			if (route_buffer[i] == '!')
			{
				int a, b, c;
				a = (route_buffer[i + 1] - 48) * 100;
				b = (route_buffer[i + 2] - 48) * 10;
				c = (route_buffer[i + 3] - 48);
				Temp_RF.target_room = a + b + c;
				i += 4;
			}
			//�� ������
			if (route_buffer[i] == '*') 
			{
				int a;
				a = (route_buffer[i + 1] - 48);
				Temp_RF.select_route = a;
				i += 2;
			}
			//���Ե� ��
			if (route_buffer[i] == '>')
			{
				int a, b, c;
				a = (route_buffer[i + 1] - 48) * 100;
				b = (route_buffer[i + 2] - 48) * 10;
				c = (route_buffer[i + 3] - 48);
				Temp_RF.return_room = a + b + c;
				i += 4;

				route_list.push_back(Temp_RF); //route_list�� ������ push
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

/*	[���� �۵���]

	���� ��ũ��Ʈ�� �޾ƿ� ����Ѵ�.

	1. ���� �� ��ȣ�� �޾ƿ� Script ���� �ȿ��� ã�� �о���δ�. (��ũ��Ʈ�� ã�� �� ���� ��� ������ �����Ų��.)
	2. �극��ũ ����Ʈ�� ���� �� ���� script_buffer �迭�� ä���.
	3. Ư�� ��ɵ鿡 ���� ����ó���� �Ѵ�. (`, $, $$, _, __, >nn)
	4. ���۸� ����ϰ� script_sleep ��ŭ ������Ų��.
	
	gameLoop�� ���������� ����Ǹ� 1��, ������������ ����ȴٸ� 0�� ��ȯ�Ѵ�.
*/
int gameLoop()
{
	//�ؽ�Ʈ ���� �ҷ����� ��
	char File_name[32] = "Script/Script_";
	sprintf(&status.room_char, "%d", status.room);
	strcat(File_name, &status.room_char);
	strcat(File_name, ".txt");

	//�ش��ϴ� ��� ��ũ��Ʈ�� ���� ��...
	if ((game_script = fopen(File_name, "r")) == NULL)
	{
		printf(" '%s' ��� ���� ��� ��ũ��Ʈ�� �߰ߵ��� �ʾҽ��ϴ�! ������ ������ �� �����ϴ�!", File_name);

		breakFlagCheck();
		exit(1); 
	}

	//��ũ��Ʈ ��º�
	while ((buffer = fgetc(game_script) != EOF)) //End_Of_Line�� ���� �� ���� �ݺ��Ѵ�. (�ؽ�Ʈ ��ü ���)
	{   
        int i = 0; // Index init

		while ((buffer = fgetc(game_script)) != '`') //'`'ǥ�ð� ���� �� ���� script_buffer�� ä���.
		{
			script_buffer[i] = buffer;
			i++;
		}
		script_buffer[i + 1] = '\0'; //��ũ��Ʈ ���� ������
		
		FLUSH;

		while (script_buffer[i] != '\0') //��ũ��Ʈ ������ ������
		{
			//��¼ӵ� ����
			if (script_buffer[i] == '>')
			{
				int a, b;
				a = (script_buffer[i + 1] - 48) * 10;
				b = script_buffer[i + 2] - 48;
				script_speed = a + b;
				i += 3;
			}

			//�б��� ����Ʈ
			if (script_buffer[i] == '$') 
			{
				if(script_buffer[i + 1] == '$') i++;
				else 
				{
					i++;
					routeCheck();
				}
			}
			//���� ��! 
			if (script_buffer[i] == '_') 
			{
				if(script_buffer[i + 1] == '_') i++;
				else return 1;
			}
			//�������
			printf("%c", script_buffer[i]); 
			i++;
			Sleep(script_speed); // ���ڸ� �ٲ㼭 �����Ե�, �����Ե� ����� ������.
		}
		//Ű�Է� ���
		fflush(stdout);
		breakFlagCheck();
	}
	fclose(game_script);
	return 0;
}

/*	[�б��� �ҷ�����]

	��Ʈ �Ŵ��� array ���� ��
	room �� target_room, select �� select_route �� �ش��ϴ� ��� �˻� �� return_room ��ȯ
	���� �ش�Ǵ� ��Ұ� ���ٸ� -1 ��ȯ
	
*/
int routeSelect(int room, int select)
{
    for(int i = 0; i < route_list.size(); i++)
    {
       if(route_list[i].target_room == room && route_list[i].select_route == select) return route_list[i].return_room;
    }
    return -1;
}

/*	[�б��� ���ñ�]

	������ �Է¹��� �� routeSelect�� ���� ������ ����.
	���� �� ���� �ű�� ���¸� �����Ѵ�.
*/
void routeCheck()
{
	retry:
	printf("\n����� ������? >> ");
	scanf("%d", &my_select);

	status.next_room = routeSelect(status.room, my_select);
	//��Ÿ ���� ó��
	if (status.next_room == -1)
	{
		printf("�߸��� �����Դϴ�!\n\n");
		goto retry;
	}
	ENTER;
	WINDOW_CLEAR;

	printf("%d ���� �����߽��ϴ�.\n\n", my_select);
	status.room = status.next_room;
	autoSave(status.room);
}

/*	[���� ��Ʈ�� �ΰ�]

	Script ���Ͽ� �ִ� Intro.txt ������ ��� ���
*/
void intro()
{
	int i = 0;
	if ((game_script = fopen("Script/Intro.txt", "r")) == NULL)
	{
		printf("��Ʈ�� ������ �������� �ʽ��ϴ�!!");
		exit(1); 
	}
	while ((buffer = fgetc(game_script)) != EOF) printf("%c", buffer); //�������
}

/*	[�����ϱ�]

	savedata_.bin������ ���� ���� �ִ� �� ��ȣ�� �����Ѵ�
*/
void autoSave(int room_no)
{
	FILE *save_file;
	save_file = fopen("savedata_.bin", "wb");

	fwrite(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return;
}

/*	[�̾��ϱ�]

	savedata_.bin������ ���� ����Ǿ��ִ� �� ��ȣ�� ��ȯ�Ѵ�
	���� savedata_.bin�� ���ٸ� �ȳ� �� �����Ѵ�.
*/
int loadSave()
{
	int room_no;
	FILE *save_file = fopen("savedata_.bin", "rb");
	if (save_file == NULL) 
	{
		printf ("���̺� ������ �����ϴ�!!");
		breakFlagCheck();
		exit(1);
	}

	fread(&room_no, sizeof(int), 1, save_file);
	fclose(save_file);

	return room_no;
}

/*	[���θ޴�]

	�޴����� �Է¹޾� ����� �����Ų��.

	1. �����ϱ�
	2. �̾��ϱ�
	0. ����
*/
int mainMenu() 
{
	int a = 0;
	while (true)
	{
		DOUBLE_ENTER;
		printf("�޴� ���� �Է��ϼ���. >> ");

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
			printf("�߸��� �����Դϴ�.");
			break;
		}
	}
}

/*	[����ȭ��]

	������ ����� ��� �޴����� �Է¹޾� ����� �����Ų��.

	1. ����ȭ��
	2. ����
*/
int endingMenu()
{
	int a = 0;

	while (true)
	{
		DOUBLE_ENTER;
		printf("������ �������ϴ�. �ٽ� �����Ͻðڽ��ϱ�?");
		DOUBLE_ENTER;
		printf("1. ���� ȭ������	0. ����");
		DOUBLE_ENTER;
		printf("�޴� ���� �Է��ϼ���. >> ");

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
			printf("�߸��� �����Դϴ�.");
			break;
		}
	}
}

/*	[�Է½��� ����]
	
	�Է¹��۸� �ʱ�ȭ�ϰ� Ű �Է��� ���޴´�.
*/
void breakFlagCheck()
{
	FLUSH_BUFFER;
	while(!GetAsyncKeyState(VK_RETURN));
}
