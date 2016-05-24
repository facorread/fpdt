# fpdt: Fabio's plagiarism detection tool

# Copyright and license

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

# Description

This is a small `C++` program designed to detect intra-corpal plagiarism; this is, students copying texts among themselves.

This program is not designed for general use. You are welcome to use it to develop your own derivative program for your own purposes. In my case, this program is designed to work with student assignment submissions downloaded from the University of Maryland's ELMS learning management system.

In my class webpage the students download the assignment questions in the form of `docx` and `xlsx` documents; they answer the questions directly there, and submit the completed documents via ELMS.

This program takes as input the file `submissions.zip` downloaded from the ELMS assignment page. It will use the unzip program to extract the submissions into a `fpdt` folder, then it will extract the text from all submissions into memory.

This program optionally takes as input the original questions documents, which is highly recommended; otherwise the program will interpret the questions and guides as plagiarism as they will appear in every submission.

# Workflow

Please read carefully and understand all instructions in this section, as they will result in file deletion.

The workflow of plagiarism detection starts with a text terminal located at the folder to which web downloads are saved (usually `~/Desktop` or `~/Downloads`).

`rm -rv submissions*.zip *.docx *.xlsx fpdtSubmissions fpdt.txt`

The ELMS assignment webpage is opened, the questions are downloaded, and then the `Download Submissions` link is clicked. Once the files are downloaded, the program is invoked at the terminal without changing the working directory. Every `docx` and `xlsx` file currently at the download directory (not subdirectories or zipfiles) is taken as questions input.

# Output

This program produces a file `fpdt.txt` containing a list in which each entry consists of the names of the students and the phrases that match between them, in decreasing length order. Each item serves as a red flag for your own inspection of their assignments.

# System requirements

This program requires `cmake`, a recent `C++14` compiler, and the `unzip` utility.

# Source code

The source code for this program is hosted at https://gitlab.com/facorread/fpdt

