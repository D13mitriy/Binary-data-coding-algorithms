#!/bin/bash
# ==================================================
# cleanup_and_stage_allowed.sh
# Cleans root (keeps dirs), then adds all files in
# subfolders that are NOT ignored by .gitignore.
# ==================================================

set -euo pipefail

echo "🔍 Checking current directory..."
if [ ! -d ".git" ]; then
  echo "❌ Error: This script must be run from the root of a Git repo!"
  exit 1
fi

echo "🧹 Cleaning up files in repository root..."
# Delete all files in the repo root except .gitignore and this script
find . -maxdepth 1 -type f \
  ! -name ".gitignore" \
  ! -name "$(basename "$0")" \
  -exec rm -v {} \;

echo "✅ Root cleanup done."

echo "📁 Finding and staging non-ignored files inside subfolders..."

# Find all files in subdirectories (depth >= 2)
while IFS= read -r -d '' file; do
  # Check if the file is ignored by Git
  if git check-ignore -q "$file"; then
    echo "🚫 Ignored: $file"
  else
    echo "➕ Adding: $file"
    git add "$file"
  fi
done < <(find . -mindepth 2 -type f -print0)

echo "✅ Done staging all non-ignored files."

echo "🧾 Summary:"
git status --short

echo "✨ You can now commit your changes with:"
echo "   git commit -m 'Clean root and stage all non-ignored files'"

