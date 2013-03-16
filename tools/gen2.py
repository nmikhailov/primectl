#!/usr/bin/env python
import sys
from pycparser import c_parser, c_ast, parse_file


def get_nodename(decl):
    """ Returns node name
    """

    if type(decl) == c_ast.TypeDecl:
        return decl.declname
    else:
        return get_nodename(decl.type)


def get_nodetype(node):
    """ Recursively get node type string
    """
    nodetype = type(node)

    if nodetype == c_ast.TypeDecl:
        quals = ' '.join(node.quals) + ' ' if node.quals else ''
        sub = get_nodetype(node.type)

        return (quals + sub[0], sub[1])
    elif nodetype == c_ast.Typename or nodetype == c_ast.Decl:
        return get_nodetype(node.type)
    elif nodetype == c_ast.IdentifierType:
        return (' '.join(node.names), '')
    elif nodetype == c_ast.PtrDecl:
        quals = ' '.join(node.quals) + ' ' if node.quals else ''
        sub = get_nodetype(node.type)

        return (quals + sub[0] + "*", sub[1])
    elif nodetype == c_ast.ArrayDecl:
        arr = '[]'
        if node.dim:
            arr = '[%s]' % node.dim.value
        sub = get_nodetype(node.type)

        return (sub[0], sub[1] + arr)
    elif nodetype == c_ast.Struct:
        return ("struct " + node.name, '')
    elif nodetype == c_ast.FuncDecl:
        args = ''
        if node.args:
            params = [' '.join(get_nodetype(p)) for p in node.args.params]
            args = ', '.join(params)

        sub = get_nodetype(node.type)

        # sub[1] ??
        return ('%s(' % sub[0], '))(%s' % args)
    else:
        sys.stderr.write("Error, wrong node type %s\n" % nodetype)
        sys.exit(1)


class FuncDeclVisitor(c_ast.NodeVisitor):
    """ Vistor to process functions declarations
    """
    def visit_FuncDecl(self, node):
        """ Process functions declarations
        """
        try:
            fn_name = get_nodename(node.type)

            # Filter functions
            if not fn_name.startswith("gl"):
                return

            fn_ret = get_nodetype(node.type)[0].strip()
            # Second part, used for function returning function pointers
            fn_ret2 = get_nodetype(node.type)[1].strip()

            arg_names, arg_typenames = [], []
            for i, arg in enumerate(node.args.params):
                cname = get_nodename(arg)
                ctype = get_nodetype(arg)

                # Skip cases with one void argument
                if ctype == ('void', '') and len(node.args.params) == 1:
                    break

                # Argument doesn't have a name
                if not cname:
                    cname = "arg%d" % i

                arg_names += [cname]
                arg_typenames += ["%s %s%s" % (ctype[0], cname, ctype[1])]

            arg_names = ', '.join(arg_names)
            arg_typenames = ', '.join(arg_typenames) + " " + fn_ret2

            def_proxy(fn_name, fn_ret, arg_typenames, arg_names)

        except Exception as e:
            print(e)
            # node.show()
            sys.exit(1)


def escape_define(string):
    """ Escape braces for definitions
    """

    # BRACE_START = ( and BRACE_END = ) are defined in defs.c
    string = string.replace('(', ' BRACE_START ')
    string = string.replace(')', ' BRACE_END ')

    return string


def def_proxy(name, ret, args, args2):
    """ Print final formatted DEF_PROXY definitions
    """

    es = escape_define
    rword = ''
    if ret != 'void':
        rword = 'return'

    print("DEF_PROXY(%s, %s, (%s), (%s), %s)" % (
        name, es(ret), es(args), es(args2), rword))


def process(filename):
    """ Load AST and make defs
    """
    # Remove compiler specific keywords
    undef = ["__attribute__(x)", "__inline", "__restrict", "__extension__"]
    undef = list(map(lambda x: "-D%s=" % x, undef))

    # Make AST
    ast = parse_file(filename, use_cpp=True, cpp_args=undef)

    # Visit AST nodes
    v = FuncDeclVisitor()
    v.visit(ast)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        filename = sys.argv[1]
    else:
        print('Usage: %s file' % sys.argv[0])
        sys.exit(1)

    process(filename)
