#!/bin/bash

cd ~/Desktop
if unzip -n submissions.zip "*.doc" -d fpdtOldDocument 2>&1 >> fpdtUnzipOutput.txt; then
	for file in fpdtOldDocument/*; do
		echo "Converting doc to docx: $file"
		libreoffice --headless --convert-to docx --outdir fpdtSubmissions "$file" 2>&1 >> umdExtractSubmissions.log
	done
fi
