#!/bin/bash
rm ../src/version.h
cd ../Scripts
python3 git_version_generator.py version.h
mv version.h ../src/
cd ../build
