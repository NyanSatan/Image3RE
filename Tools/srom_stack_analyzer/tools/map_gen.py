#!/usr/bin/env python3

from platform import platform
import sys
from collections import namedtuple

Function = namedtuple("Function", ["name", "offset"])

def main():
    try:
        platform_name = sys.argv[2]

        if not platform_name.isalnum():
            raise ValueError()

        ida_map = open(sys.argv[1], "r")
    except IndexError:
        print("usage: %s <ida map> <platform name>" % sys.argv[0])
        exit(-1)
    except IOError:
        print("failed to open input file")
        exit(-1)
    except ValueError:
        print("bad platform name - must only contain digits and alphabets")
        exit(-1)

    lines = ida_map.readlines()

    ida_map.close()

    functions = list()

    for line in lines:
        splitted = line.split("\t")

        if len(splitted) < 3:
            print("bad line:")
            print(f"\"{line}\"")
            exit(-1)

        function = Function(
            name=splitted[0],
            offset=int(splitted[2], 16)
        )

        functions.append(function)

    header_map = open(f"{platform_name}_map.h", "w")

    platform_name_big = f"{platform_name.upper()}_MAP_H"

    result = f"#ifndef {platform_name_big}\n#define {platform_name_big}\n"

    result += "\n"

    result += "#include <types.h>\n"

    result += "\n"

    result += f"static const platform_map_t {platform_name}_map = {{\n"

    for function in functions:
        result += f"    {{\"{function.name}\", 0x{function.offset:x}}},\n"

    result += "};"

    result += "\n"

    result += "\n#endif"

    result += "\n"


    header_map.write(result)
    header_map.close()

if __name__ == "__main__":
    main()
