#!/usr/bin/env bash

input=$(cat)

# Parse JSON with Node (jq not available on this machine)
cwd=$(node -e "let d=''; process.stdin.on('data',c=>d+=c).on('end',()=>{const j=JSON.parse(d); process.stdout.write(j?.workspace?.current_dir||j?.cwd||'')})" <<< "$input" 2>/dev/null)
used_pct=$(node -e "let d=''; process.stdin.on('data',c=>d+=c).on('end',()=>{const j=JSON.parse(d); process.stdout.write(String(j?.context_window?.used_percentage??''))})" <<< "$input" 2>/dev/null)

branch=""
if [ -n "$cwd" ]; then
  branch=$(git -C "$cwd" --no-optional-locks rev-parse --abbrev-ref HEAD 2>/dev/null)
fi

pr_info=""
if [ -n "$branch" ] && [ "$branch" != "HEAD" ]; then
  gh_bin="/c/Program Files/GitHub CLI/gh.exe"
  if [ -x "$gh_bin" ]; then
    pr_number=$("$gh_bin" pr list --head "$branch" --state open --json number --jq '.[0].number' 2>/dev/null)
    [ -n "$pr_number" ] && pr_info="PR#${pr_number}"
  fi
fi

ctx_info=""
if [ -n "$used_pct" ] && [ "$used_pct" != "None" ]; then
  ctx_info=$(printf "ctx:%.0f%%" "$used_pct")
fi

parts=()
[ -n "$branch" ] && parts+=("$branch")
[ -n "$pr_info" ] && parts+=("$pr_info")
[ -n "$ctx_info" ] && parts+=("$ctx_info")

if [ ${#parts[@]} -gt 0 ]; then
  printf "%s" "$(IFS=' | '; echo "${parts[*]}")"
fi
