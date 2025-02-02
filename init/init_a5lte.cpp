/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/logging.h>

#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;

void property_override(const std::string& name, const std::string& value)
{
    size_t valuelen = value.size();
    prop_info* pi = (prop_info*) __system_property_find(name.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), valuelen);
    }
    else {
        int rc = __system_property_add(name.c_str(), name.size(), value.c_str(), valuelen);
        if (rc < 0) {
            LOG(ERROR) << "property_override(\"" << name << "\", \"" << value << "\") failed: "
                       << "__system_property_add failed";
        }
    }
}

void property_override_triple(const std::string& product_prop, const std::string& system_prop, const std::string& vendor_prop, const std::string& value)
{
    property_override(product_prop, value);
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void init_dsds() {
    property_override("ro.multisim.set_audio_params", "true");
    property_override("ro.multisim.simslotcount", "2");
    property_override("persist.radio.multisim.config", "dsds");
}

void vendor_load_properties()
{
    // Init a dummy BT MAC address, will be overwritten later
    property_override("ro.boot.btmacaddr", "00:00:00:00:00:00");

    std::string bootloader = GetProperty("ro.bootloader","");

    if (bootloader.find("A500FU") == 0) {
        /* SM-A500FU */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultexx/a5ulte:6.0.1/MMB29M/A500FUXXU1CRH1:user/release-keys");
        property_override("ro.build.description", "a5ultexx-user 6.0.1 MMB29M A500FUXXU1CRH1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500FU");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ulte");
    } else if (bootloader.find("A500F") == 0) {
        /* SM-A500F */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ltexx/a5lte:6.0.1/MMB29M/A500FXXU1CPH2:user/release-keys");
        property_override("ro.build.description", "a5ltexx-user 6.0.1 MMB29M A500FXXU1CPH2 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500F");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5lte");

        // init_dsds();
    } else if (bootloader.find("A500G") == 0) {
        /* SM-A500G */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ltedd/a5lte:6.0.1/MMB29M/A500GXXS1CRJ1:user/release-keys");
        property_override("ro.build.description", "a5ltedd-user 6.0.1 MMB29M A500GXXS1CRJ1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500G");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5lte");

        // init_dsds();
    } else if (bootloader.find("A500M") == 0) {
        /* SM-A500M */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5lteub/a5lte:6.0.1/MMB29M/A500MUBS1CRI1:user/release-keys");
        property_override("ro.build.description", "a5lteub-user 6.0.1 MMB29M A500MUBS1CRI1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500M");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5lte");

        // init_dsds();
    } else if (bootloader.find("A500L") == 0) {
        /* SM-A500L */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultelgt/a5ultelgt:6.0.1/MMB29M/A500LKLU1CQC1:user/release-keys");
        property_override("ro.build.description", "a5ultelgt-user 6.0.1 MMB29M A500LKLU1CQC1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500L");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ulte");

    } else if (bootloader.find("A500H") == 0) {
        /* SM-A500H */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a53gxx/a53g:6.0.1/MMB29M/A500HXXS1CQC5:user/release-keys");
        property_override("ro.build.description", "a53gxx-user 6.0.1 MMB29M A500HXXS1CQC5 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500H");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a53g");

        // init_dsds();
    } else if (bootloader.find("A500Y") == 0) {
        /* SM-A500Y */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultedv/a5ulte:6.0.1/MMB29M/A500YDVS1CRJ1:user/release-keys");
        property_override("ro.build.description", "a5ultedv-user 6.0.1 MMB29M A500YDVS1CRJ1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500Y");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ulte");

    } else if (bootloader.find("A500F1") == 0) {
        /* SM-A500F1 */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultekx/a5ulteskt:6.0.1/MMB29M/A500F1U1CQB1:user/release-keys");
        property_override("ro.build.description", "a5ultekx-user 6.0.1 MMB29M A500F1U1CQB1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500F1");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ulteskt");

    } else if (bootloader.find("A500S") == 0) {
        /* SM-A500S */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ulteskt/a5ulteskt:6.0.1/MMB29M/A500SKSU1CQB1:user/release-keys");
        property_override("ro.build.description", "a5ulteskt-user 6.0.1 MMB29M A500SKSU1CQB1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500S");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ulteskt");

    } else if (bootloader.find("A500K") == 0) {
        /* SM-A500K */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultektt/a5ultektt:6.0.1/MMB29M/A500KKTU1CQB1:user/release-keys");
        property_override("ro.build.description", "a5ultektt-user 6.0.1 MMB29M A500KKTU1CQB1 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500K");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ultektt");

    } else if (bootloader.find("A500W") == 0) {
        /* SM-A500W */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ultebmc/a5ultebmc:6.0.1/MMB29M/A500WVLS1BPK2:user/release-keys");
        property_override("ro.build.description", "a5ultebmc-user 6.0.1 MMB29M A500WVLS1BPK2 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500W");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ultebmc");

    } else if (bootloader.find("A500YZ") == 0) {
        /* SM-A500YZ */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ltezt/a5ltezt:5.0.2/LRX22G/A500YZZTU1BOK2:user/release-keys");
        property_override("ro.build.description", "a5ltezt-user 5.0.2 LRX22G A500YZZTU1BOK2 release-keys");
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A500YZ");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ltezt");

    } else if (bootloader.find("A5000") == 0) {
        /* SM-A5000 */
        property_override_triple("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "samsung/a5ltezc/a5ltechn:6.0.1/MMB29M/A5000ZCS1CRG1:user/release-keys");
        property_override("ro.build.description", "a5ltezc-user 6.0.1 MMB29M A5000ZCS1CRG1 release-keys"); // after a system upgrade
        property_override_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "SM-A5000");
        property_override_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "a5ltechn");
    }

    std::string device = GetProperty("ro.product.device", "");
    LOG(ERROR) << "Found bootloader id %s setting build properties for %s device\n" << bootloader.c_str() << device.c_str();
}
