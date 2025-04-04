#!/bin/bash

INPUT=${1:-output/render.ppm}
OUTPUT="${INPUT%.ppm}.png"

if [ ! -f "$INPUT" ]; then
    echo "❌ Girdi dosyası bulunamadı: $INPUT"
    exit 1
fi

# convert veya magick convert hangisi varsa onu kullan
if command -v convert >/dev/null 2>&1; then
    CMD="convert"
elif command -v magick >/dev/null 2>&1; then
    CMD="magick convert"
else
    echo "❌ ImageMagick bulunamadi. Lütfen 'sudo apt install imagemagick' komutuyla yükleyin."
    exit 1
fi

$CMD "$INPUT" "$OUTPUT"
if [ $? -ne 0 ] || [ ! -f "$OUTPUT" ]; then
    echo "❌ Dönüştürme başarisiz oldu. PNG oluşmadi."
    exit 1
fi

echo "✅ PNG başariyla oluşturuldu: $OUTPUT"