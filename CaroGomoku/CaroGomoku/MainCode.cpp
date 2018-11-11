#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <sstream> 
#include <conio.h>
#include <fstream>
#include <string>


 using namespace std;
 struct Pos2D {
	 int x;
	 int y;
 };
//Variables
 string P1, P2;
 int turn = 1, modePlay = 0, maxX = 12, maxY = 27;
 bool splashscreenExist = false, isExit = false, endGame = false;
 Pos2D curPos, initPos, boardPos, coordCursor, playerLastMove, loginPos;
 Pos2D coord[30][30];
 int caroTable[30][30], predictTable[30][30];

 void main();
 //Utitlities
void textColor(short num) {
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}
void gotoXY(Pos2D cur)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { cur.x, cur.y };
	SetConsoleCursorPosition(h, c);
}
void CreateSplashScreen(int color) {
	textColor(color);
	if (!splashscreenExist) {
		//Letter C
		gotoXY(curPos);
		cout << "OOOOOOOOOOOOOO";
		for (int i = 1; i < 11; i++) {
			curPos.y += 1;
			gotoXY(curPos);
			cout << "O";
		}
		gotoXY(curPos);
		cout << "OOOOOOOOOOOOOO";
		//Letter A
		curPos.x += 28;
		curPos.y -= 11;
		for (int i = 0; i < 11; i++) {
			curPos.x -= 1;
			curPos.y += 1;
			gotoXY(curPos);
			cout << "X";
			for (int j = 1; j <= 2 * i - 1; j++) {
				cout << " ";
			}
			if (i != 0)
				cout << "X";
		}
		curPos.x += 4;
		curPos.y -= 4;
		gotoXY(curPos);
		cout << "XXXXXXXXXXXX";
		//Letter R
		curPos.x += 22;
		curPos.y = initPos.y;
		
		for (int i = 1; i < 12; i++) {
			gotoXY(curPos);
			cout << "O";
			curPos.y++;
		}
		curPos.y = initPos.y;
		gotoXY(curPos);
		cout << "OOOOOOOOOOOOOOO";
		curPos.x += 14;
		for (int i = 1; i < 5; i++) {
			gotoXY(curPos);
			cout << "O";
			curPos.y++;
		}
		//Len
		curPos.x -= 14;
		curPos.y = initPos.y + 6;
		gotoXY(curPos);
		cout << "OOOOOOOOOOOO";
		curPos.y = initPos.y + 5;
		curPos.x += 12;
		gotoXY(curPos);
		cout << "O";
		curPos.y -= 1;
		curPos.x += 1;
		gotoXY(curPos);
		cout << "O";
		curPos.y -= 1;
		curPos.x += 1;
		gotoXY(curPos);
		cout << "O";
		//Xuong
		curPos.y = initPos.y + 7;
		curPos.x -= 2;
		gotoXY(curPos);
		cout << "O";
		curPos.y += 1;
		curPos.x += 1;
		gotoXY(curPos);
		cout << "O";
		curPos.y += 1;
		curPos.x += 1;
		gotoXY(curPos);
		cout << "O";
		curPos.y += 1;
		curPos.x += 1;
		gotoXY(curPos);
		cout << "O";		
		
		//Letter O
		curPos.y = initPos.y;
		curPos.x += 10;
		gotoXY(curPos);
		cout << "XXXXXXXXXXXXXXXX";
		curPos.x -= 1;
		curPos.y += 1;
		for (int i = 1; i < 10; i++) {
			gotoXY(curPos);
			cout << "X                X";
			curPos.y++;
		}
		curPos.x += 1;
		gotoXY(curPos);
		cout << "XXXXXXXXXXXXXXXX";
	}
}
void Login(int modePlay) {
	textColor(10);
	system("cls");
	if (modePlay == 2) {
		gotoXY(loginPos);
		cout << "Username 1: ";
		loginPos.y += 5;
		gotoXY(loginPos);
		cout << "Username 2: ";
		while (1 > 0) {

		}
	}
	else {
		gotoXY(loginPos);
		cout << "Username: ";
		cin >> P1;
	}

}
void LoadStatistics(int win, int lose, int match) {
	int lineCount = 0;
	fstream f;
	f.open("Save.txt", ios::in);
	string data;

	string line;
	while (!f.eof())
	{
		getline(f, line);
		data = line;
		stringstream tmp(data);
		switch (lineCount)
		{
		case 0:
			tmp >> win;
			break;
		case 1:
			tmp >> lose;
			break;
		case 2:
			tmp >> match;
			break;
		}
		lineCount++;
	}
	f.close();
}
void SaveStatistics(string PlayerName, bool isWin) {
	int winCount = 0, loseCount = 0, matchPlayed = 0;
	LoadStatistics(winCount, loseCount, matchPlayed);
	fstream f;
	f.open(PlayerName + ".txt", ios::out);
	string data = "";
	if (isWin) {
		winCount++;
	}
	else
		loseCount++;
	matchPlayed++;
	stringstream ss;
	ss << winCount;
	ss << loseCount;
	ss << matchPlayed;
	data = ss.str();
	f << data;
	f.close();
}

