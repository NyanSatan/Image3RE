import json
import idautils
import idc

def hexify(x):
    if x is not None:
        return "0x%x" % x
    else:
        return None

result = list()

print("loading functions")

for func_start_ea in idautils.Functions():
    func_end_ea = idc.find_func_end(func_start_ea)
    func_name = idc.get_func_name(func_start_ea)
    func_exit_point = None

    # usually exit point is just the last instruction in function's bounds
    # but still we check just to be sure
    for insn_ea in idautils.Heads(func_start_ea, func_end_ea):
        disasm = idc.GetDisasm(insn_ea)

        if ("POP" in disasm and "PC" in disasm) or ("BX" in disasm and "LR" in disasm):
            func_exit_point = insn_ea
            break

    result.append(
        {
            "name" : func_name,
            "start" : hexify(func_start_ea),
            "end" : hexify(func_end_ea),
            "exit" : hexify(func_exit_point)
        }
    )

print("encoding results")

encoded = json.dumps(result, indent=4)

print("printing results")

print(encoded)
