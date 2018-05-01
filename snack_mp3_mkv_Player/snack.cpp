#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include <io.h> 

#pragma comment(lib,"winmm")
#include "Mmsystem.h"
#include "Digitalv.h"

typedef unsigned char Bool;

#define ADDRESS_TEMP "mp3folder\\"
#define FILE_SIZE 50
#define True 1
#define False 0
#define SIZE 15 // ��ũ�� �������� 0~30 ����
#define Win_Count SIZE*SIZE
#define SPEED 80


MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

Bool Boolean = True;

int dwID;
char ADDRESS[FILE_SIZE] = ADDRESS_TEMP;
char File_Name[FILE_SIZE] = { 0 };
int snack[SIZE][SIZE] = { 0 };
int EXIT = 0;
int j = 0;
int Gori = 1;
int R_x = 0, R_y = 0;
int Item_Speed = 0;
int Item_Slow_x = 999, Item_Slow_y = 999;

void MP3_SWITCH(char *Select, int *Up_Down, int count);
void SERCH_FUN();
void Sound_Play();
void gotoxy(int x, int y, int Count, char *s);
void SWITC(char *sz, int*x, int*y);
void result(int *x, int *y);
void fail(int a);
void STOP(char *Key, char Key_temp);
void Ignore(char *key);

void MP3_gotoxy(int x, int a) {
	COORD pos = { 0, x + 2 };

	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	//Ŀ�� ����ü ����, Ŀ���ǵβ� 1 ,Ŀ���� ����, ���� 

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	//52�� ����ü�� ������� ��ǥ��ġ�� Ŀ���� �����ٳ���
	if (a == 1)
		printf(" ");
	if (a == 2)
		printf("��");

}

void Random_Creat(int *x, int *y) {
	while (1) {
		*x = rand() % SIZE;
		*y = rand() % SIZE;
		if (snack[*x][*y] == 0)
			break;
	}
}

int main(void) {

	char key = 0, key_temp = 0;
	int x = 0, y = 0, Count = 3, i = 0;

	system("mode con:cols=100 lines=40");
	srand((unsigned)time(NULL));

	SERCH_FUN();
	Sound_Play();

	system("cls");
	//�⺻ �׵θ� ���� ����
	Random_Creat(&R_x, &R_y);
	gotoxy(R_x, R_y, 0, "��");
	for (i = 0; i < SIZE; i++) {
		gotoxy(i, -1, 0, "��");
		gotoxy(i, SIZE, 0, "��");
	}
	gotoxy(i, -1, 0, "��");
	gotoxy(i, SIZE, 0, "��");
	gotoxy(SIZE + 2 + 5, 1, 0, "W = ��");
	gotoxy(SIZE + 2, 2, 0, "A = ����  S = ��  D = ������");
	gotoxy(SIZE + 2 + 3, 3, 0, "�̿� Ű = �Ͻ�����");
	gotoxy(SIZE + 5, SIZE, -2, " ");
	//�⺻ �׵θ� ���� ��

	while (true) {
		snack[x][y] = Count++;
		if (_kbhit()) {   //Ű�� �Է½� 1, ���Է½� 0
			key = _getch();

			if (((key == 'w') && (key_temp == 's')) || ((key == 's') && (key_temp == 'w')) || ((key == 'a') &&
				(key_temp == 'd')) || ((key == 'd') && (key_temp == 'a'))) {
				// �ݴ�Ű�� ������ �ݴ�Ű �����ϴ� �Լ� ȣ��
				Ignore(&key);
			}

			if (key != 'w' && key != 'a' && key != 's' && key != 'd') {
				//�̿� Ű�� �Է½� �Ͻ����� �Լ� ȣ��
				STOP(&key, key_temp);
			}

			key_temp = key;
			SWITC(&key, &x, &y);
		}
		else {
			SWITC(&key, &x, &y);
		}
		gotoxy(x, y, Count, "��");
		result(&x, &y);

		if ((snack[x][y] != 0) && (key != 0))
			fail(3);

		if (EXIT != 0)
			return 0;
	}
	return 0;
}

void gotoxy(int x, int y, int Count, char *s) {
	COORD pos = { x * 2, y + 1 };

	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	//Ŀ�� ����ü ����,Ŀ���ǵβ� 1 ,Ŀ���� ����,����

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
	if (Count > 0) {
		//�� ������ ���� ���� ã�� ���ǹ�
		if (snack[x - 1][y] == Count - 1) {
			j++;
			return gotoxy(x - 1, y, Count - 1, "��");
		}
		else if (snack[x + 1][y] == Count - 1) {
			j++;
			return gotoxy(x + 1, y, Count - 1, "��");
		}
		else if (snack[x][y - 1] == Count - 1) {
			j++;
			return gotoxy(x, y - 1, Count - 1, "��");
		}
		else if (snack[x][y + 1] == Count - 1) {
			j++;
			return gotoxy(x, y + 1, Count - 1, "��");
		}
		if (Gori == j) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			if ((R_x == x) && (R_y == y)) {
				// �������� ��ġ�� �������� �����Ǵ���ġ�� �ʵ�
			}
			else {
				//�������� �����ʵǴ� ��ġ��� �������κ� �����
				printf("%s", "  ");
				snack[x][y] = 0;
				j = 0;
			}
		}
		j = 0;
	}
	if (Count == -1) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		printf("%d/%d�� ����\n", Gori, Win_Count);
	}
	if (Count == -2) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		printf("���� ����� : %s", File_Name);
	}
}

