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
SEPOLICY_UM_PATH := device/qcom/sepolicy-legacy-um
SEPOLICY_DEVICE_PATH := device/samsung/a5lte/sepolicy
include $(SEPOLICY_UM_PATH)/SEPolicy.mk

# Board specific SELinux policy variable definitions as stolen from
# legacier repo
BOARD_VENDOR_SEPOLICY_DIRS := \
              $(BOARD_VENDOR_SEPOLICY_DIRS) \
              $(SEPOLICY_LEGACY_PATH)/common \
              $(SEPOLICY_LEGACY_PATH)/legacy-common \
              $(SEPOLICY_UM_PATH) \
              $(SEPOLICY_UM_PATH)/legacy/vendor/common/sysmonapp \
              $(SEPOLICY_UM_PATH)/legacy/vendor/ssg \
              $(SEPOLICY_UM_PATH)/legacy/vendor/common

BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_LEGACY_PATH)/msm8916

ifneq (,$(filter userdebug eng, $(TARGET_BUILD_VARIANT)))
  ifneq ($(PRODUCT_SET_DEBUGFS_RESTRICTIONS),true)
    BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_UM_PATH)/legacy/vendor/common/debugfs
    BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_UM_PATH)/legacy/vendor/test/debugfs
  endif
  BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_UM_PATH)/legacy/vendor/test
endif

# DEVICE specific SELinux policy variable definitions
BOARD_VENDOR_SEPOLICY_DIRS += $(SEPOLICY_DEVICE_PATH)/common
SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += $(SEPOLICY_DEVICE_PATH)/private
