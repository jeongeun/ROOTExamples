https://root.cern.ch/root/htmldoc/guides/users-guide/Histograms.html#random-numbers-and-histograms

#source /x3/cms/jelee/MCProd/delphesSetup
export SCRAM_ARCH=slc7_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
cd /cvmfs/cms.cern.ch/slc7_amd64_gcc530/cms/cmssw/CMSSW_9_1_0_pre3/src
cmsenv
export PATH=/x3/cms/jelee/MCProd/Delphes3.4.2:/x3/cms/jelee/MCProd/Delphes3.4.2/external/ExRootAnalysis:$PATH
export LD_LIBRARY_PATH=/x3/cms/jelee/MCProd/Delphes3.4.2:$LD_LIBRARY_PATH
cd -

