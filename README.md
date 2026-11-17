Scooby-Doo! Night of 100 Frights (GameCube)  
[![Build Status]][actions] [![GIHE78_00 Progress]][treemap] <!--[![Code Progress]][progress] [![Data Progress]][progress]--> [![Discord Badge]][discord]
=============

[Build Status]: https://github.com/seilc/n100f/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/seilc/n100f/actions/workflows/build.yml
[GIHE78_00 Progress]: https://img.shields.io/endpoint?url=https%3A%2F%2Fseilc.github.io%2Fn100f%2FGIHE78_00.json
[treemap]: https://seilc.github.io/n100f/treemap.png
<!--
decomp.dev progress badges (hidden until the project is registered on decomp.dev)
See https://decomp.dev/api for an API overview.
-->
[Code Progress]: https://decomp.dev/seilc/n100f.svg?mode=shield&measure=code&label=Code
[Data Progress]: https://decomp.dev/seilc/n100f.svg?mode=shield&measure=data&label=Data
[progress]: https://decomp.dev/seilc/n100f
[Discord Badge]: https://img.shields.io/discord/829152115322257436?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/dVbGFdYU6A

A work-in-progress matching decompilation of *Scooby-Doo! Night of 100 Frights* for the GameCube (USA, `GIHE78`),
built on Heavy Iron's EvilEngine. Much of the engine is shared with the
[Ratatouille prototype decomp](https://github.com/seilc/rat), [Rise of the Underminer](https://github.com/seilc/rotu),
[The Incredibles](https://github.com/seilc/incredibles), [The SpongeBob SquarePants Movie](https://github.com/seilc/tssm)
and [Battle for Bikini Bottom](https://github.com/seilc/bfbb), which are decompiled side by side with this project.

![Progress treemap][treemap]

This repository does **not** contain any game assets or assembly whatsoever. An existing copy of the game is required.

Full disclosure: this project is AI-assisted. It was set up with the help of an LLM, and new code is decompiled with
its help, then reviewed and checked against the original binary. Matching code is byte-identical to the original, but
names, types and source structure beyond what the binary and its debug info prove may be guesses.

Supported versions:

- `GIHE78_00` - Scooby-Doo! Night of 100 Frights (USA), revision 0, retail `main.dol`

Building
========

- Install Python 3 and ninja (on Linux/macOS also [wibo](https://github.com/decompals/wibo) or wine; `configure.py`
  downloads wibo automatically).
- Copy `sys/main.dol` from the disc to `orig/GIHE78_00/sys/main.dol` (SHA-1 `c77fb29d39b5898933e8ef4d1ccd2ddac404554e`).
- Run `python configure.py`, then `ninja`.

The project is built with [decomp-toolkit](https://github.com/encounter/decomp-toolkit) on the
[dtk-template](https://github.com/encounter/dtk-template) layout; [objdiff](https://github.com/encounter/objdiff)
reads the generated `objdiff.json`. The DOL's entry point is a Heavy Iron wrapper around `__start`, so the build splits
a copy of the DOL with `__start` as its entry (`tools/patch_entry.py`).

Credits
=======

**[mitsevox/n100f](https://github.com/mitsevox/n100f)** -
An independent decompilation of this game, whose build notes were read for comparison; none of its code is used here.

Code ported from the Ratatouille, Rise of the Underminer, Incredibles, SpongeBob Movie and Battle for Bikini Bottom
decomps is credited in those repositories.
