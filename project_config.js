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
    `${SourceDir}/tiles.c`,
    `${SourceDir}/city.c`,
    `${SourceDir}/hud.c`,
    `${SourceDir}/wave.c`,
    `${SourceDir}/score.c`,
    `${SourceDir}/sprites.c`,
    `${SourceDir}/queue.c`,
    `${SourceDir}/gorgons.c`,
    `${SourceDir}/difficulty.c`,
    `${SourceDir}/lane.c`,
    `${SourceDir}/torpedo.c`,
    `${SourceDir}/collision.c`,
    `${SourceDir}/controls.c`,
    `${SourceDir}/music.c`,
];

CompileOpt = `-I${SourceDir}`;

//-- Target MSX machine version (string): "1" = MSX1
Machine = "1";

//-- Target program format (string): "ROM_32K" = plain 32 KB ROM at 4000h
Target = "ROM_32K";
