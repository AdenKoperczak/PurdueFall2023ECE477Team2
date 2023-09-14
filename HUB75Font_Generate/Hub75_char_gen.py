import os
import re
from PIL import Image

def render_txt_file(filename, name, width, height):
    with open(filename) as file:
        items = []
        for y in range(height):
            data = file.readline()
            byte = 0
            for x in range(width):
                if x >= len(data):
                    byte = byte << 1
                else:
                    byte = (byte << 1) | int(data[x] != ' ')
            items.append(f"0x{byte:02x}")

        return f"const Hub75Char{height}x{width} {name} = {{{', '.join(items)}}};"

def render_img_file(filename, name, width, height):
    items = []
    with Image.open(filename) as im:
        px = im.load()
    for y in range(height):
        byte = 0
        for x in range(width):
            byte = (byte << 1) | int(sum(px[x, y]) == 0)
        items.append(f"0x{byte:02x}")

    return f"const Hub75Char{height}x{width} {name} = {{{', '.join(items)}}};"


def run_on_dir(direct, output, width, height, func):
    out = []
    for filename in os.listdir(direct):
        if not re.match("^.*\.swp$", filename):
            print(filename)
            out.append(func(os.path.join(direct, filename), os.path.splitext(filename)[0], width, height))

    with open(output, "w") as file:
        file.write("\n".join(out))

def main():
    run_on_dir("Font7x5_img", "font7x5.c", 5, 7, render_img_file)

if __name__ == "__main__":
    main()
