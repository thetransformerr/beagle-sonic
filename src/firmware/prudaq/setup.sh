#!/bin/bash

# Copyright 2015 Google Inc. All Rights Reserved.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#    http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.  See the License for the specific language governing
# permissions and limitations under the License.

if [[ $EUID -ne 0 ]]; then
  echo "Must be root. Try again with sudo."
  exit 1
fi

# Set the shared memory buffer length to 2MB.
# Default size is 256KB, which at high sample rates will fill up
# in only a few ms, making buffer overruns more likely when the
# host CPU gets busy.
# Max value is 4194304, but under some conditions uio_pruss
# has trouble allocating that much contiguous RAM and may fail.
#
# For high sample rates and much larger buffers, consider using
# BeagleLogic's prudaq support instead.
modprobe uio_pruss extram_pool_sz=2097152


# Overlays are the proper way, but currently there are some major issues with it
# Namely it doesn't actually configure the pins

# DEVICE_OVERLAY=/lib/firmware/prudaq-00A0.dtbo
# if [[ ! -f $DEVICE_OVERLAY ]] ; then
#   echo -n "$DEVICE_OVERLAY not found, so we can't enable it."
#   echo -n " (Did you run 'make install' to generate the .dtbo"
#   echo -n " file from the .dts and copy it to /lib/firmware?)"
#   echo
#   echo
#   exit 2
# fi

# if echo prudaq > /sys/devices/platform/bone_capemgr/slots ; then
#   echo 'Device overlay successfully loaded.'
# else
#   echo
#   echo -n "Couldn't enable the prudaq-00A0.dtbo device overlay."
#   echo -n " (If the error was 'File exists', then the overlay was already"
#   echo -n " enabled.)"
#   echo
#   exit 3
# fi

# Instead do it with universal overlays and config-pin
# Grabs the needed pins under a universal overlay so config-pin can touch them
# Overlays must precede pins

configlist=(
"overlay univ-all"

"P8.27 pruin"
"P8.28 pruin"
"P8.29 pruin"
"P8.30 pruin"
"P8.39 pruin"
"P8.40 pruin"
"P8.41 pruin"
"P8.42 pruin"
"P8.43 pruin"
"P8.44 pruin"
"P8.45 pruin"
"P8.46 pruin"
"P9.26 pruin"

"P9.25 pruout"
"P9.27 pruout"
"P9.28 pruout"
"P9.29 pruout"
"P9.30 pruout"
"P9.31 pruout"
"P9.92 pruout"    # See note
)

# Note: P9.42 is referred to as P9.92 above.
# The physical pin is shared by two processor pins. Add 50 to specify P9.42.1

for args in "${configlist[@]}"
do
  config-pin $args
done

exit 0