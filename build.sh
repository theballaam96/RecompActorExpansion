#!/usr/bin/env bash
set -e
echo "Creating ./bin directory..."
mkdir -p ./bin

python to_dds.py

make

echo "Running RecompModTool..."
./RecompModTool ./mod.toml ./bin

echo "Zipping output file into ./bin..."
zip -j ./bin/asset_expansion.zip ./bin/asset_expansion.nrm

echo "Complete"