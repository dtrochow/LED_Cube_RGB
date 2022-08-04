# **Environment setup on Linux Environment**

## Table of Contents
[**Requirements**](#requirements)  
[**Build environment preparation**](#build-environment-preparation)  
[**Debug environment preparation**](#debug-environment-preparation)  



## **Requirements**

* Linux operating system
    * Tested on Ubuntu Server 22.04 LTS (https://ubuntu.com/download/server)
* 2 x Raspberry Pi Pico microcontrollers (https://www.raspberrypi.com/products/raspberry-pi-pico/)

* Raspberry Pi Pico documentation might be helpful:
  * https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
  * https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf
  * https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf




## **Build environment preparation**

1. Install all required tools

* arm-non-eabi compiller from producer site
    1. Download from gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2: 
        * https://developer.arm.com/downloads/-/gnu-rm
    2. Install arm-non-eabi

        ```bash
        sudo tar xjf gcc-arm-none-eabi-your-version.bz2 -C /usr/share/
        ```
    3. Create links to arm-non-eabi binaries

        ```bash
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc 
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
        sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-objdump /usr/bin/arm-none-eabi-objdump
        ```
    4. Install libncurses-dev and create links

        ```bash
        sudo apt install libncurses-dev
        sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
        sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
        ```
    5. Check if everythink works as expected

        ```bash
        arm-none-eabi-gcc --version
        arm-none-eabi-g++ --version
        arm-none-eabi-gdb --version
        arm-none-eabi-size --version
        ```

* All remaining Toolchain
    ```bash
    $ sudo apt update
    $ sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential 
    ```

2. Prepare pico-sdk

    * Clone pico-sdk repsitory

        ```bash
        $ git clone https://github.com/raspberrypi/pico-sdk
        ```

    * Uptade it

        ```bash
        $ cd pico-sdk
        $ git pull
        $ git submodule update
        ```

    * Set the PICO_SKD_PATH to the pico-sdk loaction
  
        * Add exporting this environment variable to .bashrc file

        ```bash
        $ vim .bashrc
        export PICO_SDK_PATH=~/path/to/pico-sdk
        ".bashrc" 119L, 3809B
        ```

        * Restart terminal

3. Check if everything is configured properly

    * Build Led Cube FW

        ```bash
        $ ./build_led_cube_rgb.sh
        ```

    * Build Unit Tests

        ```bash
        $ ./build_utests.sh
        ```

## **Debug environment preparation**

1. Wiring

   All wiring is presented on _Figure 37_ (page 62) in [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)

2. Prepare PicoProbe
   
   * Clone picoprobe repository

    ```bash
    $ cd ~/pico
    $ git clone https://github.com/raspberrypi/picoprobe.git
    ```

    * Build picoprobe

    ```bash
    $ cd picoprobe
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j4
    ```

    * Flash the picoprobe FW on Raspberry Pi Pico

        * Boot the Raspberry Pi Pico which will be used as a debugger with the BOOTSEL button pressed and drag on picoprobe.uf2.

3. Change PicoProbe USB device privilages

    The PicoProbe USB device needs to has proper privilages in Linux OS, to avoid using sudo privilages during debugging

    1. Use following command
        ```bash
        $ echo 'SUBSYSTEMS=="usb", ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0004", GROUP="users", MODE="0666"' | sudo tee -a /etc/udev/rules.d/98-PicoProbe.rules
        $ sudo udevadm control --reload
        ```
        To find the _idVendor_ and _idProducd_ you can use `lsudb -vvv` command. 

        You need to find thhose fields under `Raspberry Pi Picoprobe`.

4. Prepare OpenOCD server

    * Install required tools

        ```bash
        $ cd ~/pico
        $ sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev
        ```
    * Clone openocd repository

        ```bash
        $ git clone https://github.com/raspberrypi/openocd.git
        ```
    * Build openocd with picoprobe support enabled

        ```bash
        $ cd openocd
        $ ./bootstrap
        $ ./configure --enable-picoprobe
        $ make -j4
        ```
    * You can check if PicoProbe and OpenOCD server works properly by running this command when every connection is established properly
        ```bash
        $ src/openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -s tcl
        ```

5. Prepare VSCode debug configuration
    
    1. Install `Cortex-Debug` extension in VSCode
    2. Install `C/C++` and `C/C++ Extension Pack` extensions in VSCode
    3. Create following configuration files under `.vscode` directory

    * **launch.json**
        ```json
        {
            "version": "0.2.0",
            "configurations": [
                {
                    "name": "Pico Debug",
                    "cwd": "${workspaceRoot}",
                    "executable": "<path_to_binary_with_FW>",
                    "request": "launch",
                    "type": "cortex-debug",
                    "servertype": "openocd",
                    "serverpath": "<path_to_openocd_repo>/src/openocd",
                    // This may need to be arm-none-eabi-gdb depending on your system
                    "gdbPath" : "arm-none-eabi-gdb",
                    "device": "RP2040",
                    "configFiles": [
                        "interface/picoprobe.cfg",
                        "target/rp2040.cfg"
                    ],
                    "svdFile": "${env:PICO_SDK_PATH}/src/rp2040/hardware_regs/rp2040.svd",
                    "runToMain": true,
                    // Work around for stopping at main on restart
                    "postRestartCommands": [
                        "break main",
                        "continue"
                    ],
                    "searchDir": ["<path_to_openocd_repo>/openocd/tcl"]
                }
            ]
        }
        ```
        In some cases the serverpath field needs to be added
        (https://github.com/Marus/cortex-debug/issues/201#issuecomment-541278876)

    * **settings.json**
        ```json
        {
            // These settings tweaks to the cmake plugin will ensure
            // that you debug using cortex-debug instead of trying to launch
            // a Pico binary on the host
            "cmake.statusbar.advanced": {
                "debug": {
                    "visibility": "hidden"
                },
                "launch": {
                    "visibility": "hidden"
                },
                "build": {
                    "visibility": "default"
                },
                "buildTarget": {
                    "visibility": "hidden"
                }
            },
            "cmake.buildBeforeRun": true,
            "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
            "cortex-debug.openocdPath": "<path_to_openocd_repo>/openocd/src/openocd"
        }
        ```
