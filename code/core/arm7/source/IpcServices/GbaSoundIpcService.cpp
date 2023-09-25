#include "common.h"
#include "GbaSound.h"
#include "Sound/GbSound.h"
#include "Sound/GbaSound7.h"
#include "Sound/SoundThread.h"
#include "GbaSoundIpcService.h"

void GbaSoundIpcService::OnMessageReceived(u32 data)
{
    auto cmd = static_cast<GbaSoundIpcCommand>(data & 0xF);
    switch (cmd)
    {
        case GBA_SOUND_IPC_CMD_SETUP:
        {
            auto sharedData = reinterpret_cast<gbas_shared_t*>((data >> 4) << 5);
            gbas_init(sharedData);
            sndt_start();
            SendResponseMessage(0);
            break;
        }
        case GBA_SOUND_IPC_CMD_SET_TIMER_RELOAD:
        {
            u32 timer = (data >> 4) & 1;
            u16 reload = data >> 5;
            gbas_setTimerReload(timer, reload);
            break;
        }
        case GBA_SOUND_IPC_CMD_SET_TIMER_CONTROL:
        {
            u32 timer = (data >> 4) & 1;
            u16 control = data >> 5;
            gbas_setTimerControl(timer, control);
            break;
        }
        case GBA_SOUND_IPC_CMD_GB_REG_WRITE_8:
        {
            u32 reg = (data >> 4) & 0xFF;
            u32 value = (data >> 12) & 0xFF;
            gbs_writeReg(reg, value);
            break;
        }
        case GBA_SOUND_IPC_CMD_GB_REG_WRITE_16:
        {
            u32 reg = (data >> 4) & 0xFF;
            u32 value = (data >> 12) & 0xFFFF;
            gbs_writeReg(reg, value & 0xFF);
            gbs_writeReg(reg + 1, value >> 8);
            break;
        }
    }
}
