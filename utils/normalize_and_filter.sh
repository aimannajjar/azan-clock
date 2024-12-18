#!/bin/bash
# This scripts work with this github repo: https://github.com/HaroleDev/Airycons.git
# place it and run it in root dir of the repo to generate assets under day_images and night_images dirs
# example:
# ./normalize_and_filter day
# ./normalize_and_filyer night
# Set prefix from argument, default to "day" if not provided
prefix=${1:-day}

cd ~/Develop/Airycons
rm -rf "./${prefix}_images"
mkdir -p "./${prefix}_images"
find . -type d -name "${prefix^}" -exec find {} -type f -name "*.png" \; | xargs -I{} cp {} ./${prefix}_images/

# Allowed file names list
allowed_files=(
    "${prefix}_clear.png"
    "${prefix}_fog.png"
    "${prefix}_heavy_rain.png"
    "${prefix}_heavy_snowfall.png"
    "${prefix}_light_drizzle.png"
    "${prefix}_light_rain.png"
    "${prefix}_moderate_rain.png"
    "${prefix}_overcast.png"
    "${prefix}_partly_cloudy.png"
    "${prefix}_rime_fog.png"
    "${prefix}_snowflake.png"
    "${prefix}_thunderstorm.png"
)

# Normalize and check files
cd "${prefix}_images/"
for file in *; do 
    if [[ -f "$file" ]]; then
        # Normalize the file name
        newname=$(echo "$file" | sed 's/@32x\.png$/.png/' | tr '[:upper:]' '[:lower:]' | sed 's/[[:space:]]\+/_/g')
        newname="${prefix}_${newname}"

        # Rename file if needed
        if [[ "$file" != "$newname" ]]; then
            mv -v "$file" "$newname"
            convert $newname -quality 100 -resize 128x128 $newname
        fi

        # Check if the new name is in the allowed list, delete if not
        if [[ ! " ${allowed_files[@]} " =~ " ${newname} " ]]; then
            echo "Deleting $newname (not in allowed list)"
            rm -v "$newname"
        fi
    fi
done

# Copy to squareline assets
cd ~/Develop/azan-clock
rm -vf squareline/assets/${prefix}*.png
cp -v ~/Develop/Airycons/${prefix}_images/${prefix}_*.png squareline/assets/
