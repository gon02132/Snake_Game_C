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
#define SIZE 15 // 맵크기 조정가능 0~30 권장
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
	//커서 구조체 선언, 커서의두께 1 ,커서를 숨김, 실행 

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	//52의 구조체를 선언받은 좌표위치에 커서를 가져다놓음
	if (a == 1)
		printf(" ");
	if (a == 2)
		printf("※");

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
	//기본 테두리 세팅 시작
	Random_Creat(&R_x, &R_y);
	gotoxy(R_x, R_y, 0, "★");
	for (i = 0; i < SIZE; i++) {
		gotoxy(i, -1, 0, "─");
		gotoxy(i, SIZE, 0, "─");
	}
	gotoxy(i, -1, 0, "┐");
	gotoxy(i, SIZE, 0, "┘");
	gotoxy(SIZE + 2 + 5, 1, 0, "W = 위");
	gotoxy(SIZE + 2, 2, 0, "A = 왼쪽  S = 밑  D = 오른쪽");
	gotoxy(SIZE + 2 + 3, 3, 0, "이외 키 = 일시정지");
	gotoxy(SIZE + 5, SIZE, -2, " ");
	//기본 테두리 세팅 끝

	while (true) {
		snack[x][y] = Count++;
		if (_kbhit()) {   //키를 입력시 1, 비입력시 0
			key = _getch();

			if (((key == 'w') && (key_temp == 's')) || ((key == 's') && (key_temp == 'w')) || ((key == 'a') &&
				(key_temp == 'd')) || ((key == 'd') && (key_temp == 'a'))) {
				// 반대키를 누르면 반대키 무시하는 함수 호출
				Ignore(&key);
			}

			if (key != 'w' && key != 'a' && key != 's' && key != 'd') {
				//이외 키를 입력시 일시정지 함수 호출
				STOP(&key, key_temp);
			}

			key_temp = key;
			SWITC(&key, &x, &y);
		}
		else {
			SWITC(&key, &x, &y);
		}
		gotoxy(x, y, Count, "☆");
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
	//커서 구조체 선언,커서의두께 1 ,커서를 숨김,실행

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("%s", s);
	if (Count > 0) {
		//맨 마지막 꼬리 끝을 찾는 조건문
		if (snack[x - 1][y] == Count - 1) {
			j++;
			return gotoxy(x - 1, y, Count - 1, "☆");
		}
		else if (snack[x + 1][y] == Count - 1) {
			j++;
			return gotoxy(x + 1, y, Count - 1, "☆");
		}
		else if (snack[x][y - 1] == Count - 1) {
			j++;
			return gotoxy(x, y - 1, Count - 1, "☆");
		}
		else if (snack[x][y + 1] == Count - 1) {
			j++;
			return gotoxy(x, y + 1, Count - 1, "☆");
		}
		if (Gori == j) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			if ((R_x == x) && (R_y == y)) {
				// 지워지는 위치가 랜덤값이 생성되는위치면 않됨
			}
			else {
				//랜덤값이 생성않되는 위치라면 마지막부분 지우기
				printf("%s", "  ");
				snack[x][y] = 0;
				j = 0;
			}
		}
		j = 0;
	}
	if (Count == -1) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		printf("%d/%d개 남음\n", Gori, Win_Count);
	}
	if (Count == -2) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		printf("현재 배경음 : %s", File_Name);
	}
}

void SWITC(char *sz, int*x, int*y) {
	Sleep(SPEED - (Item_Speed*SIZE / 2)); // 게임속도
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
	//반대키를 입력시 무시하고 넘어가는 함수
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
	//일시정지!
	gotoxy(0, SIZE + 1, 0, "일시정지 상태 입니다 \n (w , a , s , d) 키중 하나를 누를시 일시정지가 풀립니다");
	gotoxy(0, SIZE + 2, 0, "bgm 일시정지(혹은 제거후 플레이)를 원할시(p), 일시정지 해제를 원할시(r) 키를 누르시오\n");
	printf("마지막으로 가던 방향 ( ");
	if (Key_temp == 'w')
		printf("↑ )");
	else if (Key_temp == 'a')
		printf("← )");
	else if (Key_temp == 's')
		printf("↓ )");
	else if (Key_temp == 'd')
		printf("→ )");

	while (1) {
		if (_kbhit()) {   //키를 입력시 1, 비입력시 0
			*Key = _getch();
			if (*Key == 'w' || *Key == 'a' || *Key == 's' || *Key == 'd') {
				gotoxy(0, SIZE + 1, 0, "                         \n                                                       ");
				gotoxy(0, SIZE + 2, 0, "                                                                                     ");
				gotoxy(0, SIZE + 3, 0, "                            ");
				//일시정지 알림 글자 지우기용
				return;
			}
			else if (*Key == 'p') {
				mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY,
					(DWORD)(LPVOID)&m_mciPlayParms); /// 일시정지
			}
			else if (*Key == 'r') {
				mciSendCommandW(dwID, MCI_RESUME, 0, NULL); /// 다시시작
			}
		}
		Sleep(100);
	}
}

