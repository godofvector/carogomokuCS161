#pragma once
#include <iostream>
using namespace std;
char[] InputName() {
	char strName[50];
	cin.get(strName, 20, '\n');
	return strName;
}