#!/usr/bin/env python3
"""
Simplified analyzer for Clang -ftime-trace.
Reports total time spent in template instantiations containing 'ebd::',
without double-counting nested events.
Usage: python analyze_pro.py <trace.json>
"""

import json
import sys

def build_tree(events):
    """Build parent-child relationships based on time containment."""
    n = len(events)
    parent = [-1] * n
    for ev in events:
        ev['end'] = ev['ts'] + ev['dur']
    for i in range(n):
        best_parent = -1
        best_dur = float('inf')
        for j in range(n):
            if i == j:
                continue
            if events[j]['ts'] <= events[i]['ts'] and events[j]['end'] >= events[i]['end']:
                if events[j]['dur'] < best_dur:
                    best_dur = events[j]['dur']
                    best_parent = j
        parent[i] = best_parent
    return parent

def main():
    if len(sys.argv) != 2:
        print("Usage: python analyze_pro.py <trace.json>")
        sys.exit(1)

    trace_file = sys.argv[1]
    with open(trace_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    events = data.get('traceEvents', [])
    # Keep only complete events (ph='X') that have ts, dur, name, and args.detail
    keep = []
    for ev in events:
        if ev.get('ph') != 'X':
            continue
        if 'ts' not in ev or 'dur' not in ev or 'name' not in ev:
            continue
        if 'args' not in ev or 'detail' not in ev['args']:
            continue
        keep.append(ev)
    events = keep
    if not events:
        print("No complete events with timing and detail found.")
        sys.exit(0)

    # Sort by start time (just in case)
    events.sort(key=lambda e: e['ts'])

    # Build parent relationships
    parent = build_tree(events)

    # Identify instantiations that contain 'ebd::'
    total_us = 0
    matched_indices = []
    for i, ev in enumerate(events):
        detail = ev['args']['detail']
        name = ev['name']
        if 'ebd::' in detail and ('InstantiateClass' in name or 'InstantiateFunction' in name):
            # Check if this event is a root among matched events (no parent also matched)
            p = parent[i]
            is_root = True
            if p != -1:
                p_ev = events[p]
                p_detail = p_ev['args']['detail']
                p_name = p_ev['name']
                if 'ebd::' in p_detail and ('InstantiateClass' in p_name or 'InstantiateFunction' in p_name):
                    is_root = False
            if is_root:
                total_us += ev['dur']
                matched_indices.append(i)

    if not matched_indices:
        print("No 'ebd::' template instantiations found.")
        sys.exit(0)

    total_ms = total_us / 1000.0
    print(f"Found {len(matched_indices)} root instantiation(s) containing 'ebd::'.")
    print(f"Total time: {total_us:,} us ({total_ms:.3f} ms)")

if __name__ == "__main__":
    main()
