#! /bin/sh
perl -ne 'printf "%s (%d%%)\n", $1, 100 * (hex($1) - 0x2100) / (0xe800 - 0x2100) if /([\dA-F]{4}) B __end/;' a.map
