#!/usr/bin/env python3
import json
import os
import re
import subprocess
import sys
import urllib.request

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:7b"

CPP_PATH = "src/REL/d/a/npc/d_a_npc_knight_leader.cpp"
HEADER_PATH = "include/d/a/npc/d_a_npc_knight_leader.h"
TARGET_S_PATH = "knight_leader_target.s"
OBJDIFF_CLI = "build/tools/objdiff-cli.exe"
BUILD_TARGET = "build/SOUE01/src/REL/d/a/npc/d_a_npc_knight_leader.o"

def query_ollama(prompt, system_prompt="You are an expert reverse engineer and GameCube/Wii PowerPC C++ decompilation specialist using Metrowerks CodeWarrior 1.5."):
    payload = {
        "model": MODEL_NAME,
        "prompt": prompt,
        "system": system_prompt,
        "stream": False,
        "options": {
            "temperature": 0.2,
            "num_predict": 1024,
        }
    }
    req = urllib.request.Request(
        OLLAMA_URL,
        data=json.dumps(payload).encode("utf-8"),
        headers={"Content-Type": "application/json"}
    )
    with urllib.request.urlopen(req) as response:
        data = json.loads(response.read().decode("utf-8"))
        return data.get("response", "")

def run_ninja():
    cmd = ["ninja", BUILD_TARGET]
    res = subprocess.run(cmd, capture_output=True, text=True)
    return res.returncode == 0, res.stdout + res.stderr

def get_knight_leader_match():
    report_file = "temp_report.json"
    cmd = [OBJDIFF_CLI, "report", "generate", "-o", report_file]
    subprocess.run(cmd, capture_output=True, text=True)
    if not os.path.exists(report_file):
        return None
    try:
        with open(report_file, "r") as f:
            data = json.load(f)
        for unit in data.get("units", []):
            if "d_a_npc_knight_leaderNP/REL/d/a/npc/d_a_npc_knight_leader" in unit.get("name", ""):
                fns = {}
                for fn in unit.get("functions", []):
                    fns[fn.get("name", "")] = fn.get("fuzzy_match_percent", 0.0)
                return fns
    except Exception as e:
        print(f"Error reading report: {e}")
    finally:
        if os.path.exists(report_file):
            try:
                os.remove(report_file)
            except:
                pass
    return None

def main():
    print(f"[*] Starting Ollama Decompilation Assistant ({MODEL_NAME})...")
    
    # 1. Test build
    ok, out = run_ninja()
    if not ok:
        print("[!] Initial build failed:")
        print(out)
        return
    print("[+] Initial build succeeded!")
    
    # 2. Check current match
    matches = get_knight_leader_match()
    if matches:
        print(f"[+] Current unit status:")
        for name, pct in matches.items():
            print(f"    - {name}: {pct:.2f}%")
    
    # 3. Read target disassembly
    if not os.path.exists(TARGET_S_PATH):
        print(f"[!] Target assembly {TARGET_S_PATH} not found.")
        return
    
    with open(TARGET_S_PATH, "r") as f:
        target_s = f.read()
    
    print("[+] Ready for automated decompilation pipeline.")

if __name__ == "__main__":
    main()
