#!/usr/bin/env python
import sys
import argparse
from pycparser import c_parser, c_ast, parse_file


class EllipsisError(Exception):
    pass


def get_nodename(node):
    """ Returns node name
    """

    t = type(node)

    if t == c_ast.TypeDecl:
        return node.declname
    elif t == c_ast.EllipsisParam:
        return None
    else:
        return get_nodename(node.type)


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
    elif nodetype == c_ast.EllipsisParam:
        raise EllipsisError
    else:
        sys.stderr.write("Error, wrong node type %s\n" % nodetype)
        sys.exit(1)


class FuncDeclVisitor(c_ast.NodeVisitor):
    """ Vistor to process functions declarations
    """
    def visit_Decl(self, node):
        """ Process functions declarations
        """
        try:
            # Since pycparse doesn't support parent links, we will find Decl->FuncDecl
            if type(node.type) != c_ast.FuncDecl:
                return
            parent_node = node
            node = node.type

            # Function name
            fn_name = get_nodename(node.type)

            if fn_name.startswith("gl") and not fn_name.startswith("glu"):
                pass
            else:
                return

            # Filter extern
            if self.extern_only and not 'extern' in parent_node.storage:
                return

            # Check for already defined
            if fn_name in self.defined_functions:
                return
            self.defined_functions.add(fn_name)

            fn_ret = get_nodetype(node.type)[0].strip()
            # Second part, used for function returning function pointers
            fn_ret2 = get_nodetype(node.type)[1].strip()

            arg_names, arg_typenames = [], []
            if node.args:
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

        except EllipsisError:
            sys.stderr.write("Warning: skipping ellipsiss\n")
            return
        except Exception as e:
            sys.stderr.write("Error: %s\n" % str(e))
            #raise e
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

    print("#ifdef %s\n#undef %s\n#endif" % (name, name))
    print("DEF_PROXY(%s, %s, (%s), (%s), %s)" % (
        name, es(ret), es(args), es(args2), rword))


def process(args):
    """ Load AST and make defs
    """
    # Remove compiler specific keywords
    undef = ["__attribute__(x)", "__inline", "__restrict", "__extension__"]
    undef = list(map(lambda x: "-D%s=" % x, undef))

    # AST nodes visitor
    v = FuncDeclVisitor()
    v.extern_only = args.extern_only
    v.defined_functions = set()

    for filename in args.files:
        # Make AST
        ast = parse_file(filename, use_cpp=True, cpp_args=undef)

        # Visit AST nodes
        v.visit(ast)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parse C headers.')

    parser.add_argument('files', metavar='FILE', type=str, nargs='+',
            help='C files to process')
    parser.add_argument('-e', metavar='extern_only', dest='extern_only',
            action='store_const', default=False, const=True,
            help='Process only extern declarations')

    args = parser.parse_args()

    process(args)