//Check Win Lose
bool CheckWinDoc(int turn, Pos2D signPos) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//Len
	while (x > 0) {
		x--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//Xuong
	x = signPos.x; y = signPos.y;
	while (x < 12) {
		x++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == 5)
		return true;
	else
		return false;
}
bool CheckWinNgang(int turn, Pos2D signPos) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//Trai
	while (y > 0) {
		y--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//Phai
	x = signPos.x; y = signPos.y;
	while (y < 27) {
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == 5)
		return true;
	else
		return false;
}
bool CheckWinSac(int turn, Pos2D signPos) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//LenPhai
	while (x > 0) {
		x--;
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//XuongTrai
	x = signPos.x; y = signPos.y;
	while (x < 12) {
		x++;
		y--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == 5)
		return true;
	else
		return false;
}
bool CheckWinHuyen(int turn, Pos2D signPos) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//XuongPhai
	while (x > 0) {
		x++;
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//LenTrai
	x = signPos.x; y = signPos.y;
	while (x < 12) {
		x--;
		y--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == 5)
		return true;
	else
		return false;
}
bool CheckBotWin(Pos2D botCoord) {
	if (CheckWinDoc(2, botCoord))
		return true;
	if (CheckWinNgang(2, botCoord))
		return true;
	if (CheckWinSac(2, botCoord))
		return true;
	if (CheckWinHuyen(2, botCoord))
		return true;
}


//First Scene Methods
void Loading() {
	int percentLoading = 0;
	Pos2D tempPos, barPos;
	curPos.x = initPos.x + 27;
	curPos.y = initPos.y + 15;
	gotoXY(curPos);
	while (1 > 0) {
		tempPos = curPos;
		tempPos.x += 8;
		gotoXY(tempPos);
		percentLoading += 10;
		textColor(12);
		cout << "LOADING " << percentLoading << " %";
		if (percentLoading == 100) {
			barPos.x += 4;
			barPos.y += 2;
			gotoXY(barPos);
			cout << "Press R to continue";
			break;
		}
		barPos.x = tempPos.x - 7;
		barPos.y = tempPos.y + 2;
		gotoXY(barPos);
		textColor(5);
		cout << "                 ";
		gotoXY(barPos);
		for (int i = 0; i < percentLoading / 10; i++) {
			textColor(18);
			cout << "   ";
		}
		Sleep(1000);
	}
}
void SetFont() {
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX font = new CONSOLE_FONT_INFOEX();
	font->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(cons, 0, font);
	font->dwFontSize.X = 72;
	font->dwFontSize.Y = 72;
	//font->FontWeight = 22;
	//font->FontFamily = FF_DECORATIVE;
	SetCurrentConsoleFontEx(cons, 0, font);
}

