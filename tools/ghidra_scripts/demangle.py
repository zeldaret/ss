# https://gist.github.com/RootCubed/8f8102fe6cf4ed79a45f1dfe23020a06

# Demangler / Itanium remangler for the CodeWarrior ABI

# Adapted from the NVIDIA demangler script by Ninji
# See https://gist.github.com/RootCubed/9ebecf21eec344f10164cdfabbf0bb41 (Python)
# and https://gist.github.com/RootCubed/d7e2629f4576059853505b7931ffd105 (C++)
# for those scripts

# Ported to Ghidra's Jython / Python 2.7 by robojumper

import argparse
import re
import sys

mode = None
verbose = False

def is_demangle():
    return mode == 'demangle'

"""
The names of the types in the output
First demangled, then remangled for Itanium
"""
names_mapping = {
    'v': ('void', 'v'),
    'b': ('bool', 'b'),
    'c': ('char', 'c'),
    's': ('short', 's'),
    'i': ('int', 'i'),
    'l': ('long', 'l'),
    'x': ('long long', 'x'),
    'Sc': ('signed char', 'a'),
    'Uc': ('unsigned char', 'h'),
    'Us': ('unsigned short', 't'),
    'Ui': ('unsigned int', 'j'),
    'Ul': ('unsigned long', 'm'),
    'Ux': ('unsigned long long', 'y'),
    'f': ('float', 'f'),
    'd': ('double', 'd'),
    'r': ('long double', 'e'),
    'w': ('wchar_t', 'w'),
    'e': ('...', 'z')
}

"""
The names of the methods in the output
First demangled, then remangled for Itanium
"""
method_mapping = {
    '__dt':  ('~$CLS$', 'D0'),
    '__ct':  ('$CLS$', 'C1'),
    '__nw':  ('operator new', 'nw'),
    '__nwa': ('operator new[]', 'na'),
    '__dl':  ('operator delete', 'dl'),
    '__dla': ('operator delete[]', 'da'),
    '__pl':  ('operator+', 'pl'),
    '__mi':  ('operator-', 'mi'),
    '__ml':  ('operator*', 'ml'),
    '__dv':  ('operator/', 'dv'),
    '__md':  ('operator%', 'rm'),
    '__er':  ('operator^', 'eo'),
    '__ad':  ('operator&', 'an'),
    '__or':  ('operator|', 'or'),
    '__co':  ('operator~', 'co'),
    '__nt':  ('operator!', 'nt'),
    '__as':  ('operator=', 'aS'),
    '__lt':  ('operator<', 'lt'),
    '__gt':  ('operator>', 'gt'),
    '__apl': ('operator+=', 'pL'),
    '__ami': ('operator-=', 'mI'),
    '__amu': ('operator*=', 'mL'),
    '__adv': ('operator/=', 'dV'),
    '__amd': ('operator%=', 'rM'),
    '__aer': ('operator^=', 'eO'),
    '__aad': ('operator&=', 'aN'),
    '__aor': ('operator|=', 'oR'),
    '__ls':  ('operator<<', 'ls'),
    '__rs':  ('operator>>', 'rs'),
    '__ars': ('operator>>=', 'rS'),
    '__als': ('operator<<=', 'lS'),
    '__eq':  ('operator==', 'eq'),
    '__ne':  ('operator!=', 'ne'),
    '__le':  ('operator<=', 'le'),
    '__ge':  ('operator>=', 'ge'),
    '__aa':  ('operator&&', 'aa'),
    '__oo':  ('operator||', 'oo'),
    '__pp':  ('operator++', 'pp'),
    '__mm':  ('operator--', 'mm'),
    '__cm':  ('operator,', 'cm'),
    '__rm':  ('operator->*', 'pm'),
    '__rf':  ('operator->', 'pt'),
    '__cl':  ('operator()', 'cl'),
    '__vc':  ('operator[]', 'ix'),
}

