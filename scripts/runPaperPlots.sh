#!/bin/sh

echo "Running and saving all plots for paper and auxilary materials"
root -l -b ABCD.C
echo "Running 1D TChiHH brazil plot"
python -b brazilHiggsinoLims.py
echo "Running 1D T5HH brazil plot"
python -b brazilGluinoLims.py

echo "Running 2D TChiHH brazil plot"
#2D TChiHH, first check scan_point has been run, and run if not
FILE=../src/limitsCombined_N1N2_data.txt
if ! [[ -f "$FILE" ]]
then
  echo "$FILE doesn't exist."
  root -l 'scan_point.cxx("N1N2","comb")'
fi
root -l limit_scan.cxx

echo "Running signal efficiency plots"
python -b signalEfficiency.py

echo "Running significance plots"
python -b significance.py
