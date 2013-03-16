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

    if type.is_const_qualified():
        suffix = "const " + suffix

    if not type.kind.name in internal2cstr.keys():
        #sys.stderr.write("Can't convert type %s\n" % type.kind.name)
        #sys.stderr.write("->> %s\n" % type.get_declaration().extent)

        #print type.get_declaration().get_usr()
        #p = type.get_declaration().get_usr()
        #p = p[p.rfind('@') + 1:]
        #return "struct " + p + " " + suffix

        #for x in type.get_declaration().semantic_parent.get_tokens():
            #print "->", x.spelling
            #if x.kind == clang.cindex.TokenKind.IDENTIFIER:
                #sys.stderr.write("=> %s\n" % x.spelling);
                #return x.spelling + " " + suffix

        sys.exit(1)

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

    args = [get_type(c) for c in node.type.argument_types()]  # Arguments type names
    args2 = ["arg%s" % i for i, c in enumerate(args)]  # Arguments names (arg1, arg2,..)
    args1 = [t + " " + n for t, n in zip(args, args2)]  # Argument declarations

    args1 = ", ".join(args1)
    args2 = ", ".join(args2)

    if node.result_type.kind == clang.cindex.TypeKind.VOID:
        print "DEF_PROXY_VOID(%s, (%s), (%s))" % (name, args1, args2)
    else:
        print "DEF_PROXY(%s, %s, (%s), (%s))" % (name, ret, args1, args2)


def print_declaration2(node):
    """ Print formated function declarations
    """
    from clang.cindex import TokenKind as TK

    # Function name
    name = node.spelling
    if not name.startswith("gl"):
        return

    good_tk = (TK.IDENTIFIER, TK.KEYWORD, TK.LITERAL, TK.PUNCTUATION)
    function = filter(lambda x: x.kind in good_tk, list(node.get_tokens()))
    function = map(lambda x: (x.spelling, x.kind), function)

    # function name index
    mid = function.index((name, TK.IDENTIFIER))

    # function return value
    fc = lambda x: not x in ("extern", "static")
    ret_val = ' '.join(filter(fc, map(lambda x: x[0], function)[:mid])).strip()

    # Function args
    args = []
    cur = []
    for n, t in function[mid + 1:] + [(",", TK.PUNCTUATION)]:
        if t != TK.PUNCTUATION:
            cur.append(n)
        elif n == ",":
            args.append((' '.join(cur[:-1]), cur[-1]))
            cur = []

    # Print
    args1 = ', '.join(map(lambda x: x[0] + " " + x[1], args))
    args2 = ', '.join(map(lambda x: x[1], args))

    if node.result_type.kind == clang.cindex.TypeKind.VOID:
        print "DEF_PROXY_VOID(%s, (%s), (%s))" % (name, args1, args2)
    else:
        print "DEF_PROXY(%s, %s, (%s), (%s))" % (name, ret_val, args1, args2)


def find_declarations(node):
    """ Find all function declarations
    """
    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        print_declaration2(node)

    # Recurse for children of this node
    for c in node.get_children():
        find_declarations(c)

if __name__ == '__main__':
    index = clang.cindex.Index.create()
    tu = index.parse(sys.argv[1], args=["-D__attribute__(x)="])
    find_declarations(tu.cursor)
