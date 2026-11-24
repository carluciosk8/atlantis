#include "frame.h"

#include "arkos/akm_player.h"   // AKM_Update (called by the H_TIMI hook)


volatile u8 g_FrameCounter;


//-----------------------------------------------------------------------------
// H_TIMI hook: called by the BIOS KEYINT ISR once per vblank. Increments the
// frame counter, then advances the music player. The BIOS KEYINT has already
// saved all registers around the hook, but expects A (a copy of VDP status
// register S#0) to be preserved for STATFL, so this __naked wrapper pushes AF
// around its work. It must not touch the VDP.
static void MusicFrameHook(void) __naked
{
    __asm
        push    af
        ld      hl, #_g_FrameCounter
        inc     (hl)
        call    _AKM_Update
        pop     af
        ret
    __endasm;
}


//-----------------------------------------------------------------------------
// Interrupts are disabled while the hook is written so a vblank cannot run a
// half-written hook (BIOS_SetHookCallback does not DI/EI itself).
void frame_install_hook(void)
{
    g_FrameCounter = 0;
    DisableInterrupt();
    BIOS_SetHookCallback(H_TIMI, MusicFrameHook);
    EnableInterrupt();
}
