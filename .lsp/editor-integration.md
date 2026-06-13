Editor-agnostic LSP integration notes

1) Servers on PATH
- Most editors' LSP clients discover servers by looking for known binaries on PATH. After running .lsp/setup-lsp.sh, ensure your editor session can see the same PATH (restart the editor or source your shell profile in GUI apps).

2) Per-project config files
- pyrightconfig.json (Python), tsconfig.json (TS/JS) and .clangd (clangd flags) are provided at the workspace root under .lsp/. You can copy them to the repository root if your LSP client expects them there.

3) Activating when files are opened
- LSP servers will start when an LSP client opens a file of the corresponding language. If a server doesn't start, check the editor logs and ensure the binary is available.

4) Optional: Create symlinks
- If your LSP client searches for config files at the repository root, you can create symlinks:

  ln -s .lsp/pyrightconfig.json pyrightconfig.json
  ln -s .lsp/tsconfig.json tsconfig.json
  ln -s .lsp/clangd .clangd

5) Security note
- The setup script performs npm global installs; review your org policies about global installations. Use a node version manager if needed.
