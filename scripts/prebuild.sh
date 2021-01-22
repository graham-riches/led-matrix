#!/bin/bash
rm ../source/version.h
cd ../scripts
python3 git_version_generator.py version.h
mv version.h ../source/
cd ../build
