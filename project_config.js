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

//-- Target MSX machine version (string): "1" = MSX1
Machine = "1";

//-- Target program format (string): "ROM_32K" = plain 32 KB ROM at 4000h
Target = "ROM_32K";
