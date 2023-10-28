# Copyright (C) 2021 The LineageOS Project
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

RECOVERY_VARIANT := twrp
TW_THEME := portrait_hdpi
TW_INCLUDE_CRYPTO := false
TW_INCLUDE_CRYPTO_FBE := false
TW_EXTRA_LANGUAGES := false
TW_EXCLUDE_NANO := true
TW_EVENT_LOGGING := false
TW_HAS_DOWNLOAD_MODE := true
TW_NO_REBOOT_BOOTLOADER := true

TW_MTP_DEVICE := /dev/mtp_usb

TARGET_RECOVERY_DEVICE_DIRS += device/samsung/a5lte/twrp