def parse_number(s, i):
    """
    Parses a number starting at position i.
    Examples:
        parse_number('123ABC', 0) -> (123, 3)

    Args:
        s (str): The input string to parse.
        i (int): The starting position in the input string.

    Returns:
        Tuple[int, int]: The parsed number and the new position in the string.
    """
    num = 0
    while s[i].isdigit():
        num = num * 10 + int(s[i])
        i += 1
    return num, i

def parse_typename(s, i):
    """
    Fully processes a mangled typename starting at index i.
    Examples:
        (demangle) parse_typename('Q23ABC3DEF', 0) -> ('ABC::DEF', 10)
        (demangle) parse_typename('Q23ABC6DEF<c>', 0) -> ('ABC::DEF<char>', 13)
        (remangle) parse_typename('Q23ABC3DEF', 0) -> ('3ABC3DEF', 10)

    Args:
        s (str): The input string.
        i (int): The starting index.
        is_toplevel (bool): Whether the type is a global-level type. Used for remangling.

    Returns:
        Tuple[str, int]:The parsed typename and the new position in the string.
    """
    if s[i] == 'Q':
        count = int(s[i + 1])
        i += 2
        bits = []
        for _ in range(count):
            size, i = parse_number(s, i)
            bits.append(resolve_templates(s[i:(i + size)], True))
            i += size
        if is_demangle():
            return '::'.join(bits), i
        else:
            return ''.join(bits), i
    else:
        size, i = parse_number(s, i)
        return resolve_templates(s[i:(i + size)], True), i + size
    
def join_modifiers(modifiers):
    """
    Joins the list of modifiers into a single string.
    Modifiers are e.g. const, pointer, reference, etc.
    In a demangled string these are right-to-left (e.g. int const * - pointer to const int)
    whereas in a mangled string they are left-to-right (e.g. PKi - pointer to const int)

    Args:
        modifiers (list[str]): The list of modifiers to join.

    Returns:
        str: The joined string of modifiers.
    """
    if is_demangle():
        return ''.join(modifiers[::-1])
    else:
        return ''.join(modifiers)

def parse_function(s, i, modifiers, name='', rettype_mode = 'show'):
    """
    Parses a function from a demangled string.
    Examples:
        (demangle) parse_function('v_v', 0, ['*']) -> ('void (*) ()', 4)
        (demangle) parse_function('s_b', 0, ['&']) -> ('bool (&) (short)', 4)
        (remangle) parse_function('i_v', 0, ['*']) -> ('FviE', 4)

    Args:
        s (str): The demangled string.
        i (int): The current index in the string.
        modifiers (list[str]): The list of modifiers.
        name (str): An identifier, if available. This is the "main" symbol name.
        rettype_mode (Literal['show', 'hide_in_demangle', 'remove']): How to handle the return type.

    Returns:
        Tuple[str, int]: The transformed function signature and the new position in the string.
    """
    # Parse the function args, return type handled later
    args = []
    while i < len(s) and s[i] != '_' and s[i] != '@':
        argtype, i = parse_type(s, i)
        args.append(argtype)

    # Special case: const
    # Note that if the function is const, it will be the last modifier
    # because e.g. CPFv is a (const pointer) to a function
    const_str = ''
    if len(modifiers) > 0 and (modifiers[-1] == ' const' or modifiers[-1] == 'K'):
        const_str = ' const' if is_demangle() else 'K'
        modifiers.pop()

    mod_str = join_modifiers(modifiers)

    if is_demangle():
        if mod_str != '':
            mod_str = '(%s)' % mod_str.strip()
        arg_str = ', '.join(args) if args[0] != 'void' else ''
        func_str = '%s%s(%s)%s' % (name, mod_str, arg_str, const_str)
        if i >= len(s) or s[i] == '@':
            return func_str, i
        if rettype_mode == 'hide_in_demangle' or rettype_mode == 'remove':
            _, i = parse_type(s, i + 1)
            return func_str, i
        else:
            return parse_type(s, i + 1, [' ' + func_str])
    else:
        if i < len(s) and s[i] != '@':
            rettype, i = parse_type(s, i + 1)
        else:
            rettype, i = ('', i)
        if rettype_mode == 'remove':
            rettype = ''
        func_encoding = '%s%s' % (rettype, ''.join(args)) if name != '' else 'F%s%sE' % (rettype, ''.join(args))
        if name != '':
            func_encoding = 'N%s%sE%s' % (const_str, name, func_encoding)
        else:
            func_encoding = '%s%s' % (const_str, func_encoding)
        return mod_str + func_encoding, i

