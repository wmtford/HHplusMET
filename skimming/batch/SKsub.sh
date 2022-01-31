#!/bin/bash

source exportProd.sh

JOBDIR=jobs
JOBTYPE=skim
INPUT=input/input_selection.txt
# SELTYPE1=signal_METVars
# SELTYPE2=LDP_METVars,SLm_METVars,SLe_METVars,SLmLDP_METVars,SLeLDP_METVars,GJet_CleanMETVars,GJetLDP_CleanMETVars,GJetLoose_CleanMETVars,GJetLooseLDP_CleanMETVars
# SELTYPE3=signal_JECup,signal_JECdown,signal_JERup,signal_JERdown
# SELTYPE4=signal_genMHT,SLm,SLe,SLm_genMHT,SLe_genMHT
SELTYPE1=signal_METVars
# SELTYPE1=GJet_CleanMETVars
SELTYPE2=
# SELTYPE2=SLm_METVars,SLe_METVars,GJet_CleanMETVars
SELTYPE3=
SELTYPE4=
#INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}
#INDIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/EWhad_WW_WZ_WH/Run2ProductionV18
INDIR=root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/scan
OUTDIR=tree
# STORE=root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}
STORE=root://cmseos.fnal.gov//store/user/emacdona/Skims/${RUN2PRODV}

CHECKARGS=""
TYPES=()
YEARS=()
DRYRUN=""

#check arguments
while getopts "ky:t:d" opt; do
	case "$opt" in
		k) CHECKARGS="${CHECKARGS} -k"
		;;
		y) IFS="," read -a YEARS <<< "$OPTARG"
		;;
		t) IFS="," read -a TYPES <<< "$OPTARG"
		;;
		d) DRYRUN="echo"
		;;
	esac
done

./SKcheck.sh ${CHECKARGS}

for TYPE in ${TYPES[@]}; do
	# temporary vars
	SELS=$SELTYPE1
	TMPINDIR="$INDIR"
	TMPSTORE="$STORE"

	# special settings
	#if [ "$TYPE" != Fast ]; then
	#	SELS=$SELS,$SELTYPE2
	#fi
	if [ "$TYPE" = Signal ] || [ "$TYPE" = SignalScan ] || [ "$TYPE" = Fast ]; then
		#SELS=$SELS,$SELTYPE3
		if [ "$TYPE" = Fast ] || [ "$TYPE" = SignalScan ]; then
			#TMPINDIR="$INDIR"/scan #changed for Vinay's scans
			TMPINDIR="$INDIR"
			TMPSTORE="$STORE"/scan
			#if [ "$TYPE" = Fast ]; then
				#SELS=$SELS,$SELTYPE4
			#fi
		fi
	fi
	if [ "$TYPE" = Data ]; then
		SELS=$SELS,signalUnblind_METVars
	fi

	for YEAR in ${YEARS[@]}; do
		SNAME=Skim${TYPE}${YEAR}
		source export${SNAME}.sh
		# skip nonexistent ones
		if [[ $? -ne 0 ]]; then continue; fi

		$DRYRUN ./SKtemp.sh ${JOBDIR} ${INPUT} ${SNAME} ${#SAMPLES[@]} ${SELS} ${TMPINDIR} ${OUTDIR} ${TMPSTORE} ${JOBTYPE}
	done
done
