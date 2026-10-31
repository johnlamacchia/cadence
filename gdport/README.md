# gdport: carry GD addresses to a new exe version

```
pip install capstone
python gdport.py <old.exe> <old addresses.json> <new.exe> [-o addresses_new.json]
```

You normally run it through `python tools/sdk_update.py --old 2.2091 --new 2.2092`, which also
copies the exe, reads the new class sizes, regenerates headers and reports layout changes.

It never guesses: a function is reported only when exactly one candidate fits, and results are
validated before they are written. Anything doubtful goes into the `unmatched` list for a human.

## How it works

1. **Function boundaries.** Every x64 function that needs unwind data is listed in the exe's
   `.pdata`; leaf functions are found from the `int3` padding between functions. (Scanning for
   prologue bytes, which an earlier version did, produces addresses in the middle of functions.)
2. **Fingerprints** for every function in both exes:
   - *exact*: its bytes, with relative branch targets and RIP-relative displacements masked;
   - *shape*: its sequence of mnemonics;
   - the *string literals* it references;
   - the functions it calls.
3. **Unique matches.** An old function is matched to a new one when its fingerprint occurs exactly
   once in each exe: exact bytes, then strings + shape, then shape alone (10+ instructions).
   Functions under 4 instructions are never matched this way: they can be byte-identical to a
   sibling (`onNextPage` / `onPrevPage`), so their bytes prove nothing.
4. **Propagation.** Matched functions anchor the rest, over several rounds:
   - *call graph*: an unmatched function's already-matched callees and callers must all agree
     on one candidate;
   - *layout order*: between two matched neighbours, if old and new contain the same number of
     functions, they pair up in order.
5. **Validation.** Matches are one-to-one and must keep the exe's layout order (the largest
   order-preserving subset is kept; crossing matches are rejected).

Output is the same JSON shape as `generated/addresses_<ver>.json`, plus `unmatched` and `stats`.

## What was measured

`python selftest.py <exe> <addresses.json>` runs two tests with a known answer (every function maps
to its own address), on the 6,841 functions in `addresses_2.2091.json`:

| Test | Correct | Wrong | Left for a human |
|---|---|---|---|
| **identity**: the new exe is the old exe | 6,786 | **0** | 55 |
| **mutated**: an instruction changed in 30% of the functions (1,877 altered), layout unchanged | 6,786 | **0** | 55 |

In the mutated test, 99.2% of the altered functions (2,015 of 2,031) were still recovered, through
shape, strings, the call graph or layout order. Each run takes about a minute.

The 55 left over are tiny or duplicated functions that cannot be told apart from their neighbours.
The test earned its keep: it found that exact-byte matching places two near-identical 2-instruction
functions wrongly, which is why step 3 refuses tiny functions.

**What it does not prove.** Both tests keep every function at its original address; neither moves
code the way a real patch does. The matcher uses content and relative order, never absolute
addresses, so it should not care, but it has not yet been run on two genuinely different GD
versions. Check the result of a real update with `tools/verify_addresses.py` before trusting it.
