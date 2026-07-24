#!/usr/bin/env bash

set -euo pipefail

if (( EUID != 0 )); then
    echo "Run this script as root: sudo $0" >&2
    exit 1
fi

if ! command -v nft >/dev/null 2>&1; then
    echo "nft command not found" >&2
    exit 1
fi

if nft list table inet guard9 >/dev/null 2>&1; then
    echo "inet guard9 already exists; run clean-nft.sh first" >&2
    exit 1
fi

nft add table inet guard9
nft 'add chain inet guard9 output { type filter hook output priority 0; policy accept; }'
nft add rule inet guard9 output ip protocol tcp tcp dport 443 queue num 0 bypass

echo "guard9 outbound HTTPS queue rule installed"
nft list table inet guard9
