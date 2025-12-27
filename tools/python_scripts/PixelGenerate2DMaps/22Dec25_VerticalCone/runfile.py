import json, re
from pathlib import Path

def scrub_json_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    out = []
    for line in text.splitlines():
        i = line.find("//")
        out.append(line[:i] if i >= 0 else line)
    return "\n".join(out)

def load_jsonc(path: str):
    raw = Path(path).read_text(encoding="utf-8")
    return json.loads(scrub_json_comments(raw))

def is_header_row(row):
    # header row is strings like ["rowtype","left","middle","right"]
    return any(isinstance(x, str) for x in row)

def expand_chunk(left: int, mid: int, right: int, direction: str):
    """
    Expand physical indices for one chunk in the requested electrical direction.
    We assume two contiguous runs split at mid:
      left..mid  and  (mid+1)..right
    Direction flips whether we traverse left->right or right->left.
    """
    if right < left:
        raise ValueError(f"Invalid range left={left} right={right}")

    if mid < left or mid > right:
        # if you ever want "no seam", set mid==left or mid==right; otherwise clamp or error
        raise ValueError(f"mid out of range: left={left} mid={mid} right={right}")

    if direction == "lr":
        # left to mid, then mid+1 to right
        a = list(range(left, mid + 1))
        b = list(range(mid + 1, right + 1))
        return a + b
    else:
        # right to mid+1, then mid to left
        a = list(range(right, mid, -1))     # right..(mid+1)
        b = list(range(mid, left - 1, -1))  # mid..left
        return a + b

def generate_ledmap_from_leftmidright(cfg):
    rows = cfg["pixels_LeftMiddleRight"]
    opts = cfg.get("options", {})
    serpentine = bool(opts.get("serpentine", True))
    across_mode = str(opts.get("across_mode", "exclude")).lower()
    if across_mode not in ("exclude", "append_rows"):
        raise ValueError('options.across_mode must be "exclude" or "append_rows"')

    # Drop header if present
    if rows and is_header_row(rows[0]):
        rows = rows[1:]

    # Separate vertical rows vs across rows
    vertical = []
    across   = []
    for entry in rows:
        if len(entry) != 4:
            raise ValueError(f"Each entry must be [rowtype,left,middle,right]. Got: {entry}")
        rowtype, left, mid, right = entry
        if int(rowtype) == 1:
            vertical.append((int(left), int(mid), int(right)))
        elif int(rowtype) == 0:
            across.append((int(left), int(mid), int(right)))
        else:
            raise ValueError(f"Unknown rowtype {rowtype} in {entry}")

    # Width: use the maximum pixel count of any *vertical* row (right-left+1)
    # (This is the “scaled to widest row” idea)
    widths = [(r - l + 1) for (l, m, r) in vertical]
    max_w = max(widths) if widths else 0

    # Height: number of vertical rows + optional appended across rows
    extra_h = 0
    if across_mode == "append_rows":
        # simplest: append each across chunk as its own additional row
        extra_h = len(across)
    h = len(vertical) + extra_h

    # Build WLED map = [h][max_w], init -1
    ledmap = [[-1 for _ in range(max_w)] for _ in range(h)]

    # Place vertical rows first
    for row_i, (l, m, r) in enumerate(vertical):
        # Determine electrical direction for this vertical row
        # serpentine: even rows LR, odd rows RL (you can flip later)
        direction = "lr" if (not serpentine or (row_i % 2 == 0)) else "rl"
        pix = expand_chunk(l, m, r, direction)

        # Scale/fit into max_w: left align with -1 padding on the right
        # If you want centering, we can do that too.
        row_w = len(pix)
        if row_w > max_w:
            raise ValueError(f"Row wider than max_w? row_w={row_w} max_w={max_w}")

        for x in range(row_w):
            ledmap[row_i][x] = pix[x]
        # remaining cells stay -1

    # Handle across rows (travellers)
    if across_mode == "append_rows":
        base = len(vertical)
        for j, (l, m, r) in enumerate(across):
            pix = expand_chunk(l, m, r, "lr")  # across direction is usually fixed; tweak later if needed
            row_w = len(pix)
            if row_w > max_w:
                # either allow widening or clip; for now widen would be “intelligent”, but WLED wants rectangle
                raise ValueError(f"Across row wider than max_w: {row_w} > {max_w}")
            for x in range(row_w):
                ledmap[base + j][x] = pix[x]

    # If across_mode == "exclude": do nothing (they exist physically, but not mapped)
    return {"map": ledmap}

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("config", help="layout jsonc")
    args = ap.parse_args()

    cfg = load_jsonc(args.config)
    outname = cfg.get("output", "ledmap.json")
    ledmap_obj = generate_ledmap_from_leftmidright(cfg)
    Path(outname).write_text(json.dumps(ledmap_obj, indent=2), encoding="utf-8")
    print(f"Wrote {outname}: rows={len(ledmap_obj['map'])}, cols={(len(ledmap_obj['map'][0]) if ledmap_obj['map'] else 0)}")

if __name__ == "__main__":
    main()
