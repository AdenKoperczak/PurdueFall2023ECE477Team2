import re

FILES = ("Schematic-F_Cu.svg", "Schematic-B_Cu.svg")

F_CU  = re.compile("C83434"), "A00000"
F_FAB = re.compile("AFAFAF"), "00A0FF"
B_CU  = re.compile("4D7FC4"), "4D7FC4"
B_FAB = re.compile("585D84"), "FFFF00"
BACKGROUND = r"""<rect x="0" y="0" width="100%" height="100%" style="fill:#222222"/>"""
BACKGROUND_LINE = 12 - 1

def do_replace(data, reg):
    return reg[0].sub(reg[1], data)

def add_backgroud(data):
    data = data.split("\n")
    data = data[:BACKGROUND_LINE] + [BACKGROUND] + data[BACKGROUND_LINE:]
    return "\n".join(data)

def run_on_file(filename):
    with open(filename, "r") as file:
        data = file.read()

    data = do_replace(data, F_CU)
    data = do_replace(data, F_FAB)
    data = do_replace(data, B_CU)
    data = do_replace(data, B_FAB)
    data = add_backgroud(data)

    with open(filename, "w") as file:
        file.write(data)
        
if __name__ == "__main__":
    for filename in FILES:
        run_on_file(filename)
