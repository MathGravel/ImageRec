#!/bin/bash

cd /home/jean/JeanM_Doctorat/PlanRecoByPolicy/PolicyGenerator 
./sarsop/src/pomdpsol ../Domain/$1'.pomdpx' --output ../Policy/$1'.policy' --timeout 5
./sarsop/src/polgraph --policy-file ../Policy/$1'.policy' --policy-graph ../Policy/$1'.dot' ../Domain/$1'.pomdpx'

