"""
Broma (.bro) parser for Geode bindings.
Extracts classes, functions, members, and platform addresses.
"""
import re
import hashlib
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class PlatformAddrs:
    win: Optional[int] = None
    win_inline: bool = False
    imac: Optional[int] = None
    m1: Optional[int] = None
    ios: Optional[int] = None

    # The bindings use this as "address not known yet"; it is far outside the image.
    WIN_PLACEHOLDER = 0x9999999

    def has_win(self) -> bool:
        return self.win is not None and not self.win_inline and self.win != self.WIN_PLACEHOLDER


@dataclass
class FuncDecl:
    name: str
    return_type: str
    params: str  # raw param string
    addrs: PlatformAddrs = field(default_factory=PlatformAddrs)
    is_virtual: bool = False
    is_static: bool = False
    is_ctor: bool = False
    is_dtor: bool = False
    is_const: bool = False
    class_name: str = ""

    def signature(self) -> str:
        prefix = ""
        if self.is_static:
            prefix = "static "
        elif self.is_virtual:
            prefix = "virtual "
        ret = self.return_type
        if self.is_ctor or self.is_dtor:
            ret = ""
        qual = f"{self.class_name}::" if self.class_name else ""
        return f"{prefix}{ret}{qual}{self.name}({self.params})".strip()

    def full_key(self) -> str:
        """Key for address map: ClassName::funcName(params)"""
        qual = f"{self.class_name}::" if self.class_name else ""
        return f"{qual}{self.name}({self.params})"


@dataclass
class MemberDecl:
    name: str
    type: str
    is_pad: bool = False
    pad_size: int = 0  # win pad size in bytes


@dataclass
class ClassDecl:
    name: str
    bases: list[str] = field(default_factory=list)
    functions: list[FuncDecl] = field(default_factory=list)
    members: list[MemberDecl] = field(default_factory=list)
    attributes: list[str] = field(default_factory=list)

    def layout_hash(self) -> str:
        """Hash of member layout for detecting class changes across versions."""
        parts = []
        for m in self.members:
            if m.is_pad:
                parts.append(f"PAD:{m.pad_size}")
            else:
                parts.append(f"{m.type}:{m.name}")
        raw = "|".join(parts)
        return hashlib.sha256(raw.encode()).hexdigest()[:16]