void SWITC(char *sz, int*x, int*y) {
	Sleep(SPEED - (Item_Speed*SIZE / 2)); // ���Ӽӵ�
	switch (*sz) {
	case 'w':
		*y = *y - 1;
		if (*y < 0)
			fail(1);
		break;
	case 'a':
		*x = *x - 1;
		if (*x < 0)
			fail(1);
		break;
	case 's':
		*y = *y + 1;
		if (*y > SIZE - 1)
			fail(1);
		break;
	case 'd':
		*x = *x + 1;
		if (*x > SIZE - 1)
			fail(1);
		break;

	default:
		break;
	}

}

void Ignore(char *key) {
	//�ݴ�Ű�� �Է½� �����ϰ� �Ѿ�� �Լ�
	switch (*key) {
	case 'w':
		*key = 's';
		break;
	case 'a':
		*key = 'd';
		break;
	case 's':
		*key = 'w';
		break;
	case 'd':
		*key = 'a';
		break;
	default:
		break;
	}
}

void STOP(char *Key, char Key_temp) {
	//�Ͻ�����!
	gotoxy(0, SIZE + 1, 0, "�Ͻ����� ���� �Դϴ� \n (w , a , s , d) Ű�� �ϳ��� ������ �Ͻ������� Ǯ���ϴ�");
	gotoxy(0, SIZE + 2, 0, "bgm �Ͻ�����(Ȥ�� ������ �÷���)�� ���ҽ�(p), �Ͻ����� ������ ���ҽ�(r) Ű�� �����ÿ�\n");
	printf("���������� ���� ���� ( ");
	if (Key_temp == 'w')
		printf("�� )");
	else if (Key_temp == 'a')
		printf("�� )");
	else if (Key_temp == 's')
		printf("�� )");
	else if (Key_temp == 'd')
		printf("�� )");

	while (1) {
		if (_kbhit()) {   //Ű�� �Է½� 1, ���Է½� 0
			*Key = _getch();
			if (*Key == 'w' || *Key == 'a' || *Key == 's' || *Key == 'd') {
				gotoxy(0, SIZE + 1, 0, "                         \n                                                       ");
				gotoxy(0, SIZE + 2, 0, "                                                                                     ");
				gotoxy(0, SIZE + 3, 0, "                            ");
				//�Ͻ����� �˸� ���� ������
				return;
			}
			else if (*Key == 'p') {
				mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY,
					(DWORD)(LPVOID)&m_mciPlayParms); /// �Ͻ�����
			}
			else if (*Key == 'r') {
				mciSendCommandW(dwID, MCI_RESUME, 0, NULL); /// �ٽý���
			}
		}
		Sleep(100);
	}
}

void result(int *x, int *y) {
	static int temp2 = 0, temp3 = 0;
	//temp�� ���� �Ǵ� �������� �ѹ��� �����ϰ��ϴ� �ӽ� ����
	if (*x == R_x && *y == R_y) {
		PlaySound("mp3folder\\levv.wav", NULL, SND_ASYNC);
		//���� ������ ���� �Ҹ�

		while (1) {
			Random_Creat(&R_x, &R_y);
			if (Item_Slow_x != R_x && Item_Slow_y != R_y)
				//��������ġ�� ������ġ�� ���������� �ݺ��� Ż��
				break;
		}
		Gori++, temp2 = 0, temp3 = 0;
		gotoxy(R_x, R_y, 0, "��");
		gotoxy(SIZE + 2, SIZE / 2, -1, 0);
		gotoxy(0, SIZE + 1, 0, "                ");
		//�������� �԰� �� ���� ���̸� ������ �۾��� ����� �뵵
	}
	if ((R_y % 14 == 0) && ((Item_Speed * 10) < SPEED - 50) && (temp3 == 0)) {
		//if���� % X ���̸� ������� �ӵ��� ������ Ȯ�� ����
		gotoxy(0, SIZE + 1, 0, "�ӵ��� �������ϴ�");
		Item_Speed++, temp3 = 1;
	}
	if (((R_x) % 16 == 0) && (temp2 == 0) && ((Item_Slow_x == 999) && (Item_Slow_y == 999))) {
		// if���� % X = ������ ����Ȯ�� ����
		temp2 = 1;
		while (1) {
			Random_Creat(&Item_Slow_x, &Item_Slow_y);
			if ((R_x != Item_Slow_x) || (R_y != Item_Slow_y))
				break;
			//������ġ�� ��������ġ�� �ٸ��� �ݺ��� Ż��
		}
		gotoxy(Item_Slow_x, Item_Slow_y, 0, "��");
		// ������ ����
	}

	if ((*x == Item_Slow_x) && (*y == Item_Slow_y)) {
		PlaySound("mp3folder\\lev.wav", NULL, SND_ASYNC);
		//������ ������ ���� �Ҹ�
		gotoxy(0, SIZE + 1, 0, "�ӵ��� �������ϴ�");
		Item_Speed--;
		Item_Slow_x = -5, Item_Slow_y = -5;
		// �ٸ� ��ġ��� �Ȱ�ġ���� �������� ������ �ӽ÷�-5�� �ʱ�ȭ
	}
	if (Gori == Win_Count) {
		fail(2);
		//������ ������ �����ҽ� ��������(�¸�)
	}
}

