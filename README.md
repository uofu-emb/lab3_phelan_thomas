# Lab 3 Threading
[![CMake](https://github.com/uofu-emb/lab3_phelan_thomas/actions/workflows/main.yml/badge.svg)](https://github.com/uofu-emb/lab3_phelan_thomas/actions/workflows/main.yml)
[![CMake](https://github.com/uofu-emb/lab3_phelan_thomas/actions/workflows/main.yml/badge.svg?branch=dev)](https://github.com/uofu-emb/lab3_phelan_thomas/actions/workflows/main.yml)

# Renode setup
The Raspberry Pico needs configuration files for Renode to work properly.

* On MacOS, the installation location is `/Applications/Renode.app/Contents/MacOs`
* On Linux, the location for Debian, Fedora, and Arch is `/opt/renode`
* On Windows, the location is `C://Program Files/Renode`

To add the Pico configuration files:
1. Copy `rp2040_spinlock.py` and `rp2040_divider.py` to the `scripts/pydev` directory of your Renode installation.
1. Copy `rpi_pico_rp2040_w.repl` to the `platforms/cpus` directory.
