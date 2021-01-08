#pragma once
#ifndef CLASS_H
#define CLASS_H
#include <algorithm>
#include <vector>
#include "windows.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>


class isAlpha {
public:
	std::string str;

	isAlpha(std::string str) :str(str) {};

	bool operator () (char init) {
		if (str.find(init) != std::string::npos)
			return false;
		return true;
	}
};

#endif CLASS_H