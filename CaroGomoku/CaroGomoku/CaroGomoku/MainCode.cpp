#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <sstream> 
#include <conio.h>
#include <fstream>
#include <string>
#include <filesystem>

 using namespace std;
 
 struct Pos2D {
	 int x;
	 int y;
 };
 const string defaultlink = "C:\\Users\\Windows 10 Gamer\\Desktop\\CaroGomoku\\CaroGomoku\\CaroGomoku\\";
//Variables
 string P1, P2;
 string mapNames[20];
 int turn, modePlay, modeHard, maxX, maxY, moveCount;
 bool splashscreenExist, isExit, endGame;
 Pos2D curPos, initPos, boardPos, coordCursor, playerLastMove, loginPos;
 Pos2D coord[30][30];
 Pos2D imove[1000];
 int caroTable[30][30], predictTable[30][30];

 void main();
 void drawBoard(int x, int y);
 //Utitlities
 void textColor(short num);
 void UpdateSaveNames(string name) {
	 fstream f;
	 f.open("names.txt", ios::in);
	 string line, data;
	 getline(f, line);
	 data = line;
	 int count = 0;
	 while (!f.eof()) {
		 getline(f, line);
		 data = line;
		 if (data != "") {
			 count++;
			 mapNames[count] = data;
			 if (data == name) {
				 return;
			 }
		 }
	 }
	 f.close();
	 //Update file names.txt
	 //system("cls");
	 count++;
	 if (name != "")
		 mapNames[count] = name;
	 f.open("names.txt", ios::out);
	 f << count << endl;
	 for (int i = 1; i <= count; i++) {
		 f << mapNames[i] << endl;
	 }
	 f.close();
 }
 void LoadSaveNames() {
	 textColor(6);
	 int count = 0;
	 fstream f;
	 string line, data;
	 f.open("names.txt", ios::in);
	 getline(f, line);
	 //cout << "File save to load, choose one of them !" << endl;
	 while (!f.eof()) {
		 count++;
		 getline(f, line);
		 data = line;
		 if (data == "") {
			 break;
		 }
		 mapNames[count] = data;
		 cout << "           " << data << endl;
	 }
	 f.close();
}
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

void Load(string name);
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
void Login() {
	textColor(10);
	system("cls");
	if (modePlay == 2) {
		gotoXY(loginPos);
		cout << "Username 1: ";
		cin >> P1;
		loginPos.y += 5;
		gotoXY(loginPos);
		cout << "Username 2: ";
		cin >> P2;
	}
	else {
		gotoXY(loginPos);
		cout << "Username: ";
		cin >> P1;
	}

}
void LoadStatistics(string PlayerName, int &win, int &lose, int &match) {
	int lineCount = 0;
	fstream f;
	f.open(PlayerName + ".txt", ios::in);
	string data;
	string line;
	while (!f.eof())
	{
		getline(f, line);
		data = line;
		if (data == "")
			return;
		switch (lineCount)
		{
		case 0:
			win = data[0] - '0';
			break;
		case 1:
			lose = data[0] - '0';
			break;
		case 2:
			match = data[0] - '0';
			break;
		}
		lineCount++;
	}
	f.close();
}
void SaveStatistics(string PlayerName, bool isWin) {
	int winCount = 0, loseCount = 0, matchPlayed = 0;
	fstream f;
	LoadStatistics(PlayerName, winCount, loseCount, matchPlayed);
	f.open(PlayerName + ".txt", ios::out);
	if (isWin) {
		winCount++;
	}
	else
		loseCount++;
	matchPlayed++;
	f << winCount << endl;
	f << loseCount << endl;
	f << matchPlayed;
	f.close();
}
void Save(int turn) {
	string slot;
	Pos2D savePos;
	savePos.x = maxY * 2 + 40; savePos.y = maxX * 2 + 2;
	gotoXY(savePos);
	cout << "                       ";
	gotoXY(savePos);
	cout << "Name:";
	cin >> slot;
	UpdateSaveNames(slot);
	fstream f;
	f.open(defaultlink + "save\\" + slot + ".txt", ios::out);
	if (turn == 1) {
		f << "1" << endl;
	}
	else {
		f << "2" << endl;
	}
	string data;
	for (int i = 1; i <= maxX; i++) {
		for (int j = 1; j <= maxY; j++) {
			data += caroTable[i][j] + '0';
		}
		data += "\n";
	}
	f << data;
	f.close();
	Sleep(1000);
	gotoXY(coord[1][1]);
	cout << "                       ";
}
void DisplayMapLoad();
void Load(string name) {
	if (name != "") {
		int x = 0, y = 1;
		int i;
		fstream f;
		f.open(defaultlink + "save\\" + name + ".txt", ios::in);
		string data;
		string line;
		getline(f, line);
		turn = line[0] - '0';
		while (!f.eof())
		{
			i = 0;
			getline(f, line);
			data = line;
			if (x != 0) {
				while (data[i] != '\0') {
					caroTable[x][y] = data[i] - '0';
					i++;
					y++;
				}
			}
			else {
				turn = data[0] - '0';
			}
			y = 1;
			x++;
		}
		f.close();
		drawBoard(maxX, maxY);
		DisplayMapLoad();
	}
}
void DisplayMapLoad() {
	textColor(7);
	for (int i = 1; i < maxX; i++)
		for (int j = 1; j < maxY; j++)
			if (caroTable[i][j] == 1) {
				gotoXY(coord[i][j]);
				cout << "X";
			}
			else if (caroTable[i][j] == 2) {
				gotoXY(coord[i][j]);
				cout << "O";
			}
}

