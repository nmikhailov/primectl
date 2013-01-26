#!/usr/bin/env python2

""" Usage: call with <filename>
"""

import sys
import clang.cindex

internal2cstr = {
    "CHAR_S": "char",
    "DOUBLE": "double",
    "FLOAT": "float",
    "FUNCTIONPROTO": "void*",  # Temp hack
    "INT": "int",
    "LONG": "long",
    "LONGDOUBLE": "long double",
    "LONGLONG": "long long",
    "RECORD": "void*",  # Temp hack
    "SCHAR": "signed char",
    "SHORT": "short",
    "UCHAR": "unsigned char",
    "UINT": "unsigned int",
    "ULONG": "unsigned long",
    "ULONGLONG": "unsigned long long",
    "USHORT": "unsigned short",
    "VOID": "void",
}


def type2string(type):
    """ Expands pointers and converts types via internal2cstr table
    """
    suffix = ""
    while type.kind == clang.cindex.TypeKind.POINTER:
        if type.is_const_qualified():
            suffix = " const " + suffix
        suffix = "*" + suffix

        type = type.get_pointee()

    if not type.kind.name in internal2cstr.keys():
        sys.stderr.write("Can't convert type %s\n" % type.kind.name)
        sys.exit(1)

    if type.is_const_qualified():
        suffix = "const " + suffix

    return internal2cstr[type.kind.name] + " " + suffix


def get_type(type):
    """ Converts internal clang.cindex.Type to C string representation
    """
    type = type.get_canonical()
    if type == clang.cindex.TypeKind.INVALID:
        sys.stderr.write("Unresolved type, aborting\n")
        sys.exit(1)

    return type2string(type)


def print_declaration(node):
    """ Print formated function declarations
    """
    name = node.spelling

    if not name.startswith("gl"):
        return

    ret = get_type(node.result_type)

    args = [get_type(c) for c in node.type.argument_types()]
    args2 = ["arg%s" % i for i, c in enumerate(args)]
    args1 = [t + " " + n for t, n in zip(args, args2)]

    args1 = ", ".join(args1)
    args2 = ", ".join(args2)

    if node.result_type.kind == clang.cindex.TypeKind.VOID:
        print "DEF_PROXY_VOID(%s, (%s), (%s))" % (name, args1, args2)
    else:
        print "DEF_PROXY(%s, %s, (%s), (%s))" % (name, ret, args1, args2)


def find_declarations(node):
    """ Find all function declarations
    """
    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        print_declaration(node)

    # Recurse for children of this node
    for c in node.get_children():
        find_declarations(c)

if __name__ == '__main__':
    index = clang.cindex.Index.create()
    tu = index.parse(sys.argv[1], args=["-D__attribute__(x)="])
    find_declarations(tu.cursor)
