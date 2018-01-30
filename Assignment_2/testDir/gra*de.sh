#! /bin/bash
set -x
log='../result.log'
for i in $( ls )
do
	echo $i
	if [ -d $i ] ; then
		cd $i
	else
		continue
	fi
done		
		
set +x
	
