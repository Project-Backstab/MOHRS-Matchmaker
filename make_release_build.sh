#/bin/env bash

rm -r ../MOHRS-Matchmaker-release/ --exclude='../MOHRS-Matchmaker-release/data/log'
rm -r build/
rm data/log/*

mkdir build
cd build
cmake ..
make -j 4

cmake --build . --target doc

cmake --install . --prefix ../../MOHRS-Matchmaker-release

cd ../../MOHRS-Matchmaker-release

rm -r include/
rm -r lib/

