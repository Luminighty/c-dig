header_guard = "LINALG_H"
filename = "linalg"
include_folder = "../include"
src_folder = "../src"

vectors = [
    {"name": "Vec2i", "prefix": "vec2i", "component": "int"  , "dimension": 2},
    {"name": "Vec2" , "prefix": "vec2" , "component": "float", "dimension": 2},
]

component_names = ["x", "y", "z", "w"]

src_includes = ["<stdio.h>"]
is_header = False
file = None

def vector_create(file, vector):
    if is_header:
        vector_typedef(file, vector)
    vector_op(file, vector, "add", "+")
    vector_op(file, vector, "sub", "-")
    vector_scale(file, vector)
    vector_dot(file, vector)
    vector_lengthsqr(file, vector)
    vector_print(file, vector)

    functions = {
        "int": {"min": "min", "max": "max"},
        "float": {"min": "fmin", "max": "fmax"},
    }
    comp = vector["component"]
    vector_fn(file, vector, "min", "min")
    vector_fn(file, vector, "max", "max")
    file.write("\n")


def vector_fn(file, vector, fn_name, fn_to_call):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]
    function_begin(name, f"{prefix}_{fn_name}", f"{name} a", f"{name} b")
    if is_header:
        return

    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f".{field} = {fn_to_call}(a.{field}, b.{field})")
    fields = "".join([f"\t\t{field},\n" for field in fields])

    file.write(f"\treturn (({name}){{\n{fields}\t}});\n")
    file.write("}\n")


def vector_print(file, vector):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]
    function_begin("void", f"{prefix}_print", f"{name} vector")
    if is_header:
        return

    format = {"int": "%d", "float": "%f"}[comp]
    formats = ", ".join([format for _ in range(0, dim)])
    values = ", ".join(["vector.{}".format(component_names[i]) for i in range(0, dim)])

    file.write(f"\tprintf(\"{name}({formats})\", {values});\n")
    file.write("}\n")


def vector_lengthsqr(file, vector):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]
    function_begin(comp, f"{prefix}_lengthsqr", f"{name} vector")
    if is_header:
        return

    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f"(vector.{field} * vector.{field})")
    fields = " + ".join(fields)

    file.write(f"\treturn {fields};\n")
    file.write("}\n")


def vector_dot(file, vector):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]
    function_begin(comp, f"{prefix}_dot", f"{name} a", f"{name} b")
    if is_header:
        return

    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f"(a.{field} * b.{field})")
    fields = " + ".join(fields)

    file.write(f"\treturn {fields};\n")
    file.write("}\n")


def vector_scale(file, vector):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]

    function_begin(name, f"{prefix}_scale", f"{name} a", f"{comp} scale")
    if is_header:
        return

    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f".{field} = a.{field} * scale")
    fields = "".join([f"\t\t{field},\n" for field in fields])

    file.write(f"\treturn (({name}){{\n{fields}\t}});\n")
    file.write("}\n")


def vector_op(file, vector, method_name, op):
    name = vector["name"]
    comp = vector["component"]
    dim = vector["dimension"]
    prefix = vector["prefix"]
    function_begin(name, f"{prefix}_{method_name}", f"{name} a", f"{name} b")
    if is_header:
        return
    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f".{field} = a.{field} {op} b.{field}")

    fields = "".join([f"\t\t{field},\n" for field in fields])
    file.write(f"\treturn (({name}){{\n{fields}\t}});\n")
    file.write("}\n")


def function_begin(return_type, fn_name, *args):
    if not is_header:
        file.write("inline ")
    arg_str = ", ".join(args)
    file.write(f"{return_type} {fn_name}({arg_str})")
    if is_header:
        file.write(";\n")
    else:
        file.write(" {\n")


def function_end():
    if not is_header:
        file.write("}\n")

def vector_macro():
    file.write("#define min(a, b) (a < b) ? a : b\n")
    file.write("#define max(a, b) (a > b) ? a : b\n")
    file.write("\n")


def vector_typedef(file, vector):
    comp = vector["component"]
    dim = vector["dimension"]
    name = vector["name"]
    file.write("typedef union {\n")

    fields = []
    for i in range(0, dim):
        field = component_names[i]
        fields.append(f"{comp} {field};")
    fields = " ".join(fields)

    file.write(f"\tstruct {{ {fields} }};\n")
    file.write(f"\t{comp} v[{dim}];\n")

    file.write(f"}} {name};\n\n")


def write_file(_file, _is_header):
    global file, is_header
    is_header = _is_header
    file = _file
    file.write("// This file has been autogenerated using math.py\n")
    if is_header:
        file.write(f"#ifndef {header_guard}\n")
        file.write(f"#define {header_guard}\n")
    else:
        file.write(f"#include \"{filename}.h\"\n")
        for include in src_includes:
            file.write(f"#include {include}\n")
        vector_macro()
    file.write("\n")

    for vector in vectors:
        vector_create(file, vector)

    file.write("\n")
    if is_header:
        file.write(f"#endif // {header_guard}\n")


with open(f"{include_folder}/{filename}.h", "w") as f:
    write_file(f, True)

with open(f"{src_folder}/{filename}.c", "w") as f:
    write_file(f, False)

