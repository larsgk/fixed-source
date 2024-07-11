# Broadcast Audio Fixed Source
This is a small application that creates a single channel continuous loop Broadcast Audio Source, using pre-encoded audio, stored with the application in flash.

Using 16 KHz mono and an nRF52840 Dongle (with 1 MB flash), it's possible to store just below 3 minutes of pre-encoded audio, which will be broadcasted in a continuous loop.

The application is a slightly modified version of the [BAP Broadcast Source sample](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/bluetooth/bap_broadcast_source).


# Getting started...
If you haven't done it yet, first go to [The Zephyr getting started guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) and install all dependencies (I'd recommend following the path with the virtual python environment).

# For development
For developers of the application, first do a fork of the repo.  Then do the following:

Make a local workspace folder (to hold the repo, zephyr and west modules):

```
mkdir my-workspace
cd my-workspace
```

Clone the repo:

```
git clone git@github.com:<your handle>/fixed-source.git
```

Initialize west and update dependencies:

```
west init -l fixed-source
west update
```

# For normal use (non-development)
This repo contains a stand alone Zephyr application that can be fetched and initialized like this:

```
west init -m https://github.com/larsgk/fixed-source --mr main my-workspace
```

Then use west to fetch dependencies:

```
cd my-workspace
west update
```

# Build and flash

Go to the repo folder:

```
cd fixed-source
```

## nRF5340 Audio DK board
The nRF5340 Audio DK has two cores - one for the application and one dedicated for the network (bluetooth controller).

Build the controller (from zephyr/samples/bluetooth/hci_ipc):
```
west build -b nrf5340_audio_dk_nrf5340_cpunet -d build/hci_ipc ../zephyr/samples/bluetooth/hci_ipc --pristine -- -DCONF_FILE=nrf5340_cpunet_iso-bt_ll_sw_split.conf
```

Build the application:
```
west build -b nrf5340_audio_dk_nrf5340_cpuapp -d build/nrf5340audiodk/app app --pristine
```

Clear all flash for the two cores with the recover command (only needed the first time):
```
nrfjprog --recover --coprocessor CP_NETWORK
nrfjprog --recover
```

And flash the two cores with the controller and application:

```
west flash -d build/hci_ipc
west flash -d build/app
```

## nRF52840 Dongle
There are a few scripts for building and flashing the nRF52840 Dongle - run them in this order:

```shell
compile_app.sh
create_flash_package.sh
flash_dongle.sh
```

Note: You'll need to get the dongle in DFU mode by pressing the small side buttton with a nail. The dongle should then start fading a red light in and out.

# Preparing the LC3 binary files
The application defaults to 16 KHz mono but can be configured to use 24 KHz by enabling the following project configuration:

```
CONFIG_BAP_BROADCAST_24_2_1=y
```

First, prepare a mono WAV file in either 16 KHz or 24 KHz sample rate.

> Note: for the nRF52840 Dongle, the maximum length for 16 KHz is just below 3 minutes of audio, for 24 KHz it's just below 2 minutes of audio (to fit the flash memory).

The data is pre-encoded with the LC3 encoder tool available from the [Google liblc3 project](https://github.com/google/liblc3/):


```
.../liblc3/bin$ ./elc3 -b 32000 input16KHz.wav output16KHz.lc3
```
or
```
.../liblc3/bin$ ./elc3 -b 48000 input24KHz.wav output24KHz.lc3
```

Then place the resulting lc3 file in the `src` folder - and modify the names in `CMakeLists.txt`:

```makefile
...
generate_inc_file_for_target(app
src/output16KHz.lc3
${gen_dir}/output16KHz.lc3.inc)

generate_inc_file_for_target(app
src/output24KHz.lc3
${gen_dir}/output24KHz.lc3.inc)
...
```

and in `main.c`

```c
...
static const uint8_t lc3_music[] = {
#include "output16KHz.lc3.inc"
};
...
static const uint8_t lc3_music[] = {
#include "output24KHz.lc3.inc"
};
```

you can also modify the broadcast name:
```c
#define BT_AUDIO_BROADCAST_NAME "Awesome Broadcast"
```
