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
#include <stdlib.h>
#include <dirent.h>
#include <sys/select.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/uinput.h>
#include <cutils/log.h>
#include <cstring>


#include "AccelSensor.h"

#define LOGTAG "AccelerometerSensor"

// ioctls
#define LSM330DLC_ACCEL_IOCTL_BASE 'a'
#define LSM330DLC_ACCEL_IOCTL_SET_ENABLE   \
	_IOW(LSM330DLC_ACCEL_IOCTL_BASE, 9, int)


/*****************************************************************************/
AccelSensor::AccelSensor()
    : SensorBase(NULL, "accelerometer_sensor"),
    mEnabled(0),
    mInputReader(4),
    mHasPendingEvent(false)
{
    ALOGE("%s: init", __func__);

    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_A;
    mPendingEvent.type = SENSOR_TYPE_ACCELEROMETER;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    if (data_fd) {
        strcpy(input_sysfs_path, "/sys/class/input/");
        strcat(input_sysfs_path, input_name);
        strcat(input_sysfs_path, "/device/");
        input_sysfs_path_len = strlen(input_sysfs_path);
    }
}

AccelSensor::~AccelSensor() {
	ALOGE("%s: init", __func__);
    //  ALOGD("AccelSensor::~AccelSensor()");
    if (mEnabled) {
        enable(0, 0);
    }
}

int AccelSensor::setInitialState() {
	ALOGE("%s: init", __func__);
    return 0;
}

int AccelSensor::enable(int32_t handle, int en) {
	ALOGE("%s: init", __func__);
    int flags = en ? 1 : 0;
    int fd;
    if (flags != mEnabled) {
        strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
        fd = open(input_sysfs_path, O_RDWR);
        if (fd >= 0) {
            write(fd, en == 1 ? "1" : "0", 2);
            close(fd);
            mEnabled = flags;
            setInitialState();
            return 0;
        }
        return -1;
    }
    return 0;
}


bool AccelSensor::hasPendingEvents() const {

    /* FIXME probably here should be returning mEnabled but instead
       mHasPendingEvents. It does not work, so we cheat.*/
    //ALOGD("AccelSensor::~hasPendingEvents %d", mHasPendingEvent ? 1 : 0 );
    return mHasPendingEvent;
}


int AccelSensor::setDelay(int32_t handle, int64_t ns)
{
	ALOGE("%s: init", __func__);
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


int AccelSensor::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    int numEventReceived = 0;
    int sensorId = ID_A;
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
            ALOGE("AccelSensor: %s Flushed sensorId: %d", __func__, sensorId);
        }
        sensorId++;
    }

    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_REL) {
            float value = event->value;
            if (event->code == EVENT_TYPE_ACCEL_X) {
                mPendingEvent.acceleration.x = value * CONVERT_A_X;
            } else if (event->code == EVENT_TYPE_ACCEL_Y) {
                mPendingEvent.acceleration.y = value * CONVERT_A_Y;
            } else if (event->code == EVENT_TYPE_ACCEL_Z) {
                mPendingEvent.acceleration.z = value * CONVERT_A_Z;
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            ALOGE("%s: unknown event (type=%d, code=%d)", LOGTAG,
                    type, event->code);
        }

        mInputReader.next();
    }
    return numEventReceived++;

}

int AccelSensor::flush(int handle)
{
    if (mEnabled){
        flush_state |= (1 << handle);
        ALOGE("AccelSensor: %s: handle: %d", __func__, handle);
        return 0;
    }
    return -EINVAL;
}