class BromaParser:
    def __init__(self):
        self.classes: dict[str, ClassDecl] = {}
        self.free_functions: list[FuncDecl] = []

    def parse_file(self, path: Path):
        text = path.read_text(encoding="utf-8", errors="replace")
        # Strip single-line comments but preserve line structure
        lines = text.split("\n")
        # We need to process the raw text with a state machine due to
        # brace nesting in inline function bodies
        self._parse_text(text)

    def _parse_text(self, text: str):
        # Remove block comments
        text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)

        pos = 0
        length = len(text)
        current_attrs = []

        while pos < length:
            # Skip whitespace
            while pos < length and text[pos] in ' \t\r\n':
                pos += 1
            if pos >= length:
                break

            # Skip line comments
            if text[pos:pos+2] == '//':
                nl = text.find('\n', pos)
                pos = nl + 1 if nl != -1 else length
                continue

            # Skip #include
            if text[pos] == '#':
                nl = text.find('\n', pos)
                pos = nl + 1 if nl != -1 else length
                continue

            # Attributes [[...]]
            if text[pos:pos+2] == '[[':
                end = text.find(']]', pos)
                if end == -1:
                    break
                current_attrs.append(text[pos+2:end])
                pos = end + 2
                continue

            # Class declaration
            m = re.match(r'class\s+([\w:]+)', text[pos:])
            if m:
                cls_name = m.group(1)
                pos += m.end()
                # Parse bases
                pos = self._skip_ws_comments(text, pos)
                bases = []
                if pos < length and text[pos] == ':':
                    pos += 1
                    bases, pos = self._parse_bases(text, pos)

                pos = self._skip_ws_comments(text, pos)
                if pos < length and text[pos] == '{':
                    pos += 1
                    cls = ClassDecl(name=cls_name, bases=bases, attributes=current_attrs[:])
                    pos = self._parse_class_body(text, pos, cls)
                    self.classes[cls_name] = cls
                current_attrs.clear()
                continue

            # Free function or other declaration before any class
            # Try to parse as a function declaration
            func, new_pos = self._try_parse_func(text, pos, "")
            if func:
                self.free_functions.append(func)
                pos = new_pos
                current_attrs.clear()
                continue

            # Skip unknown token
            nl = text.find('\n', pos)
            pos = nl + 1 if nl != -1 else length

    def _skip_ws_comments(self, text: str, pos: int) -> int:
        length = len(text)
        while pos < length:
            if text[pos] in ' \t\r\n':
                pos += 1
            elif text[pos:pos+2] == '//':
                nl = text.find('\n', pos)
                pos = nl + 1 if nl != -1 else length
            elif text[pos:pos+2] == '/*':
                end = text.find('*/', pos)
                pos = end + 2 if end != -1 else length
            else:
                break
        return pos

    def _parse_bases(self, text: str, pos: int) -> tuple[list[str], int]:
        bases = []
        length = len(text)
        current = ""
        angle_depth = 0
        while pos < length:
            ch = text[pos]
            if ch == '<':
                angle_depth += 1
                current += ch
                pos += 1
            elif ch == '>':
                angle_depth -= 1
                current += ch
                pos += 1
            elif ch == ',' and angle_depth == 0:
                b = current.strip()
                if b:
                    bases.append(b)
                current = ""
                pos += 1
            elif ch == '{':
                b = current.strip()
                if b:
                    bases.append(b)
                break
            elif ch == ';':
                b = current.strip()
                if b:
                    bases.append(b)
                pos += 1
                break
            else:
                current += ch
                pos += 1
        return bases, pos

    def _parse_class_body(self, text: str, pos: int, cls: ClassDecl) -> int:
        length = len(text)
        brace_depth = 1

        while pos < length and brace_depth > 0:
            pos = self._skip_ws_comments(text, pos)
            if pos >= length:
                break

            ch = text[pos]
            if ch == '}':
                brace_depth -= 1
                pos += 1
                continue
            if ch == '{':
                brace_depth += 1
                pos += 1
                continue

            # Skip [[attributes]] inside class
            if text[pos:pos+2] == '[[':
                end = text.find(']]', pos)
                pos = end + 2 if end != -1 else length
                continue

            # PAD
            if text[pos:pos+3] == 'PAD':
                pos = self._parse_pad(text, pos, cls)
                continue

            # Try to parse as function or member
            func, new_pos = self._try_parse_func(text, pos, cls.name)
            if func:
                cls.functions.append(func)
                pos = new_pos
                continue

            # Try to parse as member
            mem, new_pos = self._try_parse_member(text, pos)
            if mem:
                cls.members.append(mem)
                pos = new_pos
                continue

            # Skip to next line
            nl = text.find('\n', pos)
            pos = nl + 1 if nl != -1 else length

        return pos

    def _parse_pad(self, text: str, pos: int, cls: ClassDecl) -> int:
        # PAD = win 0xNN, ...;
        semi = text.find(';', pos)
        if semi == -1:
            return pos + 3
        chunk = text[pos:semi]
        win_m = re.search(r'win\s+0x([0-9a-fA-F]+)', chunk)
        pad_size = int(win_m.group(1), 16) if win_m else 0
        if pad_size > 0:
            cls.members.append(MemberDecl(
                name=f"_pad_{len(cls.members)}",
                type=f"uint8_t[{pad_size}]",
                is_pad=True,
                pad_size=pad_size
            ))
        return semi + 1

    def _try_parse_func(self, text: str, pos: int, class_name: str) -> tuple[Optional[FuncDecl], int]:
        """Try parsing a function declaration at pos. Returns (func, new_pos) or (None, pos)."""
        length = len(text)
        start = pos

        # Collect the line(s) up to ';' or '{' (for inline bodies)
        # We need to find the opening '(' to identify this as a function
        # First, scan ahead to see if there's a '(' before ';' or '}'

        # Quick check: scan for '(' within reasonable distance
        scan_end = min(pos + 500, length)
        paren_pos = -1
        for i in range(pos, scan_end):
            if text[i] == '(':
                paren_pos = i
                break
            if text[i] == ';' or text[i] == '}':
                break
            if text[i] == '\n' and i > pos + 1:
                # Check if next non-ws is not a continuation
                pass

        if paren_pos == -1:
            return None, pos

        # Extract the prefix (return type + name) before '('
        prefix = text[pos:paren_pos].strip()
        # Remove leading line comments from prefix lines
        prefix_lines = prefix.split('\n')
        clean_lines = []
        for ln in prefix_lines:
            ln = ln.strip()
            if ln.startswith('//'):
                continue
            # Remove trailing comment
            cc = ln.find('//')
            if cc != -1:
                ln = ln[:cc].strip()
            clean_lines.append(ln)
        prefix = ' '.join(clean_lines).strip()

        if not prefix:
            return None, pos

        # Parse qualifiers
        is_virtual = False
        is_static = False
        is_ctor = False
        is_dtor = False

        if prefix.startswith('virtual '):
            is_virtual = True
            prefix = prefix[8:].strip()
        if prefix.startswith('static '):
            is_static = True
            prefix = prefix[7:].strip()
        if prefix.startswith('callback '):
            prefix = prefix[9:].strip()

        # Split into return type and name
        # Destructor: ~ClassName
        # Constructor: ClassName (same as class_name)
        # Otherwise: ReturnType FuncName
        if prefix.startswith('~'):
            name = prefix
            return_type = ""
            is_dtor = True
        elif class_name and prefix == class_name:
            name = prefix
            return_type = ""
            is_ctor = True
        else:
            # Find the function name (last word)
            # Handle cases like "TodoReturn funcName" or "cocos2d::CCPoint funcName"
            # or "std::vector<int> funcName"
            parts = self._split_type_name(prefix)
            if parts is None:
                return None, pos
            return_type, name = parts

        # Parse params: find matching ')'
        params_start = paren_pos + 1
        params_end, _ = self._find_matching(text, params_start, '(', ')')
        if params_end == -1:
            return None, pos
        params = text[params_start:params_end].strip()

        pos = params_end + 1  # past ')'

        # Parse const qualifier
        pos = self._skip_ws_comments(text, pos)
        is_const = False
        if pos < length and text[pos:pos+5] == 'const':
            is_const = True
            pos += 5

        # Now look for = addrs ; or = addrs { body } or just ;
        pos = self._skip_ws_comments(text, pos)
        addrs = PlatformAddrs()

        if pos < length and text[pos] == '=':
            pos += 1
            pos = self._skip_ws_comments(text, pos)
            # Parse platform addresses until ';' or '{'
            addrs, pos = self._parse_addrs(text, pos)

        pos = self._skip_ws_comments(text, pos)
        # Skip inline body if present
        if pos < length and text[pos] == '{':
            pos = self._skip_braces(text, pos)
        elif pos < length and text[pos] == ';':
            pos += 1

        func = FuncDecl(
            name=name,
            return_type=return_type,
            params=params,
            addrs=addrs,
            is_virtual=is_virtual,
            is_static=is_static,
            is_ctor=is_ctor,
            is_dtor=is_dtor,
            is_const=is_const,
            class_name=class_name,
        )
        return func, pos

    def _split_type_name(self, prefix: str) -> Optional[tuple[str, str]]:
        """Split 'ReturnType FuncName' handling templates and namespaces."""
        if not prefix:
            return None
        # Work backwards from the end to find the function name
        # The name is the last identifier (possibly with ~ for dtor, :: for qualified)
        # But the return type can also have :: and <>
        tokens = prefix.split()
        if len(tokens) == 0:
            return None
        if len(tokens) == 1:
            return ("", tokens[0])

        # The function name is the last token unless there's template stuff
        name = tokens[-1]
        ret = ' '.join(tokens[:-1])

        # Handle case where the name got stuck to a template: "std::vector<Foo>funcName" is unlikely
        # but "operator=" etc could be tricky. For now, simple split works.
        return (ret, name)

    PLATFORMS = {'win', 'imac', 'm1', 'ios', 'android', 'android32', 'android64', 'mac'}

    def _parse_addrs(self, text: str, pos: int) -> tuple[PlatformAddrs, int]:
        """Parse platform address annotations like 'win 0x123, imac 0x456'"""
        addrs = PlatformAddrs()
        length = len(text)

        while pos < length:
            pos = self._skip_ws_comments(text, pos)
            if pos >= length:
                break
            if text[pos] in (';', '{'):
                break
            if text[pos] == ',':
                pos += 1
                continue

            # Match a word token
            m = re.match(r'(\w+)', text[pos:])
            if not m:
                break

            word = m.group(1)
            pos += m.end()

            # Bare 'inline' without a platform prefix
            if word == 'inline':
                continue

            if word not in self.PLATFORMS:
                continue

            platform = word
            pos = self._skip_ws_comments(text, pos)
            if pos >= length or text[pos] in (';', '{', ','):
                continue

            # Check for 'inline' or hex address
            if text[pos:pos+6] == 'inline':
                if platform == 'win':
                    addrs.win_inline = True
                pos += 6
            elif text[pos:pos+2] in ('0x', '0X'):
                hex_m = re.match(r'0x([0-9a-fA-F]+)', text[pos:], re.I)
                if hex_m:
                    addr_val = int(hex_m.group(1), 16)
                    if platform == 'win':
                        addrs.win = addr_val
                    elif platform == 'imac':
                        addrs.imac = addr_val
                    elif platform == 'm1':
                        addrs.m1 = addr_val
                    elif platform == 'ios':
                        addrs.ios = addr_val
                    pos += hex_m.end()

            pos = self._skip_ws_comments(text, pos)
            if pos < length and text[pos] == ',':
                pos += 1

        return addrs, pos

    def _find_matching(self, text: str, pos: int, open_ch: str, close_ch: str) -> tuple[int, int]:
        """Find matching close bracket. pos should be right after the open bracket."""
        depth = 1
        length = len(text)
        while pos < length and depth > 0:
            ch = text[pos]
            if ch == open_ch:
                depth += 1
            elif ch == close_ch:
                depth -= 1
                if depth == 0:
                    return pos, depth
            pos += 1
        return -1, depth

    def _skip_braces(self, text: str, pos: int) -> int:
        """Skip a brace-enclosed block starting at '{'."""
        if text[pos] != '{':
            return pos
        depth = 1
        pos += 1
        length = len(text)
        while pos < length and depth > 0:
            if text[pos] == '{':
                depth += 1
            elif text[pos] == '}':
                depth -= 1
            pos += 1
        return pos

    def _try_parse_member(self, text: str, pos: int) -> tuple[Optional[MemberDecl], int]:
        """Try parsing a member variable declaration."""
        length = len(text)
        # Find the semicolon
        semi = text.find(';', pos)
        if semi == -1 or semi - pos > 300:
            return None, pos

        # Check there's no '(' before the semicolon (that would be a func)
        chunk = text[pos:semi]
        # Remove any inline comments
        chunk = re.sub(r'//.*', '', chunk).strip()

        if '(' in chunk or not chunk:
            return None, pos

        # Split into type and name
        # Members look like: Type m_name or Type name
        # Handle templates: std::map<int, std::vector<int>> m_thing
        parts = self._split_member_type_name(chunk)
        if parts is None:
            return None, pos
        type_str, name = parts

        return MemberDecl(name=name, type=type_str), semi + 1

    def _split_member_type_name(self, decl: str) -> Optional[tuple[str, str]]:
        """Split a member declaration into type and name, handling templates."""
        decl = decl.strip()
        if not decl:
            return None

        # Find the last identifier after all template brackets are closed
        # Walk from the end
        i = len(decl) - 1
        # The name is at the end
        while i >= 0 and (decl[i].isalnum() or decl[i] == '_'):
            i -= 1

        if i < 0:
            return None

        name = decl[i+1:]
        type_str = decl[:i+1].strip()

        # Remove trailing * or & from type that might be stuck to name
        # Actually, in Broma format, pointers are part of the type
        if not name:
            return None
        if not type_str:
            return None

        return (type_str, name)


