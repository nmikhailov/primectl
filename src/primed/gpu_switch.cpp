/*
 * gpu_switch.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: nsl
 */

#include "gpu_switch.hpp"

#include <iostream>
#include <fstream>

#include <mntent.h>
#include <sys/stat.h>
#include <sys/mount.h>

GPUSwitch::GPUSwitch(Settings& prop) :
        m_settings(prop) {
}

GPUSwitch::~GPUSwitch() {
}

void GPUSwitch::setSecondaryEnabled(bool enabled) const {
    std::string path = getDebugfsMountPoint() + "/vgaswitcheroo/switch";

    std::ofstream out(path);
    out << (enabled ? "ON" : "OFF") << std::endl;
    out.close();

    std::cout << "GPUSwithch: Secondary GPU is "
            << (enabled ? "enabled" : "disabled") << std::endl;
}

bool GPUSwitch::enabled() const {
    std::string path = getDebugfsMountPoint() + "/vgaswitcheroo/switch";

    std::ifstream in(path);
    std::string text((std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
    in.close();

    size_t pos = 0, cnt = 0;
    while ((pos = text.find("Pwr", pos)) != std::string::npos) {
        cnt++;
        pos++;
    }

    return cnt > 2;
}

std::string GPUSwitch::getDebugfsMountPoint() const {
    std::string mountdir;
    bool mounted = false;
    // TODO: error handling
    // TODO: check for debugfs kernel support

    FILE *f = setmntent("/etc/mtab", "r");
    if (!f) {
        std::cout << "Error, can't open /etc/mtab for reading" << std::endl;
        // Error
        perror("stat");
    }

    // Iterate mount points
    mntent *ent;
    while ((ent = getmntent(f)) != NULL) {
        if (strcmp(ent->mnt_type, "debugfs") == 0) {
            if (!hasmntopt(ent, MNTOPT_RO)) {
                mountdir = ent->mnt_dir;
                mounted = true;
                break;
            }
        }
    }

    // Mount debugfs
    if (!mounted) {
        std::string path = m_settings.get<std::string>(
                "gpuswitch.debugfs_mountpoint", "/sys/kernel/debug");

        struct stat statbuf;
        int err = stat(mountdir.c_str(), &statbuf);
        if (err != ENOENT) {
            // Use temp dir
            char dtempl[] = "/tmp/debugfs.XXXXXX";
            mkdtemp(dtempl);
            mountdir = dtempl;
        }
        //
        if (!mount(NULL, mountdir.c_str(), "debugfs", 0, NULL)) {
            // OK
        } else {
            perror("mount");
        }
    }

    return mountdir;
}
