
# Silent File Drop – Reverse Engineering and Detection Analysis

This repository contains a behavioral and reverse engineering analysis of a Windows PE executable that silently hides its console window and writes a file to disk without user interaction. The binary was analyzed using Ghidra, Process Monitor, and manual sandboxing.

---

## Objective

Analyze a compiled Windows executable with no source code provided. Identify functionality through reverse engineering, evaluate threat relevance, and propose viable detection strategies based on both static and behavioral signals.

---

## Static Analysis (Ghidra)

**Summary:**
- The binary retrieves its console window handle using `GetConsoleWindow` and hides it using `ShowWindow(hwnd, SW_HIDE)`.
- It constructs a hardcoded file path `C:\Users\Public\out.txt`.
- It creates this file using `CreateFileA` with `GENERIC_WRITE` and the `FILE_ATTRIBUTE_HIDDEN` flag.
- The string `"Reverse me."` is written to the file using `WriteFile`.
- No execution, network, persistence, or privilege escalation was observed.

**Relevant API Chain:**
```c
GetConsoleWindow → ShowWindow → lstrcpyA → lstrcatA → CreateFileA → WriteFile
```

**Decompiled Flow (Ghidra):**
```c
int main() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    char fullPath[MAX_PATH] = "C:\Users\Public\out.txt";
    HANDLE hFile = CreateFileA(
        fullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_HIDDEN, NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        const char* msg = "Reverse me.";
        DWORD written;
        WriteFile(hFile, msg, lstrlenA(msg), &written, NULL);
        CloseHandle(hFile);
    }

    return 0;
}
```

---

## Dynamic Behavior (Observed)

- Console window flickers briefly, then hides.
- File is created: `C:\Users\Public\out.txt`
- Contents: `Reverse me.`
- Defender did **not** flag the binary during testing on Windows 11 (24H2).
- File attribute is `HIDDEN`.

---

## Threat Modeling & Intent

**MITRE ATT&CK Mapping:**
- **T1564.001 (Hidden Files and Directories):**
  - https://attack.mitre.org/techniques/T1564/001/
- **T1204.002 (User Execution: Malicious File):**
  - https://attack.mitre.org/techniques/T1204/002/

**Why It Matters:**
- Dropping files in `C:\Users\Public\` is common for:
  - Staging configuration
  - Host signaling
  - Loader output
- The use of `FILE_ATTRIBUTE_HIDDEN` and lack of user interaction suggests **intentional stealth**, often seen in first-stage loaders or sandbox-aware droppers.

---

## Detection Opportunities

| Layer              | Signal                                                                 |
|-------------------|------------------------------------------------------------------------|
| **EDR Telemetry** | ShowWindow + CreateFileA in short-lived processes                      |
| **File System**   | Hidden file creation in `C:\Users\Public\`                          |
| **Behavioral**    | File creation + immediate exit; no user interaction or output logged   |
| **YARA Rules**    | Hardcoded string match: `"Reverse me."` or Windows API chain patterns  |

---

## Takeaways

- This binary is not malicious by itself, but exhibits **stage-0 loader behavior**.
- Defenders should correlate **API-level visibility**, **memory protection**, and **filesystem events** to uncover early-stage droppers that attempt to avoid user suspicion.

---

## License

MIT License – provided for educational and research purposes only.
