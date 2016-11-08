#!/bin/sh
DTSTAMP=`awk "BEGIN{print strftime(\"%y%b%d_%H%M\");}" `
echo $DTSTAMP
bin/pixel_gui $@ 2>&1 | tee log.$DTSTAMP
