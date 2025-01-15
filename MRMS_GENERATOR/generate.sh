#!/bin/bash

# pull the latest MRMS reflectivity data and convert it to a GeoTiff using a
# palette (default palette). gdaldem does not convert between projections, and
# gdal_translate does not allow for the color translation, so color translation
# needs to happen first. mbtiles must be in mercader projection, so it cannot
# convert directly to mbtiles, so geotiff is used as an itermediate format
gdaldem color-relief \
    "/vsigzip//vsicurl/https://mrms.ncep.noaa.gov/2D/MergedBaseReflectivity/MRMS_MergedBaseReflectivity.latest.grib2.gz" \
    palette.txt test_dataset.geotiff  -alpha -of GTiff
# convert the geotiff to a mbtiles file including reprojection
gdal_translate test_dataset.geotiff test_dataset.mbtiles -of MBTILES
# create tiles of diffrent resolutions for multiple zoom levels
gdaladdo -r average test_dataset.mbtiles 2 4 8 16 32
