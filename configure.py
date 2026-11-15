#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GIHE78_00",  # 0: Rev 0 (disc revision 0, built Apr 16 2002)
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-2"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.3"
config.objdiff_tag = "v3.6.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Game code flags: CW GC 1.3.2 (Scooby's game code compiler). BFBB's game code flags
# (its configure.py) minus `-pragma "cpp_extensions on"` and `-gccinc` (set for BFBB's own
# source; open for Scooby), and `-str reuse,readonly` without `pool` (more Scooby functions
# reproduced without it). Unlike BFBB: `-char signed` (Scooby sign-extends chars), `-O4,s`
# (lmw/stmw for two registers) and `-common off` (xPar's gParPool sits in its object's .bss).
cflags_game = [
    *cflags_base,
    "-lang=c++",
    "-common off",
    "-char signed",
    "-str reuse,readonly",
    "-use_lmw_stmw on",
    "-O4,s",
    "-inline auto,deferred",
    "-i src/dolphin/include",
    "-i src/dolphin/include/libc",
    "-i src/SD2/Game/flat",
    "-i src/rwsdk/include",
    "-prefix prefix.mch",
]

config.linker_version = "GC/1.3.2"

# Game code prefix: include/prefix.pch++, precompiled with the game flags into
# build/<version>/include/prefix.mch (found through -i build/<version>/include) and passed to
# every game unit with -prefix (cflags_game).
config.precompiled_headers = [
    {
        "source": "prefix.pch++",
        "mw_version": "GC/1.3.2",
        "cflags": [f for f in cflags_game if f != "-prefix prefix.mch"],
    }
]
# decomp.me contexts read the prefix header itself (scratches can't use the .mch).
config.context_prefixes = {"prefix.mch": Path("include/prefix.pch++")}

# Dolphin SDK flags (dolsdk2001-dec's Makefile, release libraries; Dec 17 2001 SDK).
cflags_sdk = [
    "-O4,p",
    "-inline auto",
    "-sym on",
    "-char unsigned",
    "-nodefaults",
    "-proc gekko",
    "-fp hard",
    "-Cpp_exceptions off",
    "-enum int",
    "-warn pragmas",
    "-requireprotos",
    '-pragma "cats off"',
    "-nosyspath",
    "-i src/dolphin/include",
    "-i src/dolphin/include/libc",
    "-ir src/dolphin/src",
    "-DRELEASE",
]
sdk_char_signed = ["-char signed"]

