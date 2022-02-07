/*
DingusPPC - The Experimental PowerPC Macintosh emulator
Copyright (C) 2018-22 divingkatae and maximum
                      (theweirdo)     spatium

(Contact divingkatae#1017 or powermax#2286 on Discord for more info)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/** @file Macintosh Superdrive definitions. */

#ifndef MAC_SUPERDRIVE_H
#define MAC_SUPERDRIVE_H

#include <devices/common/hwcomponent.h>
#include <devices/floppy/floppyimg.h>

#include <cinttypes>
#include <memory>
#include <string>

namespace MacSuperdrive {

/** Apple Drive status request addresses. */
enum StatusAddr : uint8_t {
    Step_Status   = 1,
    Motor_Status  = 2,
    Eject_Latch   = 3,
    MFM_Support   = 5,
    Double_Sided  = 6,
    Drive_Exists  = 7,
    Disk_In_Drive = 8,
    Write_Protect = 9,
    Track_Zero    = 0xA,
    Drive_Mode    = 0xD,
    Drive_Ready   = 0xE,
    Media_Kind    = 0xF
};

/** Apple Drive command addresses. */
enum CommandAddr : uint8_t {
    Step_Direction    = 0,
    Do_Step           = 1,
    Motor_On_Off      = 2,
    Reset_Eject_Latch = 4,
    Switch_Drive_Mode = 5,
};

/** Type of media currently in the drive. */
enum MediaKind : uint8_t {
    low_density  = 0,
    high_density = 1, // 1 or 2 MB disk
};

/** Disk recording method. */
enum RecMethod : int {
    GCR = 0,
    MFM = 1
};

class MacSuperDrive : public HWComponent {
public:
    MacSuperDrive();
    ~MacSuperDrive() = default;

    void command(uint8_t addr, uint8_t value);
    uint8_t status(uint8_t addr);
    int insert_disk(std::string& img_path);

protected:
    void set_disk_phys_params();
    void switch_drive_mode(int mode);

private:
    uint8_t has_disk;
    uint8_t eject_latch;
    uint8_t motor_stat;  // spindle motor status: 1 - on, 0 - off
    uint8_t drive_mode;  // drive mode: 0 - GCR, 1 - MFM
    uint8_t is_ready;
    uint8_t track_zero;  // 1 - if head is at track zero
    int     step_dir;    // step direction -1/+1
    int     head_pos;    // track number the head is currently at

    // physical parameters of the currently inserted disk
    uint8_t media_kind;
    uint8_t wr_protect;
    int     rec_method;
    int     num_tracks;
    int     num_sides;
    int     sectors_per_track[80];
    int     rpm_per_track[80];
    int     track_start_block[80]; // logical block number of the first sector in a track

    std::unique_ptr<FloppyImgConverter>  img_conv;

    std::unique_ptr<char[]> disk_data;
};

}; // namespace MacSuperdrive

#endif // MAC_SUPERDRIVE_H