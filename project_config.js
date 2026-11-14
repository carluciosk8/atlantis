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
LibModules = [ "system", "bios", "vdp" ];

//-- Additional project sources to compile and link (array)
AddSources = [ "tiles.c", "city.c", "hud.c", "wave.c", "score.c", "sprites.c", "queue.c", "gorgons.c", "difficulty.c", "lane.c", "torpedo.c", "collision.c", "input.c" ];

//-- Target MSX machine version (string): "1" = MSX1
Machine = "1";

//-- Target program format (string): "ROM_32K" = plain 32 KB ROM at 4000h
Target = "ROM_32K";
