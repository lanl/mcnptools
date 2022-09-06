#!/bin/bash

declare -a UTILS=(
  "l3d2vtk"
  "l3dcoarsen"
  "l3dinfo"
  "l3dscale"
  "mctal2rad"
  "mergemctals"
  "mergemeshtals"
  "meshtal2vtk"
)

for u in ${UTILS[@]}; do
  echo "Printing help message for ${u}..."
  $u -h &> help_${u}.txt
done

