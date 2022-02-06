/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.

 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <dlfcn.h>
#include <cstring>

#include <cutils/log.h>
#include "GeomagneticSensor.h"

#define DEBUG 1
//#define ALOG_NDEBUG 0

/*****************************************************************************/

GeomagneticSensor::GeomagneticSensor()
: SensorBase(NULL, "magnetic_sensor"),
      mEnabled(0),
      mPendingMask(0),
      mInputReader(32)
{
    mAccRefCount = 0;
    mMotionValue = 0;

    memset(mPendingEvents, 0, sizeof(mPendingEvents));

    mPendingEvents[MagneticField].version = sizeof(sensors_event_t);
    mPendingEvents[MagneticField].sensor = ID_M;
    mPendingEvents[MagneticField].type = SENSOR_TYPE_MAGNETIC_FIELD;
    mPendingEvents[MagneticField].magnetic.status = SENSOR_STATUS_ACCURACY_HIGH;
    memset(mPendingEvents[MagneticField].data, 0, sizeof(mPendingEvents[MagneticField].data));

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }
}

GeomagneticSensor::~GeomagneticSensor()
{
}

int GeomagneticSensor::setInitialState() {
    return 0;
}

int GeomagneticSensor::enable(int32_t, int en) {
    int flags = en ? 1 : 0;
    if (flags != mEnabled) {
        int fd;
        strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
        fd = open(input_sysfs_path, O_RDWR);
        if (fd >= 0) {
            char buf[2];
            buf[1] = 0;
            if (flags) {
                buf[0] = '1';
            } else {
                buf[0] = '0';
            }
            write(fd, buf, sizeof(buf));
            close(fd);
            mEnabled = flags;
            setInitialState();
            return 0;
        }
        return -1;
    }
    return 0;
}

int GeomagneticSensor::setDelay(int32_t handle, int64_t ns)
{
    int fd;
    strcpy(&input_sysfs_path[input_sysfs_path_len], "poll_delay");
    fd = open(input_sysfs_path, O_RDWR);
    if (fd >= 0) {
        char buf[80];
        sprintf(buf, "%lld", ns);
        write(fd, buf, strlen(buf)+1);
        close(fd);
        return 0;
    }
    return -1;
}

int GeomagneticSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    int numEventReceived = 0;
    int sensorId = ID_M;
    while (flush_state) {
        if (flush_state & (1 << sensorId)) { /* Send flush META_DATA_FLUSH_COMPLETE immediately */
            sensors_event_t sensor_event;
            memset(&sensor_event, 0, sizeof(sensor_event));
            sensor_event.version = META_DATA_VERSION;
            sensor_event.type = SENSOR_TYPE_META_DATA;
            sensor_event.meta_data.sensor = sensorId;
            sensor_event.meta_data.what = 0;
            *data++ = sensor_event;
            count--;
            numEventReceived++;
            flush_state &= ~(1 << sensorId);
            ALOGE("GeomagneticSensor: %s Flushed sensorId: %d", __func__, sensorId);
        }
        sensorId++;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_REL) {
            processEvent(event->code, event->value);
            mInputReader.next();
        } else if (type == EV_SYN) {
            int64_t time = getTimestamp();
            for (int j=0 ; count && mPendingMask && j<numSensors ; j++) {
                if (mPendingMask & (1<<j)) {
                    mPendingMask &= ~(1<<j);
                    mPendingEvents[j].timestamp = time;
                    if (mEnabled & (1<<j)) {
                        *data++ = mPendingEvents[j];
                        count--;
                        numEventReceived++;
                    }
                }
            }
            if (!mPendingMask) {
                mInputReader.next();
            }
        } else {
            ALOGE("GeomagneticSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
            mInputReader.next();
        }
    }
    return numEventReceived;
}

void GeomagneticSensor::processEvent(int code, int value)
{
    ALOGV("GeomagneticSensor: event code=%d, value=%d", code, value);
    switch (code) {
        case EVENT_TYPE_MAGV_X:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.x = value * CONVERT_M_X;
            break;
        case EVENT_TYPE_MAGV_Y:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.y = value * CONVERT_M_Y;
            break;
        case EVENT_TYPE_MAGV_Z:
            mPendingMask |= 1<<MagneticField;
            mPendingEvents[MagneticField].magnetic.z = value * CONVERT_M_Z;
            break;
    }
}

int GeomagneticSensor::flush(int handle)
{
    if (mEnabled){
        flush_state |= (1 << handle);
        ALOGE("GeomagneticSensor: %s: handle: %d", __func__, handle);
        return 0;
    }
    return -EINVAL;
}
