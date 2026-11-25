#!/bin/sh
set -eu
awk 'NR>1 {total+=$2; if($3>0) covered+=$2} END {p=100*covered/total; printf "{\"statements\":{\"count\":%d,\"covered\":%d,\"percent\":%.14f}}\n",total,covered,p; if(p<85) exit 1}' reports/go/coverage.out > reports/go/summary.json
jq '.statements' reports/go/summary.json
