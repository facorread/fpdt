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

#include "filecls.h"
#include "listoffiles.h"
#include <iostream>
#include <list>
#include "phrasecls.h"
#include <set>

namespace fpdt {
	std::set<phraseCls, std::greater<phraseCls>> plagiarizedPhrases;

	void reportPlagiarism(phraseCls&& phrase) {
		plagiarizedPhrases.emplace(std::move(phrase));
	}

}

int main() {
	using namespace fpdt;
	typedef std::list<fileCls> fileListCls;
	///1. Questions files are opened.
	fileListCls questionsFiles;
	const listOfFilesCls questionsFileNames{listOfFiles("*.docx *.xlsx")};
	if(questionsFileNames.empty()) {
		std::cerr << "Assignment question files are required to prevent false positives.\n";
		std::exit(1);
	}
	for(const std::string& questionsFileName : questionsFileNames) {
		questionsFiles.emplace_back(questionsFileName);
	}

	///2. Assignment submissions are opened.
	fileListCls submissionFiles;
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
		fileListCls::iterator subIt{submissionFiles.emplace(submissionFiles.end(), submissionFileName)};
		for(const fileCls& questionsFile : questionsFiles) {
			subIt->removeQuestions(questionsFile);
		}
		if(submissionFiles.size() > 1) {
			for(fileListCls::iterator comparedIt(submissionFiles.begin()); comparedIt != subIt; ++comparedIt)
				subIt->searchPlagiarism(*comparedIt);
		}
	}

	///3. Print plagiarism report.
	for(const phraseCls& phrase : plagiarizedPhrases)
		phrase.print();
}