//Second Scene Methods
void ChangeToGameplayScreen() {
	while (1 != 0) {
		if (GetKeyState('R')) {
			system("cls");
			break;
		}
	}
}
void drawBoard(int m, int n) {
	system("cls");
	textColor(2);
	printf("%c", 218);
	for (int i = 0; i < n - 1; i++) {
		printf("%c%c%c%c", 196, 196, 196, 194);
	}
	printf("%c%c%c", 196, 196, 196);
	printf("%c", 191);
	printf("%\n");

	for (int i = 0; i < m - 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			printf("%c   ", 179);
		}
		printf("\n%c", 195);
		for (int j = 0; j < n - 1; j++) {
			printf("%c%c%c%c", 196, 196, 196, 197);
		}
		printf("%c%c%c", 196, 196, 196);
		printf("%c\n", 180);
	}
	for (int j = 0; j < n + 1; j++) {
		printf("%c   ", 179);
	}
	printf("\n%c", 192);
	for (int i = 0; i < n - 1; i++) {
		printf("%c%c%c%c", 196, 196, 196, 193);
	}
	printf("%c%c%c", 196, 196, 196);
	printf("%c", 217);
}
void SetCoordForArray() {
	int x = 2, y = 1;
	for (int i = 1; i <= 12; i++) {
		for (int j = 1; j <= 27; j++) {
			coord[i][j].x = x;
			coord[i][j].y = y;
			x += 4;
		}
		x = 2;
		y += 2;
	}
}
void PlayerMove();
void BotMove(int hard, Pos2D playerMovePos);

//Menu
void Menu() {
	Pos2D selectPos, menuPos;
	int check = 0;
	menuPos.x = 43; menuPos.y = 10;
	selectPos.x = menuPos.x - 5;
	selectPos.y = menuPos.y;
	textColor(10);
	gotoXY(menuPos);
	cout << "1. Player vs Computer";
	menuPos.y += 3;
	gotoXY(menuPos);
	cout << "2. Player vs Player";
	textColor(1);
	gotoXY(selectPos);
	cout << "->";
	while (1 > 0) {	
		check = _getch();
		switch (check) {
		case 72: //Up
			if (selectPos.y == 13) {
				gotoXY(selectPos);
				cout << "  ";
				selectPos.y -= 3;
			}
			break;

		case 80: //Down
			if (selectPos.y == 10) {
				gotoXY(selectPos);
				cout << "  ";
				selectPos.y += 3;
			}
			break;
			//Write menu
		}
		gotoXY(selectPos);
		cout << "->";
		if (GetAsyncKeyState(VK_RETURN)) {
			if (selectPos.y == 10) {
				//PVC
				modePlay = 1;
				return;
			}
			else {
				//PVP
				modePlay = 2;
				return;
			}
		}
	}
}

void HelpMenuInGame() {
	textColor(7);
	Pos2D helpPos;
	helpPos.x = 5;
	helpPos.y = 2 * maxX + 2;
	gotoXY(helpPos);
	cout << "Use arrow key to move            Space - Go            S - Save              R - Reset";
}

//For Gameplay
void Initialize() {

	endGame = false;
	initPos.x = 12; initPos.y = 5;
	loginPos.x = 43; loginPos.y = 10;
	//Clear screen
	system("cls");
	cout << "Caro Game developed by Nguyen Gia Huy, v1.0";
	//SplashScreen 
	for (int i = 0; i < 2; i++) {
		curPos = initPos;
		CreateSplashScreen(i);
		Sleep(1000);
	} 
	//Loading
	Loading();
	//ChangeToGamePlay
	ChangeToGameplayScreen();
	//Menu
	Menu();
	system("cls");
	Login(modePlay);
	//NewOrLoad();
	//DrawBoard
	boardPos.x = 10, boardPos.y = 1;
	drawBoard(maxX, maxY);
	SetCoordForArray();
	//MenuHelpInGame
	HelpMenuInGame();
	//Initial Value For CaroTable
	curPos.x = 2; curPos.y = 1;
	coordCursor.x = 1, coordCursor.y = 1;
	for (int i = 1; i < maxX; i++)
		for (int j = 1; j < maxY; j++)
			caroTable[i][j] = 0;
}

