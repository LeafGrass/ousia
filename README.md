# Ousia
_A Modest Stupid Embedded Operating System Basics_

`COPYRIGHT (C) LEAFGRASS`


## Introduction
* **Ousia** is picked from ancient Greek.
* Mainly intend to describe the basics of an operating system core.
* Developed following **KISS** principle.
* Completely open and free, use it anywhere you wish!


## Source Tree Architecture
* `core/`  
    Ousia core routines.

* `doc/`  
    Complete documentation of Ousia.

* `driver/`  
    Different kinds of device drivers based on Ousia.
    They are all configurable.

* `framework/`  
    Framework based on Ousia. Such as shell, vfs, etc.
    They are scalable and configurable, either.

* `include/`  
    Header files of main routines of Ousia.

* `platform/`  
    Chip and board specific code, e.g. low-level library.
    Ousia porting code are also placed here.

* `project/`  
    Project source code. Include several sample project instances.

* `script/`  
    Useful scripts used while Ousia developing.

* `support/`  
    Basic supporting stuffs for Ousia developing and building.
    Useful template files are placed here, such as porting code template.


## How to Start for Fresh New Environment
-   Install toolchain
-   Make  
    For a general purpose, these steps are enough, but there are some more
    steps needs to do for a specific platform.

    **e.g.** For stm32 on linux host, we use `dfu`,  `serial` or `jtag` to upload firmware:
    
1.  Install Codesourcery toolchain. (e.g. `arm-2011.03-42-arm-none-eabi`)
2.  Install `dfu-util/pyserial/openocd` (all up to your preference).  
    Ps: In fedora or ubuntu, `yum` or `apt-get` install them shall be fine.
3.  Resolve usb or serial access problems.  
    (e.g. copy `ousia/support/dfu/45-maple.rules` to `/etc/udev/rules.d/`)
4.  Install minicom for uart print if you want to debug with console enabled.  
    Ps: Default port is `/dev/ttyACMx`
5.  Do `make` and `make install`.


Create User Project Steps
-------------------------
-   Create a directory named `PROJECT_NAME` in folder `project`.
-   Add user source code.
-   Create a file `rules.mk` to specify source code related rules for makefile.
-   Update `TARGET_PLATFORM` and `PROJECT_NAME` in main `Makefile`, then do `make`.

_Ps: Refer to existing projects for further detailed information._


Core Developing Steps
---------------------
-   Choose or create a branch of yourself to work on.
-   Update source code, include those version related strings.
-   Modify related Makefiles and configuration files.
-   Build then do some testing on this branch.
-   Commit it.
-   Tag a new one if a new version is commited.
-   Push this branch to remote repo.
-   If this is a release version, update Ousia Release Notes.


Porting Steps
-------------
-   Create a directory named `PLATFORM_NAME` in folder `platform/`, then nav to it.
-   Create three files: `rules.mk`, `config.mk` and `target.mk`.  
	`rules.mk`: makefile source code related rules  
	`config.mk`: parameters or flags for toolchains and environment  
	`target.mk`: rules for building target  
-   Create a directory named `port` there then create three files in it.  
	`ousia_cfg.h`: os scalability related configurations  
	`ousia_port.h`: header of porting code  
	`ousia_port.c`: implentation of porting  
-   Create other porting related stuffs, such as linker scripts, if necessary.
-   Update `TARGET_PLATFORM` and `PROJECT_NAME` in main `Makefile`, then do `make`.


Build A Different Platform
--------------------------
Only modify the header TODO in root `Makefile`
-   Assign specific `TARGET_PLATFORM` and `PROJECT_NAME`


Download Code to Target Chip
----------------------------
-   Modify related user customization items in `Makefile` in source tree.
-   Do proper operation on hardware. (i.e. change boot jumper or reset or sth.)
-   Then type `make install` and it will do everything for you.

Ps: For stm32, there may be a bootloader, `make bootloader` will download
bootloader code to chip via serial. And for simulation on x86, no code
downloading procedure is needed. See `Makefile` for detailed information.


Acknowledgments
---------------
Parts of make system and libmaple stm32 low-level code are borrowed from libmaple.  
Thanks to their excellent works! - [leaflabs.com](http://leaflabs.com)  
Also great appreciations to the contributors! - [nuttx.org](http://nuttx.org)  
Many code are originally come or inspired from **NuttX**, a perfect and stable operating system.  
