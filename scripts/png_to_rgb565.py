#!/usr/bin/env python3
"""
Convert one or more PNG frames of a sprite into a C header with RGB565 pixel arrays.

Every PNG is authored at display scale (e.g. 240x320) so the sprite sits exactly where
it appears on screen. The output is cropped to the bounding box of the non-transparent
pixels of ALL frames, so the sprite keeps its on-screen position (x, y) without
storing the empty canvas around it. All frames share the same box, which lets the
display diff consecutive frames pixel for pixel.

Usage:
    python3 png_to_rgb565.py <name> <display_width> <display_height> <frame.png> [<frame.png> ...]

Output: <name>.h with <NAME>_FRAME_COUNT and a `static const sprite_t <name>_frames[]`
"""
import sys
import os
from PIL import Image, UnidentifiedImageError

# Chroma key: fully transparent pixels become this color in output.
# Must match SPRITE_TRANSPARENT_KEY in components/display/sprite.h
LIME_GREEN = 0x00FF00
KEY_R = (LIME_GREEN >> 16) & 0xFF
KEY_G = (LIME_GREEN >> 8) & 0xFF
KEY_B = LIME_GREEN & 0xFF
KEY_RGB565 = ((KEY_R & 0xF8) << 8) | ((KEY_G & 0xFC) << 3) | (KEY_B >> 3)


def fail(message):
    print(f"Error: {message}\n", file=sys.stderr)
    print(__doc__, file=sys.stderr)
    sys.exit(1)


def load_frame(png_path, width, height):
    if not os.path.isfile(png_path):
        fail(f"input file not found: '{png_path}'")

    try:
        img = Image.open(png_path)
    except UnidentifiedImageError:
        fail(f"'{png_path}' is not a valid image file")
    except OSError as e:
        fail(f"couldn't open '{png_path}': {e}")

    img = img.convert("RGBA")
    if img.size != (width, height):
        img = img.resize((width, height))
    return img


def opaque_bbox(img):
    # treat mostly transparent pixels as fully transparent
    alpha = img.getchannel("A").point(lambda a: 255 if a >= 128 else 0)
    return alpha.getbbox()  # (left, top, right, bottom) or None


def to_rgb565(img, box):
    pixels = []
    left, top, right, bottom = box
    for y in range(top, bottom):
        for x in range(left, right):
            r, g, b, a = img.getpixel((x, y))
            if a < 128:
                pixels.append(KEY_RGB565)
            else:
                pixels.append(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
    return pixels


def convert(name, width, height, png_paths):
    if not name.isidentifier():
        fail(f"'{name}' is not a valid C identifier")

    if width <= 0 or height <= 0:
        fail(f"width and height must be positive, got {width}x{height}")

    frames = [load_frame(p, width, height) for p in png_paths]

    boxes = [b for b in (opaque_bbox(f) for f in frames) if b is not None]
    if not boxes:
        fail("every frame is fully transparent")
    box = (min(b[0] for b in boxes), min(b[1] for b in boxes),
           max(b[2] for b in boxes), max(b[3] for b in boxes))
    box_w = box[2] - box[0]
    box_h = box[3] - box[1]

    out_path = f"{name}.h"
    try:
        with open(out_path, "w") as f:
            f.write("#pragma once\n")
            f.write("#include <stdint.h>\n")
            f.write('#include "sprite.h"\n\n')
            f.write(f"#define {name.upper()}_FRAME_COUNT {len(frames)}\n\n")

            for i, frame in enumerate(frames):
                pixels = to_rgb565(frame, box)
                f.write(f"static const uint16_t {name}_frame_{i}[{box_w} * {box_h}] = {{\n")
                for j in range(0, len(pixels), 12):
                    row = pixels[j:j + 12]
                    f.write("    " + ", ".join(f"0x{p:04X}" for p in row) + ",\n")
                f.write("};\n\n")

            f.write(f"static const sprite_t {name}_frames[{name.upper()}_FRAME_COUNT] = {{\n")
            for i in range(len(frames)):
                f.write(f"    {{ {box[0]}, {box[1]}, {box_w}, {box_h}, {name}_frame_{i} }},\n")
            f.write("};\n")
    except OSError as e:
        fail(f"couldn't write '{out_path}': {e}")

    size_bytes = box_w * box_h * 2 * len(frames)
    print(f"Wrote {out_path}  ({len(frames)} frames, {box_w}x{box_h} at ({box[0]}, {box[1]}), "
          f"{size_bytes:,} bytes / {size_bytes/1024:.1f} KB)")


def parse_int(value, label):
    try:
        return int(value)
    except ValueError:
        fail(f"{label} must be an integer, got '{value}'")


if __name__ == "__main__":
    if len(sys.argv) < 5:
        fail(f"expected at least 4 arguments, got {len(sys.argv) - 1}")

    name = sys.argv[1]
    width = parse_int(sys.argv[2], "width")
    height = parse_int(sys.argv[3], "height")

    convert(name, width, height, sys.argv[4:])