def parse_type(s, i, modifiers = None, name='', rettype_mode = 'show'):
    """
    Parses a type from a string - main transformation function.

    Args:
        s (str): The string to parse.
        i (int): The starting index.

    Returns:
        Tuple[str, int]: The transformed type name and the new position in the string.
    """

    if modifiers == None:
        modifiers = []

    # Type modifier is for unsigned/signed
    type_modifier = ''

    while i < len(s) and s[i].isupper() and s[i] != 'Q':
        c = s[i]
        if c == 'C': # Const
            modifiers.append(' const' if is_demangle() else 'K')
        elif c == 'P': # Pointer
            modifiers.append('*' if is_demangle() else 'P')
        elif c == 'R': # Reference
            modifiers.append('&' if is_demangle() else 'R')
        elif c == 'V': # Volatile
            modifiers.append(' volatile' if is_demangle() else 'V')
        elif c == 'U' or c == 'S': # Unsigned/Signed
            type_modifier = c
        elif c == 'F': # Function, will return early
            return parse_function(s, i + 1, modifiers, name, rettype_mode)
        elif c == 'M': # Pointer-to-member
            class_name, i = parse_type(s, i + 1)

            modifiers.append(' %s::*' % class_name if is_demangle() else 'M%s' % class_name)
            if s[i] == 'F':
                # CW includes the hidden pointer arguments in the PTMF signature
                # and also uses this to communicate constness of the PTMF
                if s[i:].startswith('FPCvPCv'):
                    modifiers.append(' const' if is_demangle() else 'K')
                    i += 7
                elif s[i:].startswith('FPCvPv'):
                    i += 6
                if s[i] == '_':
                    # small hack: simulate Fv_... by reusing the v from FPCvPCv/FPCvPv
                    i -= 1
                return parse_function(s, i, modifiers)
            else:
                # pointer-to-member-nonfunction, continue parsing as normal
                continue
        elif c == 'A': # Array
            count, i = parse_number(s, i + 1)
            # Automatically skips past the '_' after the number before the next iteration
            if is_demangle():
                modstr = join_modifiers(modifiers)
                if re.search(r'\[.*\]$', modstr) != None:
                    modifiers.insert(0, '[%d]' % count)
                elif modstr == '':
                    modifiers.insert(0, ' [%d]' % count)
                else:
                    # modifiers.insert(0, f' ({modstr}) [{count}]')
                    modifiers = [' (%s) [%d]' % (modstr, count)]
            else:
                modifiers.append('A%d_' % count)
        else:
            raise Exception('Invalid type modifier "' + c + '"')
        i += 1

    assert i < len(s)
    assert s[i].isalpha() or s[i].isdigit()

    # Now we have either an identifier or a basic type

    if s[i] == 'Q' or s[i].isdigit():
        type_name, i = parse_typename(s, i)
        if not is_demangle():
            type_name = 'N%sE' % type_name
    else:
        # Basic type - combine with type modifier and look up in mapping
        actual_type = type_modifier + s[i]
        if actual_type not in names_mapping:
            raise Exception('Invalid type "' + actual_type + '"')
        type_name = names_mapping[actual_type][0 if is_demangle() else 1]
        i += 1

    mod_str = join_modifiers(modifiers)

    if is_demangle():
        return '%s%s' % (type_name, mod_str), i
    else:
        return '%s%s' % (mod_str, type_name), i

