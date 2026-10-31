#!/usr/bin/env python3
"""
gdport: carry function addresses from one GeometryDash.exe to the next.

    pip install capstone
    python gdport.py <old.exe> <old addresses.json> <new.exe> [-o addresses_new.json]

It never guesses. A function is only reported when exactly one candidate fits, and
every result is checked for consistency before it is written; anything doubtful goes
to the `unmatched` list for a human.

How it works (see README.md for the measured results):
  1. Function boundaries. Every x64 function that needs unwind data is listed in the
     exe's .pdata; leaf functions are found from the int3 padding between functions.
  2. Fingerprints. Each function gets an exact hash (its bytes with relative branch
     targets and RIP-relative displacements masked), a shape hash (its mnemonics),
     the string literals it references, and the functions it calls.
  3. Unique matches. A function is matched when its fingerprint occurs exactly once in
     the old exe and exactly once in the new one: exact bytes, then strings + shape,
     then shape alone (for functions of 10+ instructions).
  4. Propagation. Matched functions anchor the rest: an unmatched function is matched
     through the call graph (everything it calls or is called by that is already matched
     must agree on one candidate), and through layout order (the functions between two
     matched neighbours, when old and new have the same number of them).
  5. Validation. Matches must be one-to-one and keep the exe's layout order; matches
     that cross the dominant ordering are rejected.
"""
import argparse
import bisect
import hashlib
import json
import re
import struct
import sys
import time
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

BRANCHES = {
    "call", "jmp", "jo", "jno", "jb", "jae", "je", "jne", "jbe", "ja", "js", "jns", "jp", "jnp",
    "jl", "jge", "jle", "jg", "jcxz", "jecxz", "jrcxz", "loop", "loope", "loopne",
}
MIN_EXACT_INSNS = 4           # a 2-3 instruction function can be byte-identical to its sibling
MIN_SHAPE_INSNS = 10          # shape-only matching is too weak below this
MAX_ROUNDS = 10


# --------------------------------------------------------------------------- PE

class Image:
    """A PE32+ file mapped the way the loader maps it, so an RVA indexes `mem` directly."""

    def __init__(self, path):
        raw = Path(path).read_bytes()
        e = struct.unpack_from("<I", raw, 0x3C)[0]
        nsec, = struct.unpack_from("<H", raw, e + 6)
        opt_size, = struct.unpack_from("<H", raw, e + 20)
        opt = e + 24
        if struct.unpack_from("<H", raw, opt)[0] != 0x20B:
            raise SystemExit(f"{path}: not a 64-bit PE")
        self.base, = struct.unpack_from("<Q", raw, opt + 24)
        size_of_image, size_of_headers = struct.unpack_from("<II", raw, opt + 56)
        self.exception = struct.unpack_from("<II", raw, opt + 112 + 3 * 8)    # data directory 3
        self.mem = bytearray(size_of_image)
        self.mem[:size_of_headers] = raw[:size_of_headers]
        self.sections = []
        sec = opt + opt_size
        for i in range(nsec):
            vsize, va, rawsize, rawoff = struct.unpack_from("<IIII", raw, sec + i * 40 + 8)
            flags, = struct.unpack_from("<I", raw, sec + i * 40 + 36)
            n = min(rawsize, vsize)
            self.mem[va:va + n] = raw[rawoff:rawoff + n]
            self.sections.append((va, va + vsize, flags))
        self.exec = [(lo, hi) for lo, hi, f in self.sections if f & 0x20000000]
        self.data = [(lo, hi) for lo, hi, f in self.sections if not f & 0x20000000]

    def exec_end(self, rva):
        for lo, hi in self.exec:
            if lo <= rva < hi:
                return hi
        return None

    def in_data(self, rva):
        return any(lo <= rva < hi for lo, hi in self.data)


