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

#ifndef LISTOFFILES_H
#define LISTOFFILES_H

#include <list>
#include <string>

namespace fpdt {
	typedef std::list<std::string> listOfFilesCls;

	/// Returns the list of files given by the command ls. Example: listOfFiles("*.docx")
	listOfFilesCls listOfFiles(const std::string filePattern);
}

#endif // LISTOFFILES_H
