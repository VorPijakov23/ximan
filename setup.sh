#!/bin/env bash

set -e

mkdir -p ~/.config/ximan
touch ~/.config/ximan/core.ximan

if ! command -v g++ &> /dev/null; then
    echo "setup.sh: g++ not found. Please, install gcc."
    exit 1
fi

if [ ! -f "ximan.cpp" ]; then
    echo "setup.sh: Source files not found. Make sure ximan.cpp exist."
    exit 1
fi

echo "compiling..."
g++ -o ximan ximan.cpp

sudo install -m 755 ximan /usr/local/bin/

echo "Clean..."
sudo rm ximan

echo "installation completed successfully"

if command -v ximan &> /dev/null; then
    echo "ximan is installed and ready to use."
else
    echo "installation error: ximan is not in the PATH."
    exit 1
fi

exit 0