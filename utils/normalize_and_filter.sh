#!/bin/bash
# This scripts work with this github repo: https://github.com/HaroleDev/Airycons.git
# place it and run it in root dir of the repo to generate assets under day_images and night_images dirs
# example:
# ./normalize_and_filter day
# ./normalize_and_filyer night
# Set prefix from argument, default to "day" if not provided
prefix=${1:-day}

mkdir -p "./${prefix}_images"
find . -type d -name "${prefix^}" -exec find {} -type f -name "*.png" \; | xargs -I{} cp {} ./${prefix}_images/

# Allowed file names list
allowed_files=(
    "${prefix}-clear.png"
    "${prefix}-mostly-clear.png"
    "${prefix}-partly-cloudy.png"
    "${prefix}-overcast.png"
    "${prefix}-fog.png"
    "${prefix}-rime-fog.png"
    "${prefix}-light-drizzle.png"
    "${prefix}-moderate-drizzle.png"
    "${prefix}-dense-drizzle.png"
    "${prefix}-light-rain.png"
    "${prefix}-moderate-rain.png"
    "${prefix}-heavy-rain.png"
    "${prefix}-light-freezing-drizzle.png"
    "${prefix}-dense-freezing-drizzle.png"
    "${prefix}-light-freezing-rain.png"
    "${prefix}-heavy-freezing-rain.png"
    "${prefix}-slight-snowfall.png"
    "${prefix}-moderate-snowfall.png"
    "${prefix}-heavy-snowfall.png"
    "${prefix}-snowflake.png"
    "${prefix}-thunderstorm.png"
    "${prefix}-thunderstorm-with-hail.png"
    "${prefix}-unknown.png"
)

# Normalize and check files
cd "${prefix}_images/"
for file in *; do 
    if [[ -f "$file" ]]; then
        # Normalize the file name
        newname=$(echo "$file" | sed 's/@32x\.png$/.png/' | tr '[:upper:]' '[:lower:]' | sed 's/[[:space:]]\+/-/g')
        newname="${prefix}-${newname}"

        # Rename file if needed
        if [[ "$file" != "$newname" ]]; then
            mv -v "$file" "$newname"
        fi

        # Check if the new name is in the allowed list, delete if not
        if [[ ! " ${allowed_files[@]} " =~ " ${newname} " ]]; then
            echo "Deleting $newname (not in allowed list)"
            rm -v "$newname"
        fi
    fi
done

