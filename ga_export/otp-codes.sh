#!/bin/bash

qrcode="$(zbarimg $1 2>/dev/null)"
url="${qrcode/#QR-Code:}"
echo "Parsing: $url"
echo "-----------------------"
echo ""
node index.js "$url"