//Check Win Lose
bool CheckWinDoc(int turn, Pos2D signPos, int point) {
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
	while (x <= maxX) {
		x++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == point)
		return true;
	else
		return false;
}
bool CheckWinNgang(int turn, Pos2D signPos, int point) {
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
	while (y <= maxY) {
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == point)
		return true;
	else
		return false;
}
bool CheckWinSac(int turn, Pos2D signPos, int point) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//LenPhai
	while (x > 0 && y <= maxY) {
		x--;
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//XuongTrai
	x = signPos.x; y = signPos.y;
	while (x <= maxX && y > 0) {
		x++;
		y--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == point)
		return true;
	else
		return false;
}
bool CheckWinHuyen(int turn, Pos2D signPos, int point) {
	int count = 1;
	int x = signPos.x, y = signPos.y;
	//XuongPhai
	while (x <= maxX && y <= maxY) {
		x++;
		y++;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	//LenTrai
	x = signPos.x; y = signPos.y;
	while (x > 0 && y > 0) {
		x--;
		y--;
		if (caroTable[x][y] == turn)
			count++;
		else
			break;
	}
	if (count == point)
		return true;
	else
		return false;
}
bool CheckPWin(int num, Pos2D pCoord, int point) {
	if (CheckWinDoc(num, pCoord, point))
		return true;
	if (CheckWinNgang(num, pCoord, point))
		return true;
	if (CheckWinSac(num, pCoord, point))
		return true;
	if (CheckWinHuyen(num, pCoord, point))
		return true;
}
bool CheckBotWin(Pos2D botCoord, int point) {
	if (CheckWinDoc(2, botCoord, 5))
		return true;
	if (CheckWinNgang(2, botCoord, 5))
		return true;
	if (CheckWinSac(2, botCoord, 5))
		return true;
	if (CheckWinHuyen(2, botCoord, 5))
		return true;
}


//First Scene Methods
void Loading() {
	int percentLoading = 0;
	Pos2D tempPos, barPos;
	curPos.x = initPos.x + 30;
	curPos.y = initPos.y + 14;
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
			cout << "Press P to continue";
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
		Sleep(500);
	}
}