void result(int *x, int *y) {
	static int temp2 = 0, temp3 = 0;
	//temp는 먹이 또는 아이템을 한번만 생성하게하는 임시 변수
	if (*x == R_x && *y == R_y) {
		PlaySound("mp3folder\\levv.wav", NULL, SND_ASYNC);
		//먹이 먹을때 나는 소리

		while (1) {
			Random_Creat(&R_x, &R_y);
			if (Item_Slow_x != R_x && Item_Slow_y != R_y)
				//아이템위치랑 먹이위치가 같지않으면 반복문 탈출
				break;
		}
		Gori++, temp2 = 0, temp3 = 0;
		gotoxy(R_x, R_y, 0, "★");
		gotoxy(SIZE + 2, SIZE / 2, -1, 0);
		gotoxy(0, SIZE + 1, 0, "                ");
		//아이템을 먹고난 뒤 다음 먹이를 먹을시 글씨를 지우는 용도
	}
	if ((R_y % 14 == 0) && ((Item_Speed * 10) < SPEED - 50) && (temp3 == 0)) {
		//if문의 % X 먹이를 먹을경우 속도가 빨리질 확률 설정
		gotoxy(0, SIZE + 1, 0, "속도가 빨라집니다");
		Item_Speed++, temp3 = 1;
	}
	if (((R_x) % 16 == 0) && (temp2 == 0) && ((Item_Slow_x == 999) && (Item_Slow_y == 999))) {
		// if문의 % X = 아이템 나올확률 설정
		temp2 = 1;
		while (1) {
			Random_Creat(&Item_Slow_x, &Item_Slow_y);
			if ((R_x != Item_Slow_x) || (R_y != Item_Slow_y))
				break;
			//먹이위치랑 아이템위치가 다를시 반복문 탈출
		}
		gotoxy(Item_Slow_x, Item_Slow_y, 0, "♠");
		// 아이템 생성
	}

	if ((*x == Item_Slow_x) && (*y == Item_Slow_y)) {
		PlaySound("mp3folder\\lev.wav", NULL, SND_ASYNC);
		//아이템 먹을때 나는 소리
		gotoxy(0, SIZE + 1, 0, "속도가 느려집니다");
		Item_Speed--;
		Item_Slow_x = -5, Item_Slow_y = -5;
		// 다른 위치들과 안겹치도록 아이템을 먹은뒤 임시로-5로 초기화
	}
	if (Gori == Win_Count) {
		fail(2);
		//꼬리가 조건을 만족할시 게임종료(승리)
	}
}

void fail(int a) {
	// 종료되는 이유를 말하고 종료값을 저장하는 함수
	if (a == 1) {
		printf("벽에 부딫혔습니다\n");
		EXIT = 1;
	}
	if (a == 2) {
		printf("승리!\n");
		EXIT = 2;
	}
	if (a == 3) {
		printf("꼬리밟음");
		EXIT = 3;
	}
}

void SERCH_FUN() {
	//mp3 파일(배경음) 찾는 함수
	_finddata_t fd_mp3;
	long handle_mp3;
	int result1 = 1, result2 = 1, count = 0, Key_Count = 0;
	char temp[FILE_SIZE][FILE_SIZE] = { 0 };
	char key = 0;

	handle_mp3 = _findfirst("mp3folder\\*.mp3", &fd_mp3);
	//모든 mp3파일을 찾는다
	if (handle_mp3 == -1)
		return;

	printf("현재 폴더에 있는 파일\n\n");

	if (handle_mp3 != -1) {
		while (result1 > -1) {
			if (fd_mp3.size > 0 || fd_mp3.size != NULL) {
				//printf("  파일명: %s\n", fd_mp3.name, fd_mp3.size);  // 파일만 보고싶을시
				printf("  파일명: %-15s  | 크기: %-10dbit |\n", fd_mp3.name, fd_mp3.size); //크기도 보고싶으면
				memcpy(temp[count], fd_mp3.name, FILE_SIZE);
				count++;
				//파일 갯수 카운트
			}
			result1 = _findnext(handle_mp3, &fd_mp3);
		}
	}

	printf("\n총 %d개 있습니다.\n", count);
	printf("================================================================");
	printf("\nW = 위쪽    S = 아래쪽\n");
	printf("Space bar = 선택\n");
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
	//printf("%s", ADDRESS); // 경로 출력

	//printf("선택한 노래 제목:%s", temp[Key_Count]);
	//printf("// %s //", ADDRESS); // 저장된 위치까지 보고싶을시

}

void MP3_SWITCH(char *Select, int *Up_Down, int count) {
	//배경음 고르는 선택창 gotoxy
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
	//mp3(배경음) 출력 함수
	Boolean = True;
	char Serch[4] = { 0 };
	int temp = 0;
	mciOpen.lpstrDeviceType = "mpegvideo"; // mpegvideo : mp3, waveaudio : wav, avivideo : avi
										   //mciOpen.lpstrDeviceType = "waveaudio"; // mpegvideo : mp3, waveaudio : wav, avivideo : avi
	mciOpen.lpstrElementName = ADDRESS; // 파일 경로 입력

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