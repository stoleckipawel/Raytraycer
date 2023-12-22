-- premake5.lua
workspace "Raytraycer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Raytraycer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "Raytraycer"