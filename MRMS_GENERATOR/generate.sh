#!/bin/bash

#gdal_translate "/vsigzip//vsicurl/https://mrms.ncep.noaa.gov/2D/MergedBaseReflectivity/MRMS_MergedBaseReflectivity.latest.grib2.gz" \
#               test_dataset.mbtiles -of MBTILES
gdaldem color-relief \
    "/vsigzip//vsicurl/https://mrms.ncep.noaa.gov/2D/MergedBaseReflectivity/MRMS_MergedBaseReflectivity.latest.grib2.gz" \
    palette.txt test_dataset.geotiff  -alpha -of GTiff
gdal_translate test_dataset.geotiff test_dataset.mbtiles -of MBTILES
gdaladdo -r average test_dataset.mbtiles 4 8 16 32 128