void fail(int a) {
	// ����Ǵ� ������ ���ϰ� ���ᰪ�� �����ϴ� �Լ�
	if (a == 1) {
		printf("���� �΋H�����ϴ�\n");
		EXIT = 1;
	}
	if (a == 2) {
		printf("�¸�!\n");
		EXIT = 2;
	}
	if (a == 3) {
		printf("��������");
		EXIT = 3;
	}
}

void SERCH_FUN() {
	//mp3 ����(�����) ã�� �Լ�
	_finddata_t fd_mp3;
	long handle_mp3;
	int result1 = 1, result2 = 1, count = 0, Key_Count = 0;
	char temp[FILE_SIZE][FILE_SIZE] = { 0 };
	char key = 0;

	handle_mp3 = _findfirst("mp3folder\\*.mp3", &fd_mp3);
	//��� mp3������ ã�´�
	if (handle_mp3 == -1)
		return;

	printf("���� ������ �ִ� ����\n\n");

	if (handle_mp3 != -1) {
		while (result1 > -1) {
			if (fd_mp3.size > 0 || fd_mp3.size != NULL) {
				//printf("  ���ϸ�: %s\n", fd_mp3.name, fd_mp3.size);  // ���ϸ� ���������
				printf("  ���ϸ�: %-15s  | ũ��: %-10dbit |\n", fd_mp3.name, fd_mp3.size); //ũ�⵵ ���������
				memcpy(temp[count], fd_mp3.name, FILE_SIZE);
				count++;
				//���� ���� ī��Ʈ
			}
			result1 = _findnext(handle_mp3, &fd_mp3);
		}
	}

	printf("\n�� %d�� �ֽ��ϴ�.\n", count);
	printf("================================================================");
	printf("\nW = ����    S = �Ʒ���\n");
	printf("Space bar = ����\n");
	printf("================================================================");
	while (Boolean) {
		Sleep(50);
		if (_kbhit()) {
			key = _getch();
			MP3_SWITCH(&key, &Key_Count, count);
		}
		else
			MP3_gotoxy(Key_Count, 2);
	}

	_findclose(handle_mp3);
	system("cls");
	memcpy(ADDRESS, ADDRESS_TEMP, FILE_SIZE);

	strcat_s(ADDRESS, temp[Key_Count]);
	strcat_s(File_Name, temp[Key_Count]);
	//printf("%s", ADDRESS); // ��� ���

	//printf("������ �뷡 ����:%s", temp[Key_Count]);
	//printf("// %s //", ADDRESS); // ����� ��ġ���� ���������

}

void MP3_SWITCH(char *Select, int *Up_Down, int count) {
	//����� ���� ����â gotoxy
	MP3_gotoxy(*Up_Down, 1);
	switch (*Select) {
	case 'w':
		*Up_Down = *Up_Down - 1;
		if (*Up_Down < 0) {
			*Up_Down = *Up_Down + 1;
			break;
		}
		break;

	case 's':
		*Up_Down = *Up_Down + 1;
		if (*Up_Down > count - 1) {
			*Up_Down = *Up_Down - 1;
			break;
		}
		break;
	case ' ':
		Boolean = False;
		break;
	}
	MP3_gotoxy(*Up_Down, 2);
}

void Sound_Play() {
	//mp3(�����) ��� �Լ�
	Boolean = True;
	char Serch[4] = { 0 };
	int temp = 0;
	mciOpen.lpstrDeviceType = "mpegvideo"; // mpegvideo : mp3, waveaudio : wav, avivideo : avi
										   //mciOpen.lpstrDeviceType = "waveaudio"; // mpegvideo : mp3, waveaudio : wav, avivideo : avi
	mciOpen.lpstrElementName = ADDRESS; // ���� ��� �Է�

	while (1) {
		if (ADDRESS[temp] == '\0') {
			break;
		}

		temp++;
	}
	memcpy(Serch, &ADDRESS[temp - 3], 4);

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	dwID = mciOpen.wDeviceID;

	if (strcmp(Serch - 1, "mp3")) {
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT,
			(DWORD)(LPVOID)&m_mciPlayParms);
	}
}