//Second Scene Methods
void ChangeToGameplayScreen() {
	while (1 != 0) {
		char ch;
		ch = _getch();
		if (ch == 'p') {
			system("cls");
			return;
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
int HardMode();
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
		char ch;
		ch = _getch();
		switch (ch)
		{
		case 13:
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
			break;
		}
	}
}
void ChooseToLoad() {
	textColor(7);
	system("cls");
	LoadSaveNames();
	Pos2D selectPos;
	selectPos.x = 1; selectPos.y = 0;
	int check = 0;
	while (1 > 0) {
		check = _getch();
		switch (check) {
		case 72: //Up
			if (selectPos.y > 0) {
				gotoXY(selectPos);
				cout << "  ";
				selectPos.y --;
			}
			break;

		case 80: //Down
			if (selectPos.y < 10) {
				gotoXY(selectPos);
				cout << "  ";
				selectPos.y ++;
			}
			break;
		case 13:
			Load(mapNames[selectPos.y + 1]);
			return;
		}
		gotoXY(selectPos);
		cout << "->";
	}
}
void NewOrLoad() {
	system("cls");
	Pos2D selectPos, menuPos;
	int check = 0;
	menuPos.x = 50; menuPos.y = 10;
	selectPos.x = menuPos.x - 5;
	selectPos.y = menuPos.y;
	textColor(10);
	gotoXY(menuPos);
	cout << "New Game";
	menuPos.y += 3;
	gotoXY(menuPos);
	cout << "Load Game";
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
		case 13:
			if (selectPos.y == 10) {
				Login();
				if (modePlay == 1)
					modeHard = HardMode();
				drawBoard(maxX, maxY);
				return;
			}
			else {
				ChooseToLoad();
				return;
			}
			break;
		}
		gotoXY(selectPos);
		cout << "->";
	}
}
void HelpMenuInGame() {
	textColor(7);
	Pos2D helpPos;
	helpPos.x = 12;
	helpPos.y = 2 * maxX + 2;
	gotoXY(helpPos);
	cout << "Use arrow key to move        Space - Go         Double S - Save          Double R - Reset";
}

//For Gameplay
void Initialize() {
	textColor(6);
	endGame = false;
	initPos.x = 12; initPos.y = 5;
	loginPos.x = 43; loginPos.y = 10;
	//Clear screen
	system("cls");
	cout << "Caro Game developed by Nguyen Gia Huy, v1.0";
	//SplashScreen 
	for (int i = 0; i < 1; i++) {
		curPos = initPos;
		CreateSplashScreen(i);
		Sleep(500);
	} 
	//Loading
	//Loading();
	//ChangeToGamePlay
	ChangeToGameplayScreen();

	//Menu
	Menu();
	//SetCoord
	boardPos.x = 10, boardPos.y = 1;
	SetCoordForArray();
	NewOrLoad();

	
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
	int timeReset = 0, timeSave = 0;
	while (1 > 0) {
		PlayerMove();
		char ch1;
		ch1 = _getch();
		if (ch1 == 'r') {
			main();
		}
		char ch2;
		ch2 = _getch();
		if (ch2 == 's') {
			Save(turn);
		}
	}
}

void PlayPVC() {

	while (1 > 0) {
		if (!endGame)
			PlayerMove();
		if (!endGame) 
			BotMove(modeHard, playerLastMove);
		char ch1;
		ch1 = _getch();
		if (ch1 == 'r') {
			main();
		}
		char ch2;
		ch2 = _getch();
		if (ch2 == 's') {
			Save(turn);
		}
	}
}

