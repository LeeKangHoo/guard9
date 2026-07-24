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

if ! nft list table inet guard9 >/dev/null 2>&1; then
    echo "inet guard9 does not exist; nothing to clean"
    exit 0
fi

nft delete table inet guard9
echo "guard9 nftables rules removed"