def pdata_ranges(img):
    rva, size = img.exception
    out = []
    for i in range(size // 12):
        b, e, _ = struct.unpack_from("<III", img.mem, rva + i * 12)
        if b and e > b and img.exec_end(b):
            out.append((b, e))
    return sorted(out)


def find_starts(img, extra=()):
    """Function start addresses: .pdata entries, plus leaf functions found after int3 padding."""
    ranges = pdata_ranges(img)
    begins = [b for b, _ in ranges]
    starts = set(begins) | set(extra)
    for lo, hi in img.exec:
        for m in re.finditer(rb"\xCC+", bytes(img.mem[lo:hi])):
            p = lo + m.end()
            if p >= hi or img.mem[p] == 0xCC:
                continue
            i = bisect.bisect_right(begins, p) - 1
            if i >= 0 and begins[i] < p < ranges[i][1]:
                continue                       # inside a function that has unwind data
            starts.add(p)
    return sorted(s for s in starts if img.exec_end(s))


# ------------------------------------------------------------------ fingerprints

@dataclass
class Fn:
    start: int
    size: int
    ninsn: int
    exact: bytes
    shape: bytes
    strings: frozenset
    calls: list = field(default_factory=list)      # absolute targets of direct calls / tail jumps


def digest(b):
    return hashlib.blake2b(b, digest_size=12).digest()


def read_string(img, rva):
    mem = img.mem
    end = rva
    while end < len(mem) and end - rva < 200 and 0x20 <= mem[end] < 0x7F:
        end += 1
    if end - rva >= 4 and end < len(mem) and mem[end] == 0:
        return bytes(mem[rva:end]).decode("ascii")
    return None


def analyze(img, starts):
    try:
        import capstone
    except ImportError:
        sys.exit("gdport needs capstone: pip install capstone")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    md.detail = True
    mem = img.mem
    out = {}
    for idx, s in enumerate(starts):
        e = starts[idx + 1] if idx + 1 < len(starts) else img.exec_end(s)
        e = min(e, img.exec_end(s))
        while e > s and mem[e - 1] == 0xCC:
            e -= 1
        if e <= s:
            continue
        code = bytes(mem[s:e])
        masked = bytearray(code)
        mnemonics, strings, calls = [], set(), []
        for ins in md.disasm(code, s):
            m = ins.mnemonic
            mnemonics.append(m)
            off = ins.address - s
            if m in BRANCHES:
                if ins.imm_size == 4 and ins.op_str.startswith("0x"):
                    masked[off + ins.imm_offset:off + ins.imm_offset + 4] = b"\0\0\0\0"
                    tgt = int(ins.op_str, 16)
                    if m == "call" or (m == "jmp" and not s <= tgt < e):
                        calls.append(tgt)
            elif ins.disp_size == 4 and (ins.modrm & 0xC7) == 0x05:        # [rip + disp32]
                d, = struct.unpack_from("<i", code, off + ins.disp_offset)
                masked[off + ins.disp_offset:off + ins.disp_offset + 4] = b"\0\0\0\0"
                tgt = ins.address + ins.size + d
                if img.in_data(tgt):
                    t = read_string(img, tgt)
                    if t:
                        strings.add(t)
        out[s] = Fn(s, e - s, len(mnemonics), digest(bytes(masked)), digest(" ".join(mnemonics).encode()),
                    frozenset(strings), calls)
    return out


# ----------------------------------------------------------------- the matcher

def lis_keep(pairs):
    """pairs: [(old, new)] sorted by old. Return the largest subset whose `new` strictly increases."""
    tails, tail_idx, prev = [], [], [-1] * len(pairs)
    for i, (_, n) in enumerate(pairs):
        k = bisect.bisect_left(tails, n)
        if k == len(tails):
            tails.append(n)
            tail_idx.append(i)
        else:
            tails[k] = n
            tail_idx[k] = i
        prev[i] = tail_idx[k - 1] if k else -1
    keep, i = set(), tail_idx[-1] if tail_idx else -1
    while i != -1:
        keep.add(pairs[i][0])
        i = prev[i]
    return keep


class Matcher:
    def __init__(self, old, new, log):
        self.old, self.new, self.log = old, new, log
        self.match = {}                # old start -> new start
        self.how = {}                  # old start -> method
        self.rejected = set()
        self.used_new = set()
        self.old_starts = sorted(old)
        self.new_starts = sorted(new)
        self.callees_old = {s: [t for t in f.calls if t in old and t != s] for s, f in old.items()}
        self.callees_new = {s: [t for t in f.calls if t in new and t != s] for s, f in new.items()}
        self.callers_old, self.callers_new = defaultdict(set), defaultdict(set)
        for s, ts in self.callees_old.items():
            for t in ts:
                self.callers_old[t].add(s)
        for s, ts in self.callees_new.items():
            for t in ts:
                self.callers_new[t].add(s)

    def add(self, o, n, how):
        if o in self.match or n in self.used_new or o in self.rejected:
            return False
        self.match[o] = n
        self.how[o] = how
        self.used_new.add(n)
        return True

    # ---- unique fingerprints ----
    def unique_pass(self, name, key, eligible=lambda f: True):
        oi, ni = defaultdict(list), defaultdict(list)
        for s, f in self.old.items():
            if s not in self.match and eligible(f):
                oi[key(f)].append(s)
        for s, f in self.new.items():
            if s not in self.used_new and eligible(f):
                ni[key(f)].append(s)
        added = 0
        for k, olds in oi.items():
            news = ni.get(k)
            if len(olds) == 1 and news and len(news) == 1:
                added += self.add(olds[0], news[0], name)
        return added

    # ---- plausibility of a propagated candidate ----
    def compatible(self, o, n):
        fo, fn = self.old[o], self.new[n]
        if fo.strings and fn.strings and fo.strings != fn.strings:
            return False
        lo, hi = sorted((fo.ninsn, fn.ninsn))
        return hi <= 2 * lo + 4

    # ---- call graph ----
    def callgraph_round(self):
        added = 0
        for o in self.old_starts:
            if o in self.match or o in self.rejected:
                continue
            evidence = []
            for c in self.callees_old[o]:
                if c in self.match:
                    evidence.append(self.callers_new[self.match[c]])
            for c in self.callers_old[o]:
                if c in self.match:
                    evidence.append(set(self.callees_new[self.match[c]]))
            if not evidence:
                continue
            votes = Counter(n for ev in evidence for n in ev if n not in self.used_new and self.compatible(o, n))
            if not votes:
                continue
            best, score = votes.most_common(1)[0]
            tied = [n for n, v in votes.items() if v == score]
            if len(tied) != 1 or score < len(evidence):
                continue                                           # must agree with ALL matched neighbours
            if score >= 2 or self.old[o].shape == self.new[best].shape:
                added += self.add(o, best, "callgraph")
        return added

    # ---- layout order ----
    def neighbour_round(self):
        added = 0
        anchors = sorted(self.match)
        for a, b in zip(anchors, anchors[1:]):
            na, nb = self.match[a], self.match[b]
            if nb <= na:
                continue
            olds = self.old_starts[bisect.bisect_right(self.old_starts, a):bisect.bisect_left(self.old_starts, b)]
            news = self.new_starts[bisect.bisect_right(self.new_starts, na):bisect.bisect_left(self.new_starts, nb)]
            if not olds or len(olds) != len(news) or len(olds) > 40:
                continue
            todo = [(o, n) for o, n in zip(olds, news) if o not in self.match and o not in self.rejected]
            if todo and all(self.compatible(o, n) for o, n in zip(olds, news)):
                for o, n in todo:
                    added += self.add(o, n, "neighbour")
        return added

    # ---- validation ----
    def purge(self):
        pairs = sorted(self.match.items())
        keep = lis_keep(pairs)
        bad = [o for o, _ in pairs if o not in keep]
        for o in bad:
            self.used_new.discard(self.match.pop(o))
            self.how.pop(o, None)
            self.rejected.add(o)
        return len(bad)

    def run(self):
        t = lambda s: self.log(f"  {s}")
        n = self.unique_pass("exact", lambda f: f.exact, lambda f: f.ninsn >= MIN_EXACT_INSNS)
        n += self.unique_pass("strings+shape", lambda f: (f.shape, f.strings),
                              lambda f: bool(f.strings) and f.ninsn >= MIN_EXACT_INSNS)
        n += self.unique_pass("shape", lambda f: f.shape, lambda f: f.ninsn >= MIN_SHAPE_INSNS)
        t(f"unique fingerprints: {n} matched")
        t(f"order check rejected {self.purge()}")
        for r in range(1, MAX_ROUNDS + 1):
            c, nb = self.callgraph_round(), self.neighbour_round()
            rej = self.purge()
            t(f"round {r}: call graph +{c}, layout order +{nb}, order check rejected {rej}")
            if not (c or nb):
                break


def port(old_exe, old_addrs, new_exe, log=print):
    t0 = time.time()
    old_img, new_img = Image(old_exe), Image(new_exe)
    known = {sig: int(a, 16) for sig, a in old_addrs.items()}
    log("Finding functions...")
    old_starts = find_starts(old_img, extra=known.values())
    new_starts = find_starts(new_img)
    log(f"  {len(old_starts)} functions in the old exe, {len(new_starts)} in the new")
    log("Fingerprinting...")
    old, new = analyze(old_img, old_starts), analyze(new_img, new_starts)
    log("Matching...")
    m = Matcher(old, new, log)
    m.run()

    addresses, unmatched = {}, []
    for sig, rva in known.items():
        if rva in m.match:
            addresses[sig] = m.match[rva]
        else:
            unmatched.append(sig)
    methods = Counter(m.how[known[s]] for s in addresses)
    log(f"Done in {time.time() - t0:.0f}s")
    return addresses, unmatched, dict(methods), len(m.rejected)


def main():
    ap = argparse.ArgumentParser(description="Port GD function addresses to a new exe version")
    ap.add_argument("old_exe")
    ap.add_argument("old_addresses", help="addresses_<old>.json")
    ap.add_argument("new_exe")
    ap.add_argument("-o", "--output", default="addresses_new.json")
    args = ap.parse_args()

    data = json.load(open(args.old_addresses))
    old_addrs = data["addresses"]
    print(f"{len(old_addrs)} functions from GD {data.get('version', '?')}")
    addresses, unmatched, methods, rejected = port(args.old_exe, old_addrs, args.new_exe, print)

    print(f"\nPlaced {len(addresses)} / {len(old_addrs)} ({100 * len(addresses) / len(old_addrs):.1f}%)  by {methods}")
    print(f"Left for a human: {len(unmatched)}   (order check rejected {rejected} doubtful matches)")
    out = {
        "version": "new",
        "ported_from": data.get("version", "unknown"),
        "count": len(addresses),
        "stats": {"by_method": methods, "order_check_rejected": rejected},
        "addresses": {sig: f"0x{rva:x}" for sig, rva in sorted(addresses.items())},
        "unmatched": sorted(unmatched),
    }
    Path(args.output).write_text(json.dumps(out, indent=2), encoding="utf-8")
    print(f"Written to {args.output}")


if __name__ == "__main__":
    main()
