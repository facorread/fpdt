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
#include "filecls.h"
#include <iostream>
#include "listoffiles.h"
#include "main.h"
#include "phrasecls.h"

namespace fpdt {
	void cleanExtractedFiles() {
		std::system("rm -f word/document.xml xl/sharedStrings.xml xl/worksheets/sheet*.xml fpdtExtractedSpreadsheets.txt");
	}

	/// Extracts the list of Word documents or Excel worksheets XML contained in a docx/xlsx file.
	const listOfFilesCls extractXML(const std::string& filename) {
		cleanExtractedFiles();
		// unzip returns 0 on successful extraction
		if(!std::system(std::string("unzip -qq '" + filename + "' word/document.xml 2> /dev/null").c_str()))
			return listOfFilesCls{{"word/document.xml"}};
		if(!std::system(std::string("unzip -qq '" + filename + "' xl/sharedStrings.xml 'xl/worksheets/sheet*.xml' 2> /dev/null && ls -1  > fpdtExtractedSpreadsheets.txt").c_str()))
			return listOfFiles("xl/sharedStrings.xml xl/worksheets/sheet*.xml");
		errorMsg << "Error extracting information from " << filename.c_str() << ": not a word/excel file.\n";
		std::abort();
	}

	/// Returns whether a character should be considered as valid content
	bool isContent(const char inputChar) {
		static const std::locale loc;
		return std::isalnum(inputChar, loc) || std::ispunct(inputChar, loc);
	}

	fileCls::fileCls(const std::string& filename) :
	mFileName{filename}
	{
		const listOfFilesCls listOfExtractedFilenames(extractXML(filename));
		for(const std::string& extractedFilename : listOfExtractedFilenames) {
			std::ifstream file(extractedFilename);
			if(!file) {
				errorMsg << "Error opening file " << extractedFilename.c_str() << ". Please debug.\n";
				std::abort();
			}
			// Used to transform all nonPrinting characters into only one whitespace
			bool skippingWhitespace{false};
			// Skipping an XML tag
			bool skippingTag{false};
			while(true) {
				char inputChar;
				file.get(inputChar);
				if(!file.good())
					break;
				if(skippingTag) {
					if(inputChar == '>') {
						skippingTag = false;
						if(!skippingWhitespace) {
							mContents += ' ';
							skippingWhitespace = true;
						}
					}
					continue;
				}
				if(inputChar == '<') {
					skippingTag = true;
					continue;
				}
				if(isContent(inputChar)) {
					if(skippingWhitespace)
						skippingWhitespace = false; // continue below;
					mContents += inputChar;
				} else if(!skippingWhitespace) {
					mContents += ' ';
					skippingWhitespace = true;
				}
			}
		}
		cleanExtractedFiles();
	}

	bool fileCls::nextComparisonInviable() const {
		if(mComparisonStart + 1 + minPhraseLength < mContents.length()) {
			mPosition = ++mComparisonStart;
			return false;
		}
		return true;
	}

	char fileCls::nextChar() const {
		// mContents.length() is optimized;
		if(mPosition < mContents.length())
			return mContents[mPosition++];
		else
			return 0;
	}
	void fileCls::removeQuestion() {
#ifdef DEBUG
		if((mPosition < mComparisonStart + minPhraseLength) || (mPosition >= mContents.length())) {
			errorMsg << "Attempt to erase an invalid portion of a string with length " << mContents.length() << ", at positions [" << mComparisonStart << ", " << mPosition - 1 << "). Please debug.\n";
			std::abort();
		}
#endif // DEBUG
		mContents.erase(mComparisonStart, mPosition - 1 - mComparisonStart);
		restartComparison();
	}

	void fileCls::removeQuestions(const fileCls& questionsDocument) {
		reset();
		questionsDocument.reset();
		std::string candidatePhrase;
		while(true) {
			const char c1{nextChar()};
			const char c2{questionsDocument.nextChar()};
			if(c1 && c2 && (c1 == c2)) {
				candidatePhrase.push_back(c1);
				continue;
			}
			if(candidatePhrase.length() > minPhraseLength) {
				removeQuestion();
				if(mComparisonStart + minPhraseLength >= mContents.length())
					return;
				questionsDocument.reset(); // Required because there is an all new text in *this
			} else {
				restartComparison();
				if(questionsDocument.nextComparisonInviable()) {
					if(nextComparisonInviable())
						return;
					questionsDocument.reset();
				}
			}
			candidatePhrase.clear();
		}
	}

	void fileCls::searchPlagiarism(const fileCls& otherAssignment) {
		reset();
		otherAssignment.reset();
		std::string candidatePhrase;
		while(true) {
			const char c1{nextChar()};
			const char c2{otherAssignment.nextChar()};
			if(c1 && c2 && (c1 == c2)) {
				candidatePhrase.push_back(c1);
				continue;
			}
			if(candidatePhrase.length() > minPhraseLength) {
				reportPlagiarism(phraseCls{candidatePhrase, *this, otherAssignment});
				mComparisonStart = mPosition;
				if(mComparisonStart + minPhraseLength >= mContents.length())
					return;
				otherAssignment.reset(); // Required because there is an all new text in *this
			} else {
				restartComparison();
				if(otherAssignment.nextComparisonInviable()) {
					if(nextComparisonInviable())
						return;
					otherAssignment.reset();
				}
			}
			candidatePhrase.clear();
		}
	}
}
