import re

FILES = ((["Schematic-F_Cu.svg", "Schematic-F_Fab.svg", "Schematic-F_Silkscreen.svg", "Schematic-Edge_Cuts.svg"], "Schematic-Front.svg"),
         (["Schematic-B_Cu.svg", "Schematic-B_Fab.svg", "Schematic-B_Silkscreen.svg", "Schematic-Edge_Cuts.svg"], "Schematic-Back.svg"))

F_CU  = re.compile("C83434"), "A00000"
F_FAB = re.compile("AFAFAF"), "00A0FF"
B_CU  = re.compile("4D7FC4"), "4D7FC4"
B_FAB = re.compile("585D84"), "FFFF00"
BACKGROUND = r"""<rect x="0" y="0" width="100%" height="100%" style="fill:#222222"/>"""
BACKGROUND_LINE = 12 - 1
SKIP_LINES      = 12 - 1

def do_replace(data, reg):
    return reg[0].sub(reg[1], data)

def add_backgroud(data):
    data = data.split("\n")
    data = data[:BACKGROUND_LINE] + [BACKGROUND] + data[BACKGROUND_LINE:]
    return "\n".join(data)

def combine_files(main, new):
    with open(new) as file:
        data = file.readlines()
    
    main = main.splitlines()
    main = main[:-2] + data[SKIP_LINES:-1] + main[-2:]
    return "\n".join(main)

def run_on_files(filenames, output):
    with open(filenames[0], "r") as file:
        data = file.read()
        
    for file in filenames[1:]:
        data = combine_files(data, file)

    data = do_replace(data, F_CU)
    data = do_replace(data, F_FAB)
    data = do_replace(data, B_CU)
    data = do_replace(data, B_FAB)
    data = add_backgroud(data)

    with open(output, "w") as file:
        file.write(data)
        
if __name__ == "__main__":
    for filenames, output in FILES:
        run_on_files(filenames, output)
