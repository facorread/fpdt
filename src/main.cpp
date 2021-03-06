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

#include "eta.h"
#include "studentcls.h"
#include "listoffiles.h"
#include <iostream>
#include "main.h"
#include <map>
#include "phrasecls.h"
#include <set>

namespace fpdt {
	std::set<phraseCls, std::greater<phraseCls>> plagiarizedPhrases;

	void reportPlagiarism(phraseCls&& phrase) {
		plagiarizedPhrases.emplace(std::move(phrase));
	}

	size_t nPlagiarismReports() {
		return plagiarizedPhrases.size();
	}

	std::string extractStudentName(const std::string& fileName) {
		std::string::const_iterator it(fileName.begin());
		for(; it != fileName.end(); ++it) {
			const char c(*it);
			if(c == '/')
				break;
		}
		std::string result;
		for(++it; it != fileName.end(); ++it) {
			const char c(*it);
			if(c == '_')
				break;
			result += c;
		}
		return result;
	}


}

int main() {
	using namespace fpdt;
	///1. Questions files are opened.
	studentCls questionsFiles;
	questionsFiles.setStudentName("Questions");
	const listOfFilesCls questionsFileNames{listOfFiles("*.docx *.xlsx")};
	if(questionsFileNames.empty()) {
		std::cerr << "Assignment question files are required to prevent false positives.\n";
		std::exit(1);
	}
	for(const std::string& questionsFileName : questionsFileNames) {
		questionsFiles.add(questionsFileName);
	}
	questionsFiles.organize();

	///2. Assignment submissions are opened.
	// Map of student names to student submissions.
	typedef std::map<std::string, studentCls> studentsListCls;
	studentsListCls studentSubmissions;
	bool unzipError{true};
	if(!std::system("~/doc/src/plagiarism/src/unzipDocument.sh '*.docx'"))
		unzipError = false;
	if(!std::system("~/doc/src/plagiarism/src/unzipDocument.sh '*.xlsx'"))
		unzipError = false;
	if(!std::system("~/doc/src/plagiarism/src/unzipOldDocuments.sh"))
		unzipError = false;
	if(unzipError) {
		std::cerr << "Error opening the submissions zipfile. Please review fpdtUnzipOutput.txt .\n";
		std::exit(1);
	}
	const listOfFilesCls submissionsFileNames{listOfFiles("fpdtSubmissions/*.docx fpdtSubmissions/*.xlsx")};
	if(submissionsFileNames.empty()) {
		std::cerr << "No assignments in accepted formats.\n";
		std::exit(1);
	}
	for(const std::string& submissionFileName : submissionsFileNames) {
		const std::string& studentName{extractStudentName(submissionFileName)};
		studentSubmissions[studentName].add(submissionFileName);
	}
	assertFalse(studentSubmissions.empty()) << "No student submissions. Please debug.\n";
	for(studentsListCls::value_type& val : studentSubmissions) {
		studentCls& student{val.second};
		student.setStudentName(val.first);
		student.removeQuestionsAndOrganize(questionsFiles);
		// student.organize(); // Never use this here.
	}
	for(studentsListCls::iterator sIt{studentSubmissions.begin()}; sIt != studentSubmissions.end(); ++sIt) {
		if(sIt->second.noTextFound())
			studentSubmissions.erase(sIt--);
	}
	{
		etaCls eta;
		eta.start(studentSubmissions.size() * (studentSubmissions.size() - 1) / 2);
		studentsListCls::const_iterator beginIt(studentSubmissions.begin());
		studentsListCls::const_iterator lhsIt(beginIt);
		studentsListCls::const_iterator endIt(studentSubmissions.end());
		for(++lhsIt; lhsIt != endIt; ++lhsIt) {
			for(studentsListCls::const_iterator rhsIt{beginIt}; rhsIt != lhsIt; ++rhsIt) {
				eta.step();
				if(!(eta.scenario() % 10))
					eta.info();
				lhsIt->second.searchPlagiarism(rhsIt->second);
			}
		}
		std::cerr << "              \r                                                                             \r";
		eta.printDescription();
	}
	///3. Print plagiarism report.
	for(const phraseCls& phrase : plagiarizedPhrases)
		phrase.print();

	fpdt::phraseClsTidyUp();
	std::cout << "less -S fpdt.txt\n";
	return std::system("less -S fpdt.txt\n");
}
