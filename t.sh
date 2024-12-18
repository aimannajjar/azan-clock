#!/bin/bash

# Check if a file is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

input_file="$1"

# Check if the file exists
if [ ! -f "$input_file" ]; then
    echo "File not found: $input_file"
    exit 1
fi

# Remove duplicate lines and save the result to a temporary file
temp_file=$(mktemp)
sort "$input_file" | uniq > "$temp_file"

# Replace the original file with the temporary file
mv "$temp_file" "$input_file"

echo "Duplicates removed from $input_file"
