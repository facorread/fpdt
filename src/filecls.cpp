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

#include <vector>
#include <fstream>
#include "filecls.h"

namespace fpdt {
	typedef std::vector<std::string> listOfDocumentsCls;

	void cleanExtractedFiles() {
		std::system("rm -f word/document.xml xl/sharedStrings.xml xl/worksheets/sheet*.xml fpdtExtractedSpreadsheets.txt");
	}

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

	/// Extracts the list of Word documents or Excel worksheets XML contained in a docx/xlsx file.
	const listOfDocumentsCls extractXML(const std::string& filename) {
		cleanExtractedFiles();
		// unzip returns 0 on successful extraction
		if(!std::system(std::string("unzip -qq '" + filename + "' word/document.xml 2> /dev/null").c_str()))
			return listOfDocumentsCls{{"word/document.xml"}};
		if(!std::system(std::string("unzip -qq '" + filename + "' xl/sharedStrings.xml 'xl/worksheets/sheet*.xml' 2> /dev/null && ls -1 xl/sharedStrings.xml xl/worksheets/sheet*.xml > fpdtExtractedSpreadsheets.txt").c_str())) {
			listOfDocumentsCls listOfDocuments;
			listOfDocuments.reserve(10);
			std::ifstream listOfSheets("fpdtExtractedSpreadsheets.txt");
			while(true) {
				std::string sheetFileName(getLine(listOfSheets));
				if(!listOfSheets.good())
					return listOfDocuments;
				listOfDocuments.emplace_back(sheetFileName); // The orders are attached in reverse order which does not matter;
			}
		}
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
		const listOfDocumentsCls listOfExtractedFilenames(extractXML(filename));
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

	char fileCls::nextChar() {
		// mContents.length() is optimized;
		if(mPosition < mContents.length())
			return mContents[mPosition++];
		else
			return 0;
	}

	void fileCls::removeQuestion() {
		if(mPosition > mComparisonStart + 1)
			mContents.erase(mComparisonStart, mPosition - 1 - mComparisonStart);
		else {
			errorMsg << "Attempt to erase an invalid portion of a string, at positions [" << mComparisonStart << ", " << mPosition - 1 << "). Please debug.\n";
			std::abort();
		}
	}

#ifdef DEBUG
	// Debug class
	class debugCls {
		public:
			debugCls() {
				const std::string testOutputFileName{"fileClsTestOutput.txt"};
				std::ofstream testOutput(testOutputFileName);
				testOutput << questionsDocument.contents() << '\n' << questionsSpreadsheet.contents() << '\n';
				errorMsg << "Please check the output of " << testOutputFileName.c_str() << ".\n";
			}
		private:
			fileCls questionsDocument{"../test/Samplequestionsdocument.docx"};
			fileCls questionsSpreadsheet{"../test/Sample questions spreadsheet.xlsx"};
	};

	debugCls debug;
#endif // DEBUG

}
