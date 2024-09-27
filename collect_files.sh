#!/bin/bash

# Define the new directory where the files will be copied
DEST_DIR="./cpp_hpp_files"

# Create the new directory if it doesn't exist
if [ ! -d "$DEST_DIR" ]; then
  mkdir "$DEST_DIR"
fi

# Find all .cpp and .hpp files and copy them into the new directory
find . -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec cp {} "$DEST_DIR" \;

# Output the result
echo "All .cpp and .hpp files have been copied to $DEST_DIR"