void Win(int turn) {
	int win = 0, lose = 0, match = 0;
	endGame = true;
	system("cls");
	cout << "Press R to play again...";
	Pos2D winPos;
	winPos.x = 50; winPos.y = 14;
		gotoXY(winPos);
		if (turn == 1) {
			if (modePlay == 2) {
				for (int i = 1; i < 10; i++) {
					gotoXY(winPos);
					textColor(i);
					cout << "PLAYER " << P1 << " WIN";
					Sleep(200);
				}
				SaveStatistics(P1, true);
				SaveStatistics(P2, false);
			}
			else {
				for (int i = 1; i < 10; i++) {
					gotoXY(winPos);
					textColor(i);
					cout << "PLAYER " << P1 << " WIN";
					Sleep(200);
				}
				SaveStatistics(P1, true);
				winPos.x -= 4;
				winPos.y += 3;
				gotoXY(winPos);
				LoadStatistics(P1, win, lose, match);
				cout << "Player: " << P1;
				winPos.y++;
				gotoXY(winPos);
				cout << "Win: " << win;
				winPos.y++;
				gotoXY(winPos);
				cout << "Lose: " << lose;
				winPos.y++;
				gotoXY(winPos);
				cout << "Match played: " << match;
			}
		}
		else {
			if (modePlay == 2) {
				for (int i = 1; i < 10; i++) {
					gotoXY(winPos);
					textColor(i);
					cout << "PLAYER " << P2 << " WIN";
					Sleep(200);
				}
				SaveStatistics(P1, false);
				SaveStatistics(P2, true);
				
				winPos.x -= 4;
				winPos.y += 3;
				gotoXY(winPos);
				cout << "Player: " << P1;
				LoadStatistics(P1, win, lose, match);
				winPos.y++;
				gotoXY(winPos);
				cout << "Win: " << win;
				winPos.y++;
				gotoXY(winPos);
				cout << "Lose: " << lose;
				winPos.y++;
				gotoXY(winPos);
				cout << "Match played: " << match;

				winPos.x += 4;
				winPos.y -= 3;
				gotoXY(winPos);
				cout << "Player: " << P2;
				LoadStatistics(P2, win, lose, match);
				gotoXY(winPos);
				cout << "Win: " << win;
				winPos.y++;
				gotoXY(winPos);
				cout << "Lose: " << lose;
				winPos.y++;
				gotoXY(winPos);
				cout << "Match played: " << match;
			}
			else {
				for (int i = 1; i < 10; i++) {
					gotoXY(winPos);
					textColor(i);
					cout << "COMPUTER WIN";
					Sleep(200);
				}
				SaveStatistics(P1, false);
				LoadStatistics(P1, win, lose, match);
				winPos.y += 3;
				gotoXY(winPos);
				cout << "Win: " << win;
				winPos.y++;
				gotoXY(winPos);
				cout << "Lose: " << lose;
				winPos.y++;
				gotoXY(winPos);
				cout << "Match played: " << match;
			}
		}
		while (1 > 0) {
			char ch;
			ch = _getch();
			if (ch == 'r') {
				main();
			}
		}
}

int HardMode() {
	int num;
	system("cls");
	Pos2D hardModePos;
	hardModePos.x = 0; hardModePos.y = 0;
	gotoXY(hardModePos);
	cout << "Input your mode you want to play, 1 - Easy, 2 - Normal, 3 - Hard:";
	while (1 > 0) {
		cin >> num;
		return num;
	}
}

