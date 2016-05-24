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
#include "filecls.h"
#include "listoffiles.h"
#include <iostream>
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
	studentSubmissionsCls questionsFiles;
	const listOfFilesCls questionsFileNames{listOfFiles("*.docx *.xlsx")};
	if(questionsFileNames.empty()) {
		std::cerr << "Assignment question files are required to prevent false positives.\n";
		std::exit(1);
	}
	for(const std::string& questionsFileName : questionsFileNames) {
		questionsFiles.add(questionsFileName);
	}

	///2. Assignment submissions are opened.
	// Map of student names to student submissions.
	typedef std::map<std::string, studentSubmissionsCls> studentsListCls;
	studentsListCls studentSubmissions;
	if(std::system(std::string("unzip -qqn submissions.zip '*.docx' '*.xlsx' -d fpdtSubmissions >& fpdtUnzipOutput.txt").c_str())) {
		std::cerr << "Error opening the submissions zipfile. Please review.\n";
		std::exit(1);
	}
	const listOfFilesCls submissionsFileNames{listOfFiles("fpdtSubmissions/*.{docx,xlsx}")};
	if(submissionsFileNames.empty()) {
		std::cerr << "No assignments in accepted formats.\n";
		std::exit(1);
	}
	for(const std::string& submissionFileName : submissionsFileNames) {
		const std::string& studentName{extractStudentName(submissionFileName)};
		studentSubmissions[studentName].add(submissionFileName);
	}
	if(studentSubmissions.empty()) {
		errorMsg << "No student submissions. Please debug.\n";
		std::abort();
	}
	for(studentsListCls::value_type& val : studentSubmissions) {
		val.second.setStudentName(val.first);
		val.second.removeQuestions(questionsFiles);
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
}
