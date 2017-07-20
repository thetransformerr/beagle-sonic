#!/bin/bash


if [[ $EUID -ne 0 ]]; then
  echo "Must be root. Try again with sudo."
  exit 1
fi




# Overlays are the proper way, but currently there are some major issues with it
# Namely it doesn't actually configure the pins

# Instead do it with universal overlays and config-pin
# Grabs the needed pins under a universal overlay so config-pin can touch them
# Assumes needed universal overlay is loaded. If not, config-pin can do it.
# If doing so, add the overlay before the pin configs.

configlist=(
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

#output pins
"P9.25 pruout"
"P9.27 pruout"
"P9.28 pruout"
"P9.29 pruout"
"P9.30 pruout"
"P9.31 pruout"
"P9.22 pwm"
)


for args in "${configlist[@]}"
do
  config-pin $args
  echo 0 > /sys/devices/platform/ocp/48300000.epwmss/48300200.pwm/pwm/pwmchip0/export || true
done

exit 0
