# gdport — GD address porting tool

Carries function addresses from one GeometryDash.exe version to the next
by fingerprinting each function and matching by:
1. String references (highest confidence)
2. Exact byte sequences (with addresses masked)
3. Loose instruction shape (opcode sequence, ignoring operands)
4. Call graph neighbors (functions that call or are called by matched functions)
5. Validation pass (verify matches by checking callers/strings make sense)

## Usage

```
python gdport.py <old_exe> <old_addresses.json> <new_exe> [-o output.json]
```

Example:
```
python gdport.py ../binaries/2.2091/GeometryDash.exe \
                  ../generated/addresses_2.2091.json \
                  ../binaries/2.2092/GeometryDash.exe \
                  -o ../generated/addresses_2.2092.json
```

## How it works

1. **Fingerprint**: For each known function in the old exe, compute:
   - Referenced string literals
   - Byte pattern with addresses/offsets masked to wildcards
   - Opcode-only sequence (instruction mnemonics without operands)
   - Direct call targets and callers

2. **Match**: For each function in the new exe:
   - String match: if it references the exact same strings → high confidence
   - Exact match: if masked byte pattern is identical → medium-high confidence
   - Shape match: if opcode sequence matches → medium confidence
   - Neighbor match: if most call targets/callers already matched → lower confidence

3. **Validate**: Check that matched functions have plausible callers and string
   references in the new binary.

## Output

A JSON file in the same format as the codegen output, with:
- `matched`: functions successfully ported (with confidence score)
- `unmatched`: functions that couldn't be placed (for manual IDA work)
- `stats`: match counts by method

## Accuracy

On test builds with known ground truth: ~96% correct, 0% wrong (unmatched
functions are reported as unknown rather than guessed).