def resolve_templates(s, remangle_add_length):
    """
    Resolves template types in a type string.
    Examples:
        (demangle) resolve_templates('std<c>', false) -> 'std<char>'
        (remangle) resolve_templates('std<c>', false) -> 'stdIcE'
        (remangle) resolve_templates('std<c>', true) -> '3stdIcE'

    Args:
        s (str): The string to resolve.
        remangle_add_length (bool): Whether to add the length prefix in remangling.

    Returns:
        str: The resolved string.
    """
    begin_pos = s.find('<')
    if begin_pos == -1:
        if re.match(r'^@unnamed@.+@$', s) != None:
            if is_demangle():
                # name.split('@')[2] contains the name of the file the anonymous namespace is in,
                # but we lose that information here since we follow c++filt's behavior.
                return '(anonymous namespace)'
            else:
                return '12_GLOBAL__N_1'
        unnamed_type_m = re.match(r'^@class\$(\d*).+$', s)
        if unnamed_type_m != None:
            typenum = int(unnamed_type_m.group(1)) if unnamed_type_m.group(1) != '' else -1
            if is_demangle():
                return '{unnamed type#%d}' % (typenum + 2)
            else:
                return 'Ut%s_' % (str(typenum) if typenum > -1 else '')
        if not is_demangle() and remangle_add_length:
            return '%d%s' % (len(s), s)
        return s
    template_str = ''
    i = begin_pos + 1
    while i < len(s):
        if s[i] == ',':
            if is_demangle():
                template_str += ', '
            i += 1
            continue
        if s[i] == '>':
            break
        elif re.match(r'[-\d]+[>,]', s[i:]) != None:
            # Integer literal
            # ss/robojumper: fix [0] -> .group(0)
            literal = re.match(r'[-\d]+', s[i:]).group(0)
            template_str += literal if is_demangle() else 'XLi%sEE' % literal.replace('-', 'n')
            i += len(literal)
        else:
            type, i = parse_type(s, i)
            template_str += type
    if is_demangle():
        template_str = '<%s>' % template_str
        # replicate c++filt behavior
        if template_str[-2:] == '>>':
            template_str = template_str[:-1] + ' >'
        return s[0:begin_pos] + template_str
    else:
        if remangle_add_length:
            return str(begin_pos) + s[0:begin_pos] + ('I%sE' % template_str)
        return s[0:begin_pos] + ('I%sE' % template_str)

