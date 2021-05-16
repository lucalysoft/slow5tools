#!/bin/bash

# @author: Hiruna Samarakoon (hirunas@eng.pdn.ac.lk)

###############################################################################

# first do f2s and then merge several times with different number of threads

Usage="test_merge_threads.sh [path to fast5 directory] [path to create a temporary directory] [path to slow5tools executable]"

if [[ "$#" -lt 3 ]]; then
	echo "Usage: $Usage"
	exit
fi

FAST5_DIR=$1
TEST_DIR=$2
SLOWTOOLS=$3
F2S_OUTPUT_DIR=$TEST_DIR
MERGED_OUTPUT_DIR=$TEST_DIR
SLOW5_FORMAT=blow5
# set TEMP_DIR to use a faster storage system
TEMP_DIR="-f $TEST_DIR"
IOP=4
LOSSY_F2S=-l
LOSSY_MERGE=-l

THREAD_LIST="32 24 16 8 4 1"
# THREAD_LIST="1"


clean_file_system_cache() {
	clean_fscache
	#sync
	#echo 3 | tee /proc/sys/vm/drop_caches
}

slow5_merge_varied_threads () {

	for num in $THREAD_LIST
	do
		# clean_fscache

		num_threads=$num
		MERGED_BLOW5="$MERGED_OUTPUT_DIR/merged_thread_$num_threads.blow5"
		LOG="$TEST_DIR/merge_$SLOW5_FORMAT.log"


		echo >> $LOG
		echo "-------------$num_threads threads---------" >> $LOG
		echo >> $LOG

		/usr/bin/time -v $SLOWTOOLS merge $F2S_OUTPUT_DIR $TEMP_DIR $LOSSY_MERGE -t $num_threads -o $MERGED_BLOW5 2>> $LOG
		
		# delete merge file if necessary
		rm $MERGED_BLOW5
		# clean_fscache

	done

}

# create test directory
test -d  $TEST_DIR && rm -r $TEST_DIR && mkdir $TEST_DIR

# f2s .blow5s
SLOW5_FORMAT=blow5
F2S_OUTPUT_DIR=$TEST_DIR/blow5s
test -d  $F2S_OUTPUT_DIR && rm -r $F2S_OUTPUT_DIR
mkdir $F2S_OUTPUT_DIR
$SLOWTOOLS f2s $FAST5_DIR -d $F2S_OUTPUT_DIR --iop $IOP $LOSSY_F2S 2>>"$TEST_DIR/f2s_$SLOW5_FORMAT.log"

MERGED_OUTPUT_DIR=$TEST_DIR/merged_blow5s
test -d  $MERGED_OUTPUT_DIR && rm -r $MERGED_OUTPUT_DIR
mkdir $MERGED_OUTPUT_DIR
slow5_merge_varied_threads

# remove f2s or merge output if necessary
rm -r $F2S_OUTPUT_DIR
rm -r $MERGED_OUTPUT_DIR

# f2s .slow5s
SLOW5_FORMAT=slow5
F2S_OUTPUT_DIR=$TEST_DIR/slow5s
test -d  $F2S_OUTPUT_DIR && rm -r $F2S_OUTPUT_DIR
mkdir $F2S_OUTPUT_DIR
$SLOWTOOLS f2s $FAST5_DIR -d $F2S_OUTPUT_DIR --iop $IOP $LOSSY_F2S -s 2>>"$TEST_DIR/f2s_$SLOW5_FORMAT.log"

MERGED_OUTPUT_DIR=$TEST_DIR/merged_slow5s
test -d  $MERGED_OUTPUT_DIR && rm -r $MERGED_OUTPUT_DIR
mkdir $MERGED_OUTPUT_DIR
slow5_merge_varied_threads

# remove f2s or merge output if necessary
rm -r $F2S_OUTPUT_DIR
rm -r $MERGED_OUTPUT_DIR


# f2s .blow5s compressed
SLOW5_FORMAT=blow5_compressed
F2S_OUTPUT_DIR=$TEST_DIR/blow5_compressed
test -d  $F2S_OUTPUT_DIR && rm -r $F2S_OUTPUT_DIR
mkdir $F2S_OUTPUT_DIR
$SLOWTOOLS f2s $FAST5_DIR -d $F2S_OUTPUT_DIR --iop $IOP $LOSSY_F2S -c 2>>"$TEST_DIR/f2s_$SLOW5_FORMAT.log"

MERGED_OUTPUT_DIR=$TEST_DIR/merged_blow5_compressed
test -d  $MERGED_OUTPUT_DIR && rm -r $MERGED_OUTPUT_DIR
mkdir $MERGED_OUTPUT_DIR
slow5_merge_varied_threads

# remove f2s or merge output if necessary
rm -r $F2S_OUTPUT_DIR
rm -r $MERGED_OUTPUT_DIR
