// ____________________________
//  Atlantis — MSXgl project configuration
//─────────────────────────────────────────────────────────────────────────────

//*******************************************************************************
// PROJECT SETTINGS
//*******************************************************************************

//-- Project name (string). Will be used for the output filename
ProjName = "atlantis";

//-- List of project modules to build (array). If empty, ProjName will be added
ProjModules = [ ProjName ];

//-- List of library modules to build (array)
LibModules = [ "system", "bios", "vdp", "math", "arkos/akm_player", "input" ];

//-- Additional project sources to compile and link (array)
SourceDir = "src";

AddSources =
[
    `${SourceDir}/platform/frame.c`,
    `${SourceDir}/game/flow.c`,
    `${SourceDir}/platform/tiles.c`,
    `${SourceDir}/game/city.c`,
    `${SourceDir}/game/sky.c`,
    `${SourceDir}/ui/hud.c`,
    `${SourceDir}/ui/screens.c`,
    `${SourceDir}/ui/flyby.c`,
    `${SourceDir}/game/wave.c`,
    `${SourceDir}/game/score.c`,
    `${SourceDir}/platform/sprites.c`,
    `${SourceDir}/game/queue.c`,
    `${SourceDir}/game/gorgons.c`,
    `${SourceDir}/game/difficulty.c`,
    `${SourceDir}/game/lane.c`,
    `${SourceDir}/game/torpedo.c`,
    `${SourceDir}/game/collision.c`,
    `${SourceDir}/game/deathray.c`,
    `${SourceDir}/game/controls.c`,
    `${SourceDir}/platform/music.c`,
    `${SourceDir}/data/sprite_data.c`,
    `${SourceDir}/data/city_data.c`,
    `${SourceDir}/data/sky_data.c`,
    `${SourceDir}/data/logo_data.c`,
    `${SourceDir}/data/text_data.c`,
];

// Headers are included by bare name, so every source directory is on the include path.
CompileOpt = `-I${SourceDir} -I${SourceDir}/platform -I${SourceDir}/game -I${SourceDir}/ui -I${SourceDir}/data`;

//-- Target MSX machine version (string): "1" = MSX1
Machine = "1";

//-- Target program format (string): "ROM_32K" = plain 32 KB ROM at 4000h
Target = "ROM_32K";
