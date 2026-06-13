#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
echo "Workspace: $ROOT_DIR"

echo "Installing LSP helpers via npm (pyright, typescript, tsserver, bash-language-server)..."

if ! command -v npm >/dev/null 2>&1; then
  echo "npm not found. Please install Node.js/npm first." >&2
  exit 1
fi

# Use npm to install useful language servers globally
npm install -g pyright typescript typescript-language-server bash-language-server || {
  echo "npm global install failed. Consider running with a user-global prefix or use nvm/fnm." >&2
}

echo "Checking for clangd..."
if command -v clangd >/dev/null 2>&1; then
  echo "clangd found: $(command -v clangd)"
else
  echo "clangd not found. Please install clangd (system package manager or download from LLVM releases)." >&2
  echo "Examples: apt install clangd-14 (Debian/Ubuntu), brew install llvm (macOS)" >&2
fi

echo "Done. Ensure your editor's LSP client can find these binaries on PATH. See .lsp/README.md for details."
