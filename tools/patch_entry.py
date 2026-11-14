#!/usr/bin/env python3

###
# Writes a copy of a DOL with its header entry point set to ADDRESS.
#
# Usage:
#   python3 tools/patch_entry.py IN.dol OUT.dol ADDRESS
###

import argparse
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Writes a copy of a DOL with its header entry point set to ADDRESS."
    )
    parser.add_argument("dol_in", type=Path, help="input DOL")
    parser.add_argument("dol_out", type=Path, help="output DOL")
    parser.add_argument("address", type=lambda x: int(x, 0), help="entry point")
    args = parser.parse_args()

    data = bytearray(args.dol_in.read_bytes())
    data[0xE0:0xE4] = args.address.to_bytes(4, "big")
    args.dol_out.parent.mkdir(parents=True, exist_ok=True)
    args.dol_out.write_bytes(data)


if __name__ == "__main__":
    main()
