#!/bin/bash

{
	set -o xtrace
	unzip "$@" word/document.xml 'word/media/*.emf' xl/sharedStrings.xml 'xl/worksheets/sheet*.xml'
	[ \"$(echo word/media/*.emf)\" != \"word/media/*.emf\" ] && {
		libreoffice --headless --convert-to pdf --outdir word/media word/media/*.emf
		for file in word/media/*.pdf; do pdftotext \"${file}\"; done;
	}
} >& fpdt.log