def demangle(s):
    """
    Demangles a mangled symbol name.
    """

    at_sym = ''
    thunk_offsets = []
    m = re.match(r'^@([^@]+)@(.+)$', s)
    if m != None:
        m_thunk = re.match(r'^@(\d+)(?:@(\d+))?@(.+)$', s)
        if m_thunk != None:
            thunk_offsets = [int(m_thunk.group(1))]
            if m_thunk.group(2) != None:
                thunk_offsets.append(int(m_thunk.group(2)))
            s = m_thunk.group(3)
        else:
            at_sym = m.group(1)
            if at_sym not in ['LOCAL', 'GUARD', 'STRING']:
                raise Exception('Invalid symbol name "' + s + '"')
            s = m.group(2)

    template_depth = 0
    last_possible_end = -1
    for i in range(1, len(s)):
        if s[i] == '<':
            template_depth += 1
        elif s[i] == '>':
            template_depth -= 1
        if template_depth == 0 and i + 2 < len(s) and s[i:i + 2] == '__' and s[i + 2] in 'CFQ0123456789':
            last_possible_end = i
            break
    if last_possible_end == -1:
        return s

    i = last_possible_end

    method, remainder = s[:i], s[i + 2:]
    if remainder[0] == 'F':
        # Global function without class
        class_name = ''
        i = 0
    else:
        class_name, i = parse_typename(remainder, 0)

    if '<' in method:
        template_start = method.find('<')
        pre_template, template = method[:template_start], method[template_start:]
        resolved_templates = resolve_templates(template, False)
    else:
        pre_template, resolved_templates = method, ''

    if pre_template in ['__ct', '__dt']:
        rettype_mode = 'remove'
    elif at_sym != '':
        rettype_mode = 'hide_in_demangle'
    else:
        rettype_mode = 'show'

    if method == '__vt':
        return 'vtable for %s' % class_name if is_demangle() else '_ZTVN%sE' % class_name
    elif method.startswith('__op'):
        # Use method because the type might contain templates
        cv_type_name, _ = parse_type(method[4:], 0)
        pre_template = 'operator %s' % cv_type_name if is_demangle() else 'cv%s' % cv_type_name
        # __op cannot be templated
        resolved_templates = ''
    elif pre_template in method_mapping:
        pre_template = method_mapping[pre_template][0 if is_demangle() else 1]
        if is_demangle():
            # __ct should use the template of the function, not of the parent class
            last_class_name = re.sub(r'<.+>', '', class_name).split('::')[-1]
            pre_template = pre_template.replace('$CLS$', last_class_name)
    else:
        if not is_demangle():
            pre_template = '%d%s' % (len(pre_template), pre_template)
    
    method = '%s%s' % (pre_template, resolved_templates)

    if is_demangle():
        demangled = '::'.join(filter(None, [class_name, method]))
    else:
        demangled = class_name + method

    if i < len(remainder):
        demangled, i = parse_type(remainder, i, name=demangled, rettype_mode=rettype_mode)
    elif not is_demangle():
        demangled = 'N%sE' % demangled

    if i < len(remainder) and remainder[i] == '@' and at_sym in ['LOCAL', 'GUARD']:
        subs = remainder[i + 1:].split('@')
        local_sym_name = subs[0]
        local_sym_extra = ('_' + subs[1]) if len(subs) > 1 else ''
        if not is_demangle():
            local_sym_name = str(len(local_sym_name)) + local_sym_name + local_sym_extra
    elif at_sym == 'GUARD' and i >= len(remainder):
        local_sym_name = method
    elif at_sym == 'STRING' and i < len(remainder) and remainder[i] == '@' and not is_demangle():
        local_sym_name = '_' + remainder[i + 1]
    else:
        local_sym_name = ''

    if is_demangle():
        if local_sym_name != '':
            demangled += '::%s' % local_sym_name

        # c++filt removes spaces in (* <symbol name>) -> (*<symbol name>), try to replicate this
        while True:
            m = re.search(r'\((?:[*&]|const| )+ (\w+.+)$', demangled)
            if m == None or m.group(1).startswith('const'):
                break
            demangled = demangled[:m.start(1) - 1] + m.group(1)

        if at_sym == 'GUARD':
            return 'guard variable for %s' % demangled
        elif at_sym == 'STRING':
            return '%s::string literal' % demangled
        elif len(thunk_offsets) > 0:
            thunk_type = 'virtual' if len(thunk_offsets) == 2 else 'non-virtual'
            return '%s thunk to %s' % (thunk_type, demangled)
        else:
            return demangled
    else:
        if len(thunk_offsets) == 1:
            demangled = 'Th%d_%s' % (thunk_offsets[0], demangled)
        elif len(thunk_offsets) == 2:
            demangled = 'Tv%d_n%d_%s' % (thunk_offsets[0], thunk_offsets[1], demangled)
        if at_sym == 'LOCAL':
            demangled = 'Z%sE%s' % (demangled, local_sym_name)
        if at_sym == 'GUARD':
            demangled = 'GVZ%sE%s' % (demangled, local_sym_name)
        if at_sym == 'STRING':
            demangled = 'Z%sEs%s' % (demangled, local_sym_name)
        return '_Z%s' % demangled

def demangle_try(s):
    try:
        return demangle(s)
    except Exception as e:
        # ss/robojumper: more context
        sys.stderr.write('Demangler error: ' + str(e) + ' trying to demangle ' + s + '\n')
        raise e

def main():
    global mode
    global verbose
    parser = argparse.ArgumentParser()
    parser.add_argument('symbol', type=str, nargs='?')
    parser.add_argument('-m', '--mode', choices=['demangle', 'remangle_itanium'], required=True)
    parser.add_argument('-v', '--verbose', action='store_true', default=False)
    args = parser.parse_args()
    mode = args.mode
    verbose = args.verbose
    if args.symbol is None:
        while True:
            sym = input()
            print(demangle_try(sym))
    else:
        print(demangle_try(args.symbol))
        return

if __name__ == '__main__':
    main()
