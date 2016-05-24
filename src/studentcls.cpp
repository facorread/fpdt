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
#include "studentcls.h"
#include <iostream>
#include "listoffiles.h"
#include "main.h"
#include "phrasecls.h"
#include <set>

namespace fpdt {
	void cleanExtractedFiles() {
		std::system("rm -f word/document.xml xl/sharedStrings.xml xl/worksheets/sheet*.xml");
	}

	/// Extracts the list of Word documents or Excel worksheets XML contained in a docx/xlsx file.
	const listOfFilesCls extractXML(const std::string& filename) {
		cleanExtractedFiles();
		// unzip returns 0 on successful extraction
		if(!std::system(std::string("unzip -qq '" + filename + "' word/document.xml 2> /dev/null").c_str()))
			return listOfFilesCls{{"word/document.xml"}};
		if(!std::system(std::string("unzip -qq '" + filename + "' xl/sharedStrings.xml 'xl/worksheets/sheet*.xml' 2> /dev/null").c_str()))
			return listOfFiles("xl/sharedStrings.xml xl/worksheets/sheet*.xml");
		errorMsg << "Error extracting information from " << filename.c_str() << ": not a word/excel file.\n";
		std::abort();
	}

	/// Returns whether a character should be considered as valid content
	bool isContent(const char inputChar) {
		static const std::locale loc;
		return std::isalnum(inputChar, loc) || std::ispunct(inputChar, loc);
	}

	void studentCls::add(const std::string& submissionFileName) {
		const listOfFilesCls listOfExtractedFilenames(extractXML(submissionFileName));
		for(const std::string& extractedFilename : listOfExtractedFilenames) {
			std::ifstream file(extractedFilename);
			if(!file) {
				errorMsg << "Error opening file " << extractedFilename.c_str() << ". Please debug.\n";
				std::abort();
			}
			std::string phrase;
			bool startingSentence{true};
			// Used to transform all nonPrinting characters into only one whitespace
			bool skippingWhitespace{false};
			// Skipping an XML tag
			bool skippingTag{false};
			char inputChar;
			bool previousInputCharIsDigit{false};
			while(file.get(inputChar)) {
				if(skippingTag) {
					if(inputChar == '>')
						skippingTag = false;
					continue;
				}
				if(inputChar == '<') {
					skippingTag = true;
					continue;
				}
				if(isContent(inputChar)) {
					if(skippingWhitespace)
						skippingWhitespace = false; // continue below;
					phrase += inputChar;
					if((((inputChar == '.') && !previousInputCharIsDigit) || (inputChar == '?') || (inputChar == ':')) && (phrase.length() > 20)) {
						mUnorderedHashes.emplace_back(calculateHashAndStorePhrase(std::move(phrase)));
						phrase.clear();
						startingSentence = true;
					} else if(startingSentence)
						startingSentence = false;
					previousInputCharIsDigit = std::isdigit(inputChar, std::locale{});
			} else {
				if(!skippingWhitespace) {
					if(!startingSentence)
						phrase += ' ';
					skippingWhitespace = true;
				}
			}
			}
		}
		cleanExtractedFiles();
	}

void studentCls::organize() {
	std::set<phraseHashCls> uniqueSortedHashes;
	uniqueSortedHashes.insert(std::make_move_iterator(mUnorderedHashes.begin()), std::make_move_iterator(mUnorderedHashes.end()));
	if(uniqueSortedHashes.empty()) {
		errorMsg << "Submissions for student " << studentName().c_str() << " have no data. Please debug.\n";
		std::abort();
	}
	mUnorderedHashes.clear();
	mHashes.assign(std::make_move_iterator(uniqueSortedHashes.begin()), std::make_move_iterator(uniqueSortedHashes.end()));
}

void studentCls::removeQuestionsAndOrganize(const studentCls& questionsDocument) {
	mUnorderedHashes.sort();
	mUnorderedHashes.unique();
	if(mUnorderedHashes.empty()) {
		errorMsg << "Submissions for student " << studentName().c_str() << " have no data. Please debug.\n";
		std::abort();
	}
	orderedHashesCls::const_iterator questionIt(questionsDocument.mHashes.cbegin());
	const orderedHashesCls::const_iterator questionEnd(questionsDocument.mHashes.cend());
	unorderedHashesCls::iterator hashIt(mUnorderedHashes.begin());
	const unorderedHashesCls::const_iterator hashEnd(mUnorderedHashes.cend());
	while(true) {
		if(*hashIt < *questionIt) {
			if(++hashIt == hashEnd)
				break;
			continue;
		}
		if(*questionIt < *hashIt) {
			if(++questionIt == questionEnd)
				break;
			continue;
		}
		hashIt = mUnorderedHashes.erase(hashIt);
		if(hashIt == hashEnd)
			break;
	}
	mHashes.assign(std::make_move_iterator(mUnorderedHashes.begin()), std::make_move_iterator(mUnorderedHashes.end()));
	mUnorderedHashes.clear();
}

void studentCls::searchPlagiarism(const studentCls& otherStudent) const {
	matchedHashesCls matchedHashes;
	orderedHashesCls::const_iterator it(mHashes.cbegin());
	const orderedHashesCls::const_iterator itEnd(mHashes.cend());
	orderedHashesCls::const_iterator otherIt(otherStudent.mHashes.cbegin());
	const orderedHashesCls::const_iterator otherEnd(otherStudent.mHashes.cend());
	while(true) {
		if(*it < *otherIt) {
			if(++it == itEnd)
				break;
			continue;
		}
		if(*otherIt < *it) {
			if(++otherIt == otherEnd)
				break;
			continue;
		}
		matchedHashes.emplace_back(*it);
		if(++it == itEnd)
			break;
	}
	reportPlagiarism(phraseCls{std::move(matchedHashes), *this, otherStudent});
}

}
