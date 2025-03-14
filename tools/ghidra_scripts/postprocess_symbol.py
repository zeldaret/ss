def postprocess_demangled_name(demangled):
    """Turn a demangled name into a name that's usable as a Ghidra symbol name,
    by stripping illegal characters and making sure namespaces are created as expected"""
    if demangled.startswith("vtable for "):
        demangled = demangled[len("vtable for "):] + "::__vtable"

    if demangled.endswith(" const"):
        demangled = demangled[:-len("const ")]

    thunk = False
    guard = False
    if demangled.startswith("non-virtual thunk to "):
        thunk = True
        demangled = demangled[len("non-virtual thunk to "):]

    if demangled.startswith("virtual thunk to "):
        thunk = True
        demangled = demangled[len("virtual thunk to "):]

    if demangled.startswith("guard variable for "):
        guard = True
        demangled = demangled[len("guard variable for "):]

    demangled = demangled.replace("(anonymous namespace)", "anonymous")
    demangled = demangled.replace("operator ", "operator_")

    template_open = demangled.index("<") if "<" in demangled else None
    if template_open is not None:
        first_space = demangled.rindex(" ", 0, template_open) if " " in demangled[:template_open] else None
        open_paren = demangled.index("(") if "(" in demangled else None
        if first_space and open_paren and first_space < template_open and template_open < open_paren:
            # this looks like a templated return type, so drop the return type
            demangled = demangled[(first_space+1):]

    demangled = demangled.replace(" ", "")
    if ")::" in demangled:
        # dFontMng_c::getFontPath(unsigned char)::TEMP_FONT_NAME
        left = demangled.split("(")[0]
        right = demangled.rsplit(")")[-1]
        # dFontMng_c::getFontPath::TEMP_FONT_NAME
        demangled = left + right
    else:
        demangled = demangled.split("(")[0]

    if thunk:
        demangled += "_thunk"
    if guard:
        demangled += "_guard"

    name_list = []
    template_depth = 0
    # do not split namespace within template arguments
    for part in demangled.split("::"):
        if template_depth == 0:
            name_list.append(part)
        else:
            name_list[-1] += "::" + part

        for char in part:
            if char == "<":
                template_depth += 1
            elif char == ">":
                template_depth -= 1

    return name_list


if __name__ == "__main__":
    import demangle
    demangle.mode = 'demangle'

    testcases = [
        [
            "@GUARD@getFontPath__10dFontMng_cFUc@TEMP_FONT_NAME",
            ["dFontMng_c", "getFontPath", "TEMP_FONT_NAME_guard"],
        ],
        [
            "__vt__Q24mDvd42TUncompressInfo_c<Q23EGG15StreamDecompSZS>",
            ["mDvd", "TUncompressInfo_c<EGG::StreamDecompSZS>", "__vtable"],
        ],
        ["chaseT<i>__4sLibFPiii_i", ["sLib", "chaseT<int>"]],
        [
            "CalcAnimationFVS<Q44nw4r3g3d27@unnamed@g3d_resanmchr_cpp@46CAnmFmtTraits<Q34nw4r3g3d18ResAnmChrFVS96Data>>__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPCQ34nw4r3g3d16ResAnmChrFVSDataf_f",
            [
                "nw4r",
                "g3d",
                "anonymous",
                "CalcAnimationFVS<nw4r::g3d::anonymous::CAnmFmtTraits<nw4r::g3d::ResAnmChrFVS96Data>>",
            ],
        ],
        ["baseID_Turn<10sStateID_c>__Fv_RC10sStateID_c", ["baseID_Turn<sStateID_c>"]],
    ]

    for mangled, postprocessed in testcases:
        demangled = demangle.demangle(mangled)
        actual = postprocess_demangled_name(demangled)
        assert actual == postprocessed, str(demangled) + " -> " + str(actual)

    print("OK")
