# Copyright (C) 2015 The CyanogenMod Project
# Copyright (C) 2018 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Inherit some common TWRP stuff.
$(call inherit-product, vendor/twrp/config/common.mk)

# Inherit device configuration for a5lte
$(call inherit-product, device/samsung/a5lte/full_a5lte.mk)

PRODUCT_NAME := twrp_a5lte
BOARD_VENDOR := samsung
PRODUCT_DEVICE := a5lte

TARGET_VENDOR_PRODUCT_NAME := a5lte
TARGET_VENDOR_DEVICE_NAME := a5ltexx
PRODUCT_BUILD_PROP_OVERRIDES += TARGET_DEVICE=a5lte PRODUCT_NAME=a5ltexx

# Fingerprint
BUILD_FINGERPRINT := samsung/a5ltexx/a5lte:6.0.1/MMB29M/A500FXXU1CPH2:user/release-keys
BUILD_DESCRIPTION := a5ltexx-user 6.0.1 MMB29M A500FXXU1CPH2 release-keys

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_NAME=a5ltexx \
    TARGET_DEVICE="a5ltexx"
    PRIVATE_BUILD_DESC="$(BUILD_DESCRIPTION)"

PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    ro.build.fingerprint=$(BUILD_FINGERPRINT)
