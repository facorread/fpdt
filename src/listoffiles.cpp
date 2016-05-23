/*

This file is part of fpdt: Fabio's plagiarism detection tool
Copyright (C) 2016 - 2017 Fabio Correa <facorread@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <fstream>
#include "listoffiles.h"

std::string getLine(std::ifstream& file) {
	std::string result;
	while(true) {
		char c;
		file.get(c);
		if((c == '\n') || !file.good())
			return result;
		result.push_back(c);
	}
}

fpdt::listOfFilesCls fpdt::listOfFiles(const std::string filePattern) {
	if(std::system(("ls -1 " + filePattern + " > fpdtListOfFiles.txt").c_str()))
		return {};
	listOfFilesCls listOfDocuments;
	std::ifstream listOfSheets("fpdtListOfFiles.txt");
	while(true) {
		std::string sheetFileName(getLine(listOfSheets));
		if(!listOfSheets.good())
			return listOfDocuments;
		listOfDocuments.emplace_back(sheetFileName); // The orders are attached in reverse order which does not matter;
	}
	return {}; // Innocuous
}
