#
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
#

SEPOLICY_LEGACY_PATH := device/qcom/sepolicy-legacy
SEPOLICY_DEVICE_PATH := device/samsung/a5lte/sepolicy
include $(SEPOLICY_LEGACY_PATH)/sepolicy.mk

# DEVICE specific SELinux policy variable definitions
BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_DEVICE_PATH)/common
SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += $(SEPOLICY_DEVICE_PATH)/private
