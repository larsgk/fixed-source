# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(fixed_source)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

set(gen_dir ${ZEPHYR_BINARY_DIR}/include/generated/)

generate_inc_file_for_target(app
src/HoldonaSec_16Khz_byBryanTeoh_FreePD.lc3
${gen_dir}/HoldonaSec_16Khz_byBryanTeoh_FreePD.lc3.inc)

generate_inc_file_for_target(app
src/HoldonaSec_24Khz_byBryanTeoh_FreePD.lc3
${gen_dir}/HoldonaSec_24Khz_byBryanTeoh_FreePD.lc3.inc)
