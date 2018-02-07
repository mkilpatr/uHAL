*******************************
*                             *
*          FED Tester         *
*                             *
*******************************

Instructions:

Start by setting source:
  . ./setup.bash

Enable developer tools:
  devtoolset

Compile Code (if needed):
  make

Run code:
  fedtester
or
  glibsetup

The GLIB architecure is completely removed from the FEDtool integration. The GLIB functions only require uhal libraries to compile and execute. 

to quickly replace the ip address of all file in the HWDescription files do:
grep -rl '{old_ipaddr}' ./settings | xargs sed -i 's/{old_ipaddr}/{new_ipaddr}/g'
