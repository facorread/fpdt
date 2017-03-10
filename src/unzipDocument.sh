#!/bin/bash

unzip -n submissions.zip "$@" -d fpdtSubmissions 2>&1 >> fpdtUnzipOutput.txt