void main() {
	//Variables 
	turn = 1, modePlay = 0, modeHard = 3, maxX = 25, maxY = 29, moveCount = 0;
	splashscreenExist = false, isExit = false, endGame = false;
	Initialize();
	if (modePlay == 2)
		PlayPVP();
	else {
		PlayPVC();
	}
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
				if (coordCursor.x < maxX)
					coordCursor.x += 1;
				break;
			case 75: //Left
				if (coordCursor.y > 1)
					coordCursor.y -= 1;
				break;
			case 77: //Right
				if (coordCursor.y < maxY)
					coordCursor.y += 1;
				break;
			case 'r':
				main();
				break;
			case 's':
				Save(turn);
				break;
			}
			playerLastMove.x = coordCursor.x; playerLastMove.y = coordCursor.y;
			curPos = coord[coordCursor.x][coordCursor.y];
			gotoXY(curPos);
			if (isPlaying) {
				if (GetAsyncKeyState(' ') && caroTable[coordCursor.x][coordCursor.y] == 0) {  //PlayerGo
					moveCount++;
					imove[moveCount] = coordCursor;
					if (turn == 1) {
						cout << "X";
						caroTable[coordCursor.x][coordCursor.y] = 1;
						if (CheckPWin(1, coordCursor, 5)) {
							Win(1);
						}
					}
					else {
						cout << "O";
						caroTable[coordCursor.x][coordCursor.y] = 2;
						if (CheckPWin(2, coordCursor, 5)) {
							Win(1);
						}
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
	if (CheckBotWin(botCoord, 5)) {
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
				CheckBotWin(botCoord, 5);
				return;
			}
		if (playerMovePos.x > i)
			if (caroTable[playerMovePos.x - i][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				gotoXY(coord[playerMovePos.x - i][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x - i][playerMovePos.y] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				return;
			}
		if (playerMovePos.y > i)
			if (caroTable[playerMovePos.x][playerMovePos.y - i] == 0) {
				botCoord = playerMovePos;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y - i]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y - i] = 2;
				CheckBotWin(botCoord, 5);
				return;
			}
		if (playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x][playerMovePos.y + i] == 0) {
				botCoord = playerMovePos;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y + i]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y + i] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				return;
			}
		if (playerMovePos.x < maxX - i + 1)
			if (caroTable[playerMovePos.x + i][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				gotoXY(coord[playerMovePos.x + i][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x + i][playerMovePos.y] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				break;
			}
		if (playerMovePos.x > 1)
			if (caroTable[playerMovePos.x - 1][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x -= 1;
				gotoXY(coord[playerMovePos.x - 1][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x - 1][playerMovePos.y] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				break;
			}
		if (playerMovePos.y > 1)
			if (caroTable[playerMovePos.x][playerMovePos.y - 1] == 0) {
				botCoord = playerMovePos;
				botCoord.y -= 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y - 1]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y - 1] = 2;
				CheckBotWin(botCoord, 5);
				break;
			}
		if (playerMovePos.y < maxY)
			if (caroTable[playerMovePos.x][playerMovePos.y + 1] == 0) {
				botCoord = playerMovePos;
				botCoord.y += 1;
				gotoXY(coord[playerMovePos.x][playerMovePos.y + 1]);
				cout << "O";
				caroTable[playerMovePos.x][playerMovePos.y + 1] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				break;
			}
		if (playerMovePos.x < maxX)
			if (caroTable[playerMovePos.x + 1][playerMovePos.y] == 0) {
				botCoord = playerMovePos;
				botCoord.x += 1;
				gotoXY(coord[playerMovePos.x + 1][playerMovePos.y]);
				cout << "O";
				caroTable[playerMovePos.x + 1][playerMovePos.y] = 2;
				CheckBotWin(botCoord, 5);
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
				CheckBotWin(botCoord, 5);
				break;
			}
		BotGoRandom(playerMovePos);
		CheckBotWin(botCoord, 5);
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
			CheckBotWin(botCoord, 5);
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

		//Turn 2

		//Huyen
		tempPos = playerMovePos;
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
			CheckBotWin(botCoord, 5);
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
		int max = 0;
		Pos2D trytoGo, bettertoGo;
			for (int dis = 1; dis <= 5; dis++) {
				for (int i = -dis; i <= dis; i++) 
					for (int j = -dis; j <= dis; j++)
						if (caroTable[playerMovePos.x + i][playerMovePos.y + j] == 0) {
							//Save the trytoGo position
							trytoGo.x = playerMovePos.x + i;
							trytoGo.y = playerMovePos.y + j;
							for (int cont = 5; cont > 0; cont--) {
								if (cont >= max) {
									if (CheckPWin(1, trytoGo, cont) || CheckBotWin(trytoGo, cont)) {
										bettertoGo.x = trytoGo.x;
										bettertoGo.y = trytoGo.y;
										max = cont;
									}
								}
							}
						}
				}
		BotWrite(bettertoGo.x, bettertoGo.y);
	}
}

