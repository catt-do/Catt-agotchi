#!/usr/bin/env python3
"""
Convert a PNG sprite into a C header with a RGB565 pixel array.

Usage:
    python3 png_to_rgb565.py <input.png> <c_file_name> <target_width> <target_height>
"""
import sys
import os
from PIL import Image, UnidentifiedImageError


def fail(message):
    print(f"Error: {message}\n", file=sys.stderr)
    print(__doc__, file=sys.stderr)
    sys.exit(1)


def convert(png_path, name, width, height):
    if not os.path.isfile(png_path):
        fail(f"input file not found: '{png_path}'")

    if not name.isidentifier():
        fail(f"'{name}' is not a valid C identifier")

    if width <= 0 or height <= 0:
        fail(f"width and height must be positive, got {width}x{height}")

    try:
        img = Image.open(png_path)
    except UnidentifiedImageError:
        fail(f"'{png_path}' is not a valid image file")
    except OSError as e:
        fail(f"couldn't open '{png_path}': {e}")

    img = img.convert("RGBA").resize((width, height))

    # Chroma key: fully transparent pixels become this color in output
    LIME_GREEN = 0x00FF00
    KEY_R = (LIME_GREEN >> 16) & 0xFF
    KEY_G = (LIME_GREEN >> 8) & 0xFF
    KEY_B = LIME_GREEN & 0xFF

    pixels = []
    for y in range(height):
        for x in range(width):
            r, g, b, a = img.getpixel((x, y))
            if a < 128:  # treat mostly transparent pixels as fully transparent
                r, g, b = KEY_R, KEY_G, KEY_B
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            pixels.append(rgb565)

    out_path = f"{name}.h"
    try:
        with open(out_path, "w") as f:
            f.write("#pragma once\n")
            f.write("#include <stdint.h>\n\n")
            f.write(f"#define {name.upper()}_WIDTH  {width}\n")
            f.write(f"#define {name.upper()}_HEIGHT {height}\n")
            key_rgb565 = ((KEY_R & 0xF8) << 8) | ((KEY_G & 0xFC) << 3) | (KEY_B >> 3)
            f.write(f"#define {name.upper()}_TRANSPARENT_KEY 0x{key_rgb565:04X}\n\n")
            f.write(f"static const uint16_t {name}_data[{width} * {height}] = {{\n")
            for i in range(0, len(pixels), 12):
                row = pixels[i:i + 12]
                f.write("    " + ", ".join(f"0x{p:04X}" for p in row) + ",\n")
            f.write("};\n")
    except OSError as e:
        fail(f"couldn't write '{out_path}': {e}")

    size_bytes = width * height * 2
    print(f"Wrote {out_path}  ({width}x{height}, {size_bytes:,} bytes / {size_bytes/1024:.1f} KB)")


def parse_int(value, label):
    try:
        return int(value)
    except ValueError:
        fail(f"{label} must be an integer, got '{value}'")


if __name__ == "__main__":
    if len(sys.argv) != 5:
        fail(f"expected 4 arguments, got {len(sys.argv) - 1}")

    png_path = sys.argv[1]
    name = sys.argv[2]
    width = parse_int(sys.argv[3], "width")
    height = parse_int(sys.argv[4], "height")

    convert(png_path, name, width, height)
