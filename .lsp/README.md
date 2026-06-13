LSP setup (editor-agnostic)

This folder contains editor-agnostic helpers to get Language Servers running for this workspace.

Supported language servers in this setup:
- clangd (C/C++)
- pyright (Python)
- typescript-language-server + typescript (JS/TS)
- bash-language-server (Bash)

How LSPs activate
- Language servers will activate when an editor or LSP client opens a file belonging to the language and the client discovers the corresponding server on PATH.
- This repository provides minimal project-level configuration files (pyrightconfig.json, tsconfig.json, .clangd) and a helper installer script. Your editor still needs to be configured to use the server binaries (most editors auto-discover servers on PATH).

Quick install (recommended)
1. Ensure you have Node.js/npm installed (or use a Node version manager).
2. Run the installer script shipped here:

   bash .lsp/setup-lsp.sh

What the script does
- Installs pyright, typescript, typescript-language-server and bash-language-server via npm (global install).
- Attempts to detect clangd. If clangd is not present it prints platform-specific instructions (apt/brew) because packaging differs by OS.

Notes and caveats
- Global npm installs may require elevated permissions on some systems. Prefer to configure npm global prefix to a user-writable location (see npm docs) or use a Node version manager (nvm, fnm, asdf).
- clangd is usually provided by your system package manager or by downloading a prebuilt release from LLVM. The script will not perform sudo installs automatically.
- For C/C++ projects clangd works best when a compile_commands.json is present (CMake: set CMAKE_EXPORT_COMPILE_COMMANDS=ON). Without it clangd falls back to heuristics.

Editor integration
- VS Code, Neovim, Emacs, Kakoune and others can auto-detect servers on PATH. See your editor's LSP plugin docs if servers are not discovered.

If you want help wiring these servers into a specific editor, tell me which editor and I will provide the precise configuration snippet.
