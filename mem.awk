#
# Copyright 2012 Red Hat, Inc.
#
# The contents of this file are subject to the terms
# of the Common Development and Distribution License
# (the "License").  You may not use this file except
# in compliance with the License.
#
# See the License for the specific language governing
# permissions and limitations under the License.
#

# Simple awk script to parse the output of "dmidecode --type 17" (run as
# root) to figure out the count, size and speed of the installed DIMMs.
#
# As root:
#   # dmidecode --type 17 | awk -f /shak/tools/mem.awk
#
# As user:
#   $ sudo dmidecode --type 17 | awk -f /shak/tools/mem.awk
#

BEGIN {
    count = 0;
    size = 0;
    size_mag = "";
    speed = 0;
    speed_mag = "";
    skip_speed = 0;
}

/^\tSize:/ {
    if ($2 == "No" && $3 == "Module" && $4 = "Installed") {
        skip_speed = 1;
    }
    else {
        count++;
        if (size == 0) {
            size = $2
            size_mag = $3;
        }
        else {
            if (size != $2) {
                printf("Unexpected memory size, expected %s, found %s\n", size, $2);
            }
        }
    }
}

/^\tSpeed:/ {
    if (skip_speed) {
        skip_speed = 0;
    }
    else {
        if (speed == 0) {
            speed = $2
            speed_mag = $3
        }
        else {
            if (speed != $2) {
                printf("Unexpected memory speed, expected %s, found %s\n", speed, $2);
            }
        }
    }
}

END {
    printf("%s %s%s %s%s\n", count, size, size_mag, speed, speed_mag);
}