void PlayPVP() {
	while (1 > 0) {
		PlayerMove();
		if (GetAsyncKeyState('R')) {		//Reset
			main();
		}
	}
}

void PlayPVC() {
	while (1 > 0) {
		if (!endGame)
			PlayerMove();
		if (!endGame) 
			BotMove(2, playerLastMove);
		if (GetAsyncKeyState('R')) {		//Reset
			main();
		}
	}
}

void Win(int turn) {
	endGame = true;
	system("cls");
	Pos2D winPos; 
	winPos.x = 48; winPos.y = 12;
	for (int i = 1; i < 10; i++) {
		gotoXY(winPos);
		if (turn == 1) {
			if (modePlay == 2) {
				cout << "PLAYER 1 WIN";
				SaveStatistics(P1, true);
				SaveStatistics(P2, false);
			}
			else {
				cout << "PLAYER WIN";
				SaveStatistics(P1, true);
			}
		}
		else {
			if (modePlay == 2) {
				cout << "PLAYER 2 WIN";
				SaveStatistics(P1, false);
				SaveStatistics(P2, true);
			}
			else {
				cout << "COMPUTER WIN";
				SaveStatistics(P1, false);
			}
		}
	}

}

void Save() {
	fstream f;
	f.open("Save.txt", ios::out);
	string data;
	for (int i = 1; i <= 12; i++) {
		for (int j = 1; j <= 27; j++) {
			data += caroTable[i][j] + '0';
		}
		data += "\n";
	}
	f << data;
	f.close();
}
void Load() {
	int x = 1, y = 1;
	int i;
	fstream f;
	f.open("Save.txt", ios::in);
	string data;

	string line;
	while (!f.eof())
	{
		i = 0;
		getline(f, line);
		data = line;
		while (data[i] != '\0') {
			caroTable[x][y] = data[i] - '0';
			i++;
			y++;
		}
		y = 1;
		x++;
	}
	f.close();
}
void DisplayMapLoad() {
	for (int i = 1; i < maxX; i++) 
		for (int j = 1; j < maxY; j++)
			if (caroTable[i][j] == 1) {
				gotoXY(coord[i][j]);
				cout << "X";
			}
			else {
				gotoXY(coord[i][j]);
				cout << "O";
			}
}



void main() {
	//Variables 
	
	Initialize();
	if (modePlay == 2)
		PlayPVP();
	else
		PlayPVC();
	return;
}



void PlayerMove() {
	bool isPlaying = false;
	if (turn > 0) {
		int check = 0;
		while (1 > 0) {
			check = _getch();
				switch (check) {
			case 72: //Up
				if (coordCursor.x > 1)
					coordCursor.x -= 1;
				break;
			case 80: //Down
				if (coordCursor.x < 12)
					coordCursor.x += 1;
				break;
			case 75: //Left
				if (coordCursor.y > 1)
					coordCursor.y -= 1;
				break;
			case 77: //Right
				if (coordCursor.y < 27)
					coordCursor.y += 1;
				break;
			}
			playerLastMove.x = coordCursor.x; playerLastMove.y = coordCursor.y;
			curPos = coord[coordCursor.x][coordCursor.y];
			gotoXY(curPos);
			if (isPlaying) {
				if (GetAsyncKeyState(' ') && caroTable[coordCursor.x][coordCursor.y] == 0) {  //PlayerGo
					if (turn == 1) {
						cout << "X";
						caroTable[coordCursor.x][coordCursor.y] = 1;
						if (CheckWinDoc(1, coordCursor))
							Win(1);
						if (CheckWinNgang(1, coordCursor))
							Win(1);
						if (CheckWinSac(1, coordCursor))
							Win(1);
						if (CheckWinHuyen(1, coordCursor))
							Win(1);
					}
					else {
						cout << "O";
						caroTable[coordCursor.x][coordCursor.y] = 2;
						if (CheckWinDoc(2, coordCursor))
							Win(2);
						if (CheckWinNgang(2, coordCursor))
							Win(2);
						if (CheckWinSac(2, coordCursor))
							Win(2);
						if (CheckWinHuyen(2, coordCursor))
							Win(2);
					}
					gotoXY(curPos);
					if (turn == 1)
						turn = 2;
					else
						turn = 1;
					break;
				}
			}
			else {
				isPlaying = true;
			}
			if (GetAsyncKeyState('S')) {		//Save
				Save();
			}
			if (GetAsyncKeyState('R')) {		//Reset
				main();
			}
		}
	}
}