def parse_version(bindings_dir: Path, version: str) -> BromaParser:
    """Parse all .bro files for a given GD version."""
    parser = BromaParser()
    version_dir = bindings_dir / version

    # Parse in dependency order
    for bro_name in ["Extras.bro", "FMOD.bro", "Kazmath.bro", "Cocos2d.bro", "GeometryDash.bro"]:
        bro_path = version_dir / bro_name
        if bro_path.exists():
            parser.parse_file(bro_path)

    return parser


if __name__ == "__main__":
    import sys
    bindings_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("../bindings/bindings")
    version = sys.argv[2] if len(sys.argv) > 2 else "2.2091"

    parser = parse_version(bindings_dir, version)
    print(f"Parsed {len(parser.classes)} classes, {len(parser.free_functions)} free functions")

    win_funcs = 0
    inline_funcs = 0
    total_funcs = 0
    for cls in parser.classes.values():
        for f in cls.functions:
            total_funcs += 1
            if f.addrs.has_win():
                win_funcs += 1
            if f.addrs.win_inline:
                inline_funcs += 1
    for f in parser.free_functions:
        total_funcs += 1
        if f.addrs.has_win():
            win_funcs += 1

    print(f"Total functions: {total_funcs}")
    print(f"  With win address: {win_funcs}")
    print(f"  Win inline: {inline_funcs}")

    total_members = sum(len(c.members) for c in parser.classes.values())
    print(f"Total members: {total_members}")

    # Print a sample
    if "MenuLayer" in parser.classes:
        ml = parser.classes["MenuLayer"]
        print(f"\nMenuLayer: {len(ml.functions)} functions, {len(ml.members)} members")
        print(f"  Bases: {ml.bases}")
        for f in ml.functions[:5]:
            addr_str = hex(f.addrs.win) if f.addrs.win else ("inline" if f.addrs.win_inline else "none")
            print(f"  {f.signature()} = win {addr_str}")