# Runtime library flags (BFBB's and the later games'); its compiler is not known yet.
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# The DOL's entry is HI's stack-fill wrapper, which calls __start: dtk splits a copy whose
# entry is __start, and the link names the wrapper as the entry.
config.split_entry_patch = (
    Path("orig") / config.version / "sys" / "main.dol",
    config.build_dir / config.version / "orig" / "main.dol",
    0x80005304,
)
config.ldflags.append("-m fn_80016898")


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk,
        "progress_category": "sdk",
        "objects": objects,
    }


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "SD2",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_game,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "SD2/Game/flat/iAnimSKB.cpp"),
            Object(NonMatching, "SD2/Game/flat/iCamera.cpp"),
            Object(NonMatching, "SD2/Game/flat/iCollide.cpp"),
            Object(Matching, "SD2/Game/flat/iCollideFast.cpp"),
            Object(NonMatching, "SD2/Game/flat/iDebug.cpp"),
            Object(Matching, "SD2/Game/flat/iDraw.cpp"),
            Object(NonMatching, "SD2/Game/flat/iEnv.cpp"),
            Object(NonMatching, "SD2/Game/flat/iFile.cpp"),
            Object(NonMatching, "SD2/Game/flat/iFont.cpp"),
            Object(NonMatching, "SD2/Game/flat/iLight.cpp"),
            Object(NonMatching, "SD2/Game/flat/iMath.cpp"),
            Object(NonMatching, "SD2/Game/flat/iMath3.cpp"),
            Object(NonMatching, "SD2/Game/flat/iMemMgr.cpp"),
            Object(NonMatching, "SD2/Game/flat/iModel.cpp"),
            Object(NonMatching, "SD2/Game/flat/iMorph.cpp"),
            Object(NonMatching, "SD2/Game/flat/iPad.cpp"),
            Object(NonMatching, "SD2/Game/flat/iParMgr.cpp"),
            Object(NonMatching, "SD2/Game/flat/isavegame.cpp"),
            Object(NonMatching, "SD2/Game/flat/iScrFX.cpp"),
            Object(NonMatching, "SD2/Game/flat/iShadow.cpp"),
            Object(NonMatching, "SD2/Game/flat/iSystem.cpp"),
            Object(NonMatching, "SD2/Game/flat/iTime.cpp"),
            Object(NonMatching, "SD2/Game/flat/xAnim.cpp"),
            Object(Matching, "SD2/Game/flat/xBase.cpp"),
            Object(NonMatching, "SD2/Game/flat/xbinio.cpp"),
            Object(NonMatching, "SD2/Game/flat/xBound.cpp"),
            Object(NonMatching, "SD2/Game/flat/xCamera.cpp"),
            Object(NonMatching, "SD2/Game/flat/xClimate.cpp"),
            Object(NonMatching, "SD2/Game/flat/xCollide.cpp"),
            Object(NonMatching, "SD2/Game/flat/xCollideFast.cpp"),
            Object(NonMatching, "SD2/Game/flat/xDebug.cpp"),
            Object(Matching, "SD2/Game/flat/xDraw.cpp"),
            Object(NonMatching, "SD2/Game/flat/xEnt.cpp"),
            Object(NonMatching, "SD2/Game/flat/xEntDrive.cpp"),
            Object(NonMatching, "SD2/Game/flat/xEntMotion.cpp"),
            Object(Matching, "SD2/Game/flat/xEnv.cpp"),
            Object(NonMatching, "SD2/Game/flat/xFFX.cpp"),
            Object(NonMatching, "SD2/Game/flat/xFont.cpp"),
            Object(NonMatching, "SD2/Game/flat/xFX.cpp"),
            Object(NonMatching, "SD2/Game/flat/xhipio.cpp"),
            Object(NonMatching, "SD2/Game/flat/xIni.cpp"),
            Object(Matching, "SD2/Game/flat/xLinkList.cpp"),
            Object(NonMatching, "SD2/Game/flat/xMath.cpp"),
            Object(NonMatching, "SD2/Game/flat/xMath3.cpp"),
            Object(NonMatching, "SD2/Game/flat/xMemMgr.cpp"),
            Object(NonMatching, "SD2/Game/flat/xModel.cpp"),
            Object(NonMatching, "SD2/Game/flat/xMorph.cpp"),
            Object(NonMatching, "SD2/Game/flat/xMovePoint.cpp"),
            Object(NonMatching, "SD2/Game/flat/xordarray.cpp"),
            Object(NonMatching, "SD2/Game/flat/xPad.cpp"),
            Object(Matching, "SD2/Game/flat/xPar.cpp"),
            Object(NonMatching, "SD2/Game/flat/xParGroup.cpp"),
            Object(Matching, "SD2/Game/flat/xParMgr.cpp"),
            Object(NonMatching, "SD2/Game/flat/xPartition.cpp"),
            Object(NonMatching, "SD2/Game/flat/xpkrsvc.cpp"),
            Object(Matching, "SD2/Game/flat/xQuickCull.cpp"),
            Object(NonMatching, "SD2/Game/flat/xsavegame.cpp"),
            Object(NonMatching, "SD2/Game/flat/xScene.cpp"),
            Object(NonMatching, "SD2/Game/flat/xScrFx.cpp"),
            Object(NonMatching, "SD2/Game/flat/xserializer.cpp"),
            Object(NonMatching, "SD2/Game/flat/xShadow.cpp"),
            Object(NonMatching, "SD2/Game/flat/xSnd.cpp"),
            Object(NonMatching, "SD2/Game/flat/xSpline.cpp"),
            Object(NonMatching, "SD2/Game/flat/xstransvc.cpp"),
            Object(NonMatching, "SD2/Game/flat/xString.cpp"),
            Object(Matching, "SD2/Game/flat/xSurface.cpp"),
            Object(NonMatching, "SD2/Game/flat/xTRC.cpp"),
            Object(NonMatching, "SD2/Game/flat/xutil.cpp"),
            Object(NonMatching, "SD2/Game/flat/zActionLine.cpp"),
            Object(NonMatching, "SD2/Game/flat/zAnimList.cpp"),
            Object(NonMatching, "SD2/Game/flat/zAssetTypes.cpp"),
            Object(NonMatching, "SD2/Game/flat/zCamera.cpp"),
            Object(NonMatching, "SD2/Game/flat/zConditional.cpp"),
            Object(NonMatching, "SD2/Game/flat/zCounter.cpp"),
            Object(NonMatching, "SD2/Game/flat/zCutsceneMgr.cpp"),
            Object(NonMatching, "SD2/Game/flat/zDispatcher.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEGenerator.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEnt.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntBubble.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntButton.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntDestructObj.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntHangable.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntPickup.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntPlayer.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntProjectile.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntShaggy.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntSimpleObj.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntTrigger.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEntVillain.cpp"),
            Object(NonMatching, "SD2/Game/flat/zEnv.cpp"),
            Object(NonMatching, "SD2/Game/flat/zFeet.cpp"),
            Object(Matching, "SD2/Game/flat/zFMV.cpp"),
            Object(NonMatching, "SD2/Game/flat/zFog.cpp"),
            Object(NonMatching, "SD2/Game/flat/zFX.cpp"),
            Object(NonMatching, "SD2/Game/flat/zGame.cpp"),
            Object(NonMatching, "SD2/Game/flat/zGameExtras.cpp"),
            Object(NonMatching, "SD2/Game/flat/zGameState.cpp"),
            Object(NonMatching, "SD2/Game/flat/zGroup.cpp"),
            Object(NonMatching, "SD2/Game/flat/zGust.cpp"),
            Object(NonMatching, "SD2/Game/flat/zHologram.cpp"),
            Object(NonMatching, "SD2/Game/flat/zHud.cpp"),
            Object(NonMatching, "SD2/Game/flat/zLaugh.cpp"),
            Object(NonMatching, "SD2/Game/flat/zLight.cpp"),
            Object(NonMatching, "SD2/Game/flat/zLightEffect.cpp"),
            Object(NonMatching, "SD2/Game/flat/zLightning.cpp"),
            Object(NonMatching, "SD2/Game/flat/zLobMaster.cpp"),
            Object(NonMatching, "SD2/Game/flat/zMain.cpp"),
            Object(NonMatching, "SD2/Game/flat/zMenu.cpp"),
            Object(NonMatching, "SD2/Game/flat/zMovePoint.cpp"),
            Object(NonMatching, "SD2/Game/flat/zMusic.cpp"),
            Object(NonMatching, "SD2/Game/flat/zParCmd.cpp"),
            Object(NonMatching, "SD2/Game/flat/zParEmitter.cpp"),
            Object(NonMatching, "SD2/Game/flat/zParSys.cpp"),
            Object(NonMatching, "SD2/Game/flat/zPendulum.cpp"),
            Object(NonMatching, "SD2/Game/flat/zPlatform.cpp"),
            Object(NonMatching, "SD2/Game/flat/zPortal.cpp"),
            Object(NonMatching, "SD2/Game/flat/zRenderState.cpp"),
            Object(NonMatching, "SD2/Game/flat/zRumble.cpp"),
            Object(NonMatching, "SD2/Game/flat/zSaveLoad.cpp"),
            Object(NonMatching, "SD2/Game/flat/zScene.cpp"),
            Object(NonMatching, "SD2/Game/flat/zScript.cpp"),
            Object(NonMatching, "SD2/Game/flat/zSFX.cpp"),
            Object(NonMatching, "SD2/Game/flat/zSurface.cpp"),
            Object(NonMatching, "SD2/Game/flat/zTimer.cpp"),
            Object(NonMatching, "SD2/Game/flat/zUI.cpp"),
            Object(NonMatching, "SD2/Game/flat/zUIFont.cpp"),
            Object(NonMatching, "SD2/Game/flat/zVar.cpp"),
            Object(NonMatching, "SD2/Game/flat/iAnim.cpp"),
            Object(NonMatching, "SD2/Game/flat/iSnd.cpp"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
        ],
    },
    DolphinLib(
        "ai",
        [
            Object(Matching, "dolphin/src/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "amcstubs",
        [
            Object(Matching, "dolphin/src/amcstubs/AmcExi2Stubs.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(NonMatching, "dolphin/src/ar/ar.c"),
            Object(Matching, "dolphin/src/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "ax",
        [
            Object(Matching, "dolphin/src/ax/AX.c"),
            Object(NonMatching, "dolphin/src/ax/AXAlloc.c"),
            Object(NonMatching, "dolphin/src/ax/AXAux.c"),
            Object(NonMatching, "dolphin/src/ax/AXCL.c"),
            Object(NonMatching, "dolphin/src/ax/AXOut.c"),
            Object(Matching, "dolphin/src/ax/AXSPB.c"),
            Object(NonMatching, "dolphin/src/ax/AXVPB.c"),
            Object(Matching, "dolphin/src/ax/AXProf.c"),
            Object(Matching, "dolphin/src/ax/DSPCode.c"),
        ],
    ),
    DolphinLib(
        "base",
        [
            Object(Matching, "dolphin/src/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "dolphin/src/db/db.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(NonMatching, "dolphin/src/dsp/dsp.c"),
            Object(NonMatching, "dolphin/src/dsp/dsp_debug.c"),
            Object(NonMatching, "dolphin/src/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "exi",
        [
            Object(NonMatching, "dolphin/src/exi/EXIBios.c"),
            Object(Matching, "dolphin/src/exi/EXIUart.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(Matching, "dolphin/src/gx/GXInit.c"),
            Object(NonMatching, "dolphin/src/gx/GXFifo.c"),
            Object(Matching, "dolphin/src/gx/GXAttr.c"),
            Object(Matching, "dolphin/src/gx/GXMisc.c"),
            Object(Matching, "dolphin/src/gx/GXGeometry.c"),
            Object(Matching, "dolphin/src/gx/GXFrameBuf.c"),
            Object(Matching, "dolphin/src/gx/GXLight.c"),
            Object(Matching, "dolphin/src/gx/GXTexture.c"),
            Object(Matching, "dolphin/src/gx/GXBump.c"),
            Object(Matching, "dolphin/src/gx/GXTev.c"),
            Object(Matching, "dolphin/src/gx/GXPixel.c"),
            Object(Matching, "dolphin/src/gx/GXStubs.c"),
            Object(Matching, "dolphin/src/gx/GXDisplayList.c"),
            Object(Matching, "dolphin/src/gx/GXTransform.c"),
            Object(Matching, "dolphin/src/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "dolphin/src/mtx/mtx.c", extra_cflags=sdk_char_signed),
            Object(Matching, "dolphin/src/mtx/mtx44.c", extra_cflags=sdk_char_signed),
        ],
    ),
    DolphinLib(
        "odenotstub",
        [
            Object(Matching, "dolphin/src/odenotstub/odenotstub.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(NonMatching, "dolphin/src/os/OS.c"),
            Object(Matching, "dolphin/src/os/OSAddress.c"),
            Object(NonMatching, "dolphin/src/os/OSAlarm.c"),
            Object(Matching, "dolphin/src/os/OSAlloc.c"),
            Object(Matching, "dolphin/src/os/OSArena.c"),
            Object(NonMatching, "dolphin/src/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/src/os/OSCache.c"),
            Object(Matching, "dolphin/src/os/OSContext.c"),
            Object(NonMatching, "dolphin/src/os/OSError.c"),
            Object(Matching, "dolphin/src/os/OSFont.c"),
            Object(NonMatching, "dolphin/src/os/OSInterrupt.c"),
            Object(Matching, "dolphin/src/os/OSLink.c"),
            Object(NonMatching, "dolphin/src/os/OSMemory.c"),
            Object(Matching, "dolphin/src/os/OSMutex.c"),
            Object(NonMatching, "dolphin/src/os/OSReboot.c"),
            Object(NonMatching, "dolphin/src/os/OSReset.c"),
            Object(NonMatching, "dolphin/src/os/OSResetSW.c"),
            Object(NonMatching, "dolphin/src/os/OSRtc.c"),
            Object(Matching, "dolphin/src/os/OSSync.c"),
            Object(Matching, "dolphin/src/os/OSThread.c"),
            Object(Matching, "dolphin/src/os/OSTime.c"),
            Object(Matching, "dolphin/src/os/__ppc_eabi_init.c"),
        ],
    ),
    DolphinLib(
        "si",
        [
            Object(Matching, "dolphin/src/si/SIBios.c"),
            Object(Matching, "dolphin/src/si/SISamplingRate.c"),
        ],
    ),
    DolphinLib(
        "vi",
        [
            Object(NonMatching, "dolphin/src/vi/vi.c"),
        ],
    ),
    DolphinLib(
        "pad",
        [
            Object(Matching, "dolphin/src/pad/Padclamp.c"),
            Object(Matching, "dolphin/src/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(Matching, "dolphin/src/dvd/dvdlow.c"),
            Object(Matching, "dolphin/src/dvd/dvdfs.c", extra_cflags=sdk_char_signed),
            Object(Matching, "dolphin/src/dvd/dvd.c"),
            Object(Matching, "dolphin/src/dvd/dvdqueue.c"),
            Object(Matching, "dolphin/src/dvd/dvderror.c"),
            Object(Matching, "dolphin/src/dvd/fstload.c", extra_cflags=sdk_char_signed),
        ],
    ),
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects


# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