void BotWrite(int x, int y) {
	gotoXY(coord[x][y]);
	cout << "O";
	caroTable[x][y] = 2;
	gotoXY(coord[x][y]);
	Pos2D botCoord; 
	botCoord.x = x; botCoord.y = y;
	if (CheckBotWin(botCoord)) {
		system("cls");
		Win(2);
	}
}
void BotGoRandom(Pos2D playerMovePos) {
	//Random
	//8 directions
	Pos2D botCoord;
	for (int i = 1; i < 7; i++) {
		if (playerMovePos.x > i && playerMovePos.y > i)
			if (caroTable[playerMovePos.x - i][playerMovePos.y - i] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x - i][playerMovePos.y - i]);
				cout << "O";
				caroTable[playerMovePos.x - i][playerMovePos.y - i] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.x > i)
			if (caroTable[playerMovePos.x - i][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				gotoXY(coord[playerMovePos.x - i][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x - i][playerMovePos.y] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.x > i && playerMovePos.y < maxY - i + 1 )
			if (caroTable[playerMovePos.x - i][playerMovePos.y + i] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x - i][playerMovePos.y + i]);
				cout << "O";
				caroTable[playerMovePos.x - i][playerMovePos.y + i] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.y > i)
			if (caroTable[playerMovePos.x][playerMovePos.y - i] == 0) {
				botCoord = playerMovePos;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y - i]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y - i] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x][playerMovePos.y + i] == 0) {
				botCoord = playerMovePos;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y + i]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y + i] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.x < maxX && playerMovePos.y > i)
			if (caroTable[playerMovePos.x + i][playerMovePos.y - i] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x + i][playerMovePos.y - i]);
				cout << "O";
				caroTable[playerMovePos.x + i][playerMovePos.y - i] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.x < maxX - i + 1)
			if (caroTable[playerMovePos.x + i][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				gotoXY(coord[playerMovePos.x + i][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x + i][playerMovePos.y] = 2;
				CheckBotWin(botCoord);
				return;
			}
		if (playerMovePos.x < maxX - i + 1 && playerMovePos.y < maxY - i + 1)
			if (caroTable[playerMovePos.x + i][playerMovePos.y + i] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x + i][playerMovePos.y + i]);
				cout << "O";
				caroTable[playerMovePos.x + i][playerMovePos.y + i] = 2;
				CheckBotWin(botCoord);
				return;
			}
	}
}

void BotMove(int hard, Pos2D playerMovePos) {
	turn = 1;
	Pos2D botCoord;
	botCoord.x = 1; botCoord.y = 1;
	int max = 0, contf = 0, conts = 0, cont = 0;
	switch (hard) {
	case 1:					//Easy
		//8 directions
		if (playerMovePos.x > 1 && playerMovePos.y > 1)
			if (caroTable[playerMovePos.x - 1][playerMovePos.y - 1] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x - 1][playerMovePos.y - 1]);
				cout << "O";
				caroTable[playerMovePos.x - 1][playerMovePos.y - 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.x > 1)
			if (caroTable[playerMovePos.x - 1][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				gotoXY(coord[playerMovePos.x - 1][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x - 1][playerMovePos.y] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.x > 1 && playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x - 1][playerMovePos.y + 1] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x - 1][playerMovePos.y + 1]);
				cout << "O";
				caroTable[playerMovePos.x - 1][playerMovePos.y + 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.y > 1)
			if (caroTable[playerMovePos.x][playerMovePos.y - 1] == 0) {
				botCoord = playerMovePos;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y - 1]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y - 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x][playerMovePos.y + 1] == 0) {
				botCoord = playerMovePos;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y + 1]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y + 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.x < maxX && playerMovePos.y > 1)
			if (caroTable[playerMovePos.x + 1][playerMovePos.y - 1] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x + 1][playerMovePos.y - 1]);
				cout << "O";
				caroTable[playerMovePos.x + 1][playerMovePos.y - 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.x < maxX)
			if (caroTable[playerMovePos.x + 1][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				gotoXY(coord[playerMovePos.x + 1][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x + 1][playerMovePos.y] = 2;
				CheckBotWin(botCoord);
				break;
			}
		if (playerMovePos.x < maxX && playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x + 1][playerMovePos.y + 1] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x + 1][playerMovePos.y + 1]);
				cout << "O";
				caroTable[playerMovePos.x + 1][playerMovePos.y + 1] = 2;
				CheckBotWin(botCoord);
				break;
			}
		BotGoRandom(playerMovePos);
		CheckBotWin(botCoord);
		break;
	case 2:					//Normal
		//Find 3 point continuos
		//8 directions
		Pos2D tempPos = playerMovePos;
		//Huyen
		contf = 0; conts = 0;
		while (tempPos.x > 1 && tempPos.y > 1) {
			tempPos.x -= 1; tempPos.y -= 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				contf++;
			else
				break;
		} //Continuos first
		tempPos = playerMovePos;
		while (tempPos.x < maxX && tempPos.y < maxY) {
			tempPos.x += 1; tempPos.y += 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				conts++;
			else
				break;
		} //Continuos Second
		cont = contf + conts;
		if (cont >= 2) {
			if (contf >= 2) {
				if (playerMovePos.x < maxX && playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.x > 3 && playerMovePos.y > 3) {
					if (caroTable[playerMovePos.x - 3][playerMovePos.y - 3] == 0) {
						BotWrite(playerMovePos.x - 3, playerMovePos.y - 3);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 2) {
				if (playerMovePos.x > 1 && playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.x < maxX - 2 && playerMovePos.y < maxY - 2) {
					if (caroTable[playerMovePos.x + 3][playerMovePos.y + 3] == 0) {
						BotWrite(playerMovePos.x + 3, playerMovePos.y + 3);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (playerMovePos.x > 2 && playerMovePos.y > 2) {
				if (caroTable[playerMovePos.x - 2][playerMovePos.y - 2] == 0) {
					BotWrite(playerMovePos.x - 2, playerMovePos.y - 2);
					return;
				}
			}
			if (playerMovePos.x < maxX - 1 && playerMovePos.y < maxY - 1) {
				if (caroTable[playerMovePos.x + 2][playerMovePos.y + 2] == 0) {
					BotWrite(playerMovePos.x + 2, playerMovePos.y + 2);
					return;
				}
			}
			BotGoRandom(playerMovePos);
			CheckBotWin(botCoord);
			return;
		}
		else 
		if (cont >= 1) {
			if (contf >= 1) {
				if (playerMovePos.x < maxX && playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.x > 2 && playerMovePos.y > 2) {
					if (caroTable[playerMovePos.x - 2][playerMovePos.y - 2] == 0) {
						BotWrite(playerMovePos.x - 2, playerMovePos.y - 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 1) {
				if (playerMovePos.x > 1 && playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.x < maxX - 1 && playerMovePos.y < maxY - 2) {
					if (caroTable[playerMovePos.x + 2][playerMovePos.y + 2] == 0) {
						BotWrite(playerMovePos.x + 2, playerMovePos.y + 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			BotGoRandom(playerMovePos);
			CheckBotWin(botCoord);
			return;
		}


		//Doc
		contf = 0; conts = 0;
		tempPos = playerMovePos;
		while (tempPos.x > 1) {
			tempPos.x -= 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				contf++;
			else
				break;
		}
		tempPos = playerMovePos;
		while (tempPos.x < maxX) {
			tempPos.x += 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				conts++;
			else
				break;
		}
		cont = contf + conts;
		if (cont >= 2) {
			if (contf >= 2) {
				if (playerMovePos.x < maxX) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y);
						return;
					}
				}
				if (playerMovePos.x > 3) {
					if (caroTable[playerMovePos.x - 3][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x - 3, playerMovePos.y);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 2) {
				if (playerMovePos.x > 1) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y);
						return;
					}
				}
				if (playerMovePos.x < maxX - 2) {
					if (caroTable[playerMovePos.x + 3][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x + 3, playerMovePos.y);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (playerMovePos.x > 2) {
				if (caroTable[playerMovePos.x - 2][playerMovePos.y] == 0) {
					BotWrite(playerMovePos.x - 2, playerMovePos.y);
					return;
				}
			}
			if (playerMovePos.x < maxX - 1) {
				if (caroTable[playerMovePos.x + 2][playerMovePos.y] == 0) {
					BotWrite(playerMovePos.x + 2, playerMovePos.y);
					return;
				}
			}
			BotGoRandom(playerMovePos);
			return;
		}
		else
		if (cont >= 1) {
			if (contf >= 1) {
				if (playerMovePos.x < maxX) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y);
						return;
					}
				}
				if (playerMovePos.x > 2) {
					if (caroTable[playerMovePos.x - 2][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x - 2, playerMovePos.y);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 1) {
				if (playerMovePos.x > 1) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y);
						return;
					}
				}
				if (playerMovePos.x < maxX - 1) {
					if (caroTable[playerMovePos.x + 2][playerMovePos.y] == 0) {
						BotWrite(playerMovePos.x + 2, playerMovePos.y);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			BotGoRandom(playerMovePos);
			return;
		}

		//Sac
		contf = 0; conts = 0;
		tempPos = playerMovePos;
		while (tempPos.x > 1 && tempPos.y < maxY) {
			tempPos.x -= 1; tempPos.y += 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				contf++;
			else
				break;
		}
		tempPos = playerMovePos;
		while (tempPos.x < maxX && tempPos.y > 1) {
			tempPos.x += 1; tempPos.y -= 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				conts++;
			else
				break;
		}
		cont = contf + conts;
		if (cont >= 2) {
			if (contf >= 2) {
				if (playerMovePos.x < maxX && playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.x > 2 && playerMovePos.y < maxY - 1) {
					if (caroTable[playerMovePos.x - 2][playerMovePos.y + 2] == 0) {
						BotWrite(playerMovePos.x - 2, playerMovePos.y + 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 2) {
				if (playerMovePos.x > 1 && playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.x < maxX - 2 && playerMovePos.y > 3) {
					if (caroTable[playerMovePos.x + 3][playerMovePos.y - 3] == 0) {
						BotWrite(playerMovePos.x + 3, playerMovePos.y - 3);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (playerMovePos.x > 2 && playerMovePos.y < maxY - 1) {
				if (caroTable[playerMovePos.x - 2][playerMovePos.y + 2] == 0) {
					BotWrite(playerMovePos.x - 2, playerMovePos.y + 2);
					return;
				}
			}
			if (playerMovePos.x < maxX - 1 && playerMovePos.y > 2) {
				if (caroTable[playerMovePos.x + 2][playerMovePos.y - 2] == 0) {
					BotWrite(playerMovePos.x + 2, playerMovePos.y - 2);
					return;
				}
			}
			BotGoRandom(playerMovePos);
			return;
		}
		else
		if (cont >= 1) {
			if (contf >= 1) {
				if (playerMovePos.x < maxX && playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x + 1][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x + 1, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.x > 2 && playerMovePos.y > maxY - 1) {
					if (caroTable[playerMovePos.x - 2][playerMovePos.y + 2] == 0) {
						BotWrite(playerMovePos.x - 2, playerMovePos.y + 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 1) {
				if (playerMovePos.x > 1 && playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x - 1][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x - 1, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.x < maxX - 1 && playerMovePos.y > 2) {
					if (caroTable[playerMovePos.x + 2][playerMovePos.y - 2] == 0) {
						BotWrite(playerMovePos.x + 2, playerMovePos.y - 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			BotGoRandom(playerMovePos);
			return;
		}


		//Ngang
		contf = 0; conts = 0;
		tempPos = playerMovePos;
		while (tempPos.y > 1) {
			tempPos.y -= 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				contf++;
			else
				break;
		}
		tempPos = playerMovePos;
		while (tempPos.y < maxY) {
			tempPos.y += 1;
			if (caroTable[tempPos.x][tempPos.y] == 1)
				conts++;
			else
				break;
		}
		cont = contf + conts;
		if (cont >= 2) {
			if (contf >= 2) {
				if (playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.y > 3) {
					if (caroTable[playerMovePos.x][playerMovePos.y - 3] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y - 3);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 2) {
				if (playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.y < maxY - 2) {
					if (caroTable[playerMovePos.x][playerMovePos.y + 3] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y + 3);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (playerMovePos.y > 2) {
				if (caroTable[playerMovePos.x][playerMovePos.y - 2] == 0) {
					BotWrite(playerMovePos.x, playerMovePos.y - 2);
					return;
				}
			}
			if (playerMovePos.y < maxY - 1) {
				if (caroTable[playerMovePos.x][playerMovePos.y + 2] == 0) {
					BotWrite(playerMovePos.x, playerMovePos.y + 2);
					return;
				}
			}
			BotGoRandom(playerMovePos);
			return;
		}
		else
		if (cont >= 1) {
			if (contf >= 1) {
				if (playerMovePos.y < maxY) {
					if (caroTable[playerMovePos.x][playerMovePos.y + 1] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y + 1);
						return;
					}
				}
				if (playerMovePos.y > 2) {
					if (caroTable[playerMovePos.x][playerMovePos.y - 2] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y - 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			if (conts >= 1) {
				if (playerMovePos.y > 1) {
					if (caroTable[playerMovePos.x][playerMovePos.y - 1] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y - 1);
						return;
					}
				}
				if (playerMovePos.y < maxY - 1) {
					if (caroTable[playerMovePos.x][playerMovePos.y + 2] == 0) {
						BotWrite(playerMovePos.x, playerMovePos.y + 2);
						return;
					}
				}
				BotGoRandom(playerMovePos);
				return;
			}
			BotGoRandom(playerMovePos);
			return;
		}
		BotGoRandom(playerMovePos);
		return;
		break;
	case 3:					//Hard
		if (playerMovePos.x > 1 && playerMovePos.y > 1)
			caroTable[playerMovePos.x - 1][playerMovePos.y - 1] ++;
		if (playerMovePos.x > 1)
			caroTable[playerMovePos.x - 1][playerMovePos.y] ++;
		if (playerMovePos.x > 1 && playerMovePos.y < maxY)
			caroTable[playerMovePos.x - 1][playerMovePos.y + 1] ++;
		if (playerMovePos.y > 1)
			caroTable[playerMovePos.x][playerMovePos.y - 1] ++;
		if (playerMovePos.y < maxY)
			caroTable[playerMovePos.x][playerMovePos.y + 1] ++;
		if (playerMovePos.x < maxX && playerMovePos.y > 1)
			caroTable[playerMovePos.x + 1][playerMovePos.y - 1] ++;
		if (playerMovePos.x < maxX)
			caroTable[playerMovePos.x + 1][playerMovePos.y] ++;
		if (playerMovePos.x > maxX && playerMovePos.y > maxY)
			caroTable[playerMovePos.x + 1][playerMovePos.y + 1] ++;
		break;
	}
}

