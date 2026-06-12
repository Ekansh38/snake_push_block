#!/bin/bash
# Builds a self-contained snake_push_block.app in build/.
# Copies every Homebrew dylib the game links against (recursively) into
# Contents/Frameworks and rewrites install names to @rpath, so the app
# runs on machines without Homebrew/SDL installed.
set -euo pipefail
cd "$(dirname "$0")/.."

APP_NAME="snake_push_block"
BIN=snake_push_block
PROJECT_DIR=$PWD
# Assemble + sign in a temp dir: iCloud's fileproviderd stamps files under
# Desktop/Documents with com.apple.FinderInfo faster than we can strip it,
# which makes codesign fail with "detritus not allowed".
STAGE=$(mktemp -d /tmp/snake_push_block_bundle.XXXXXX)
trap 'rm -rf "$STAGE"' EXIT
APP="$STAGE/$APP_NAME.app"
MACOS="$APP/Contents/MacOS"
RESOURCES="$APP/Contents/Resources"
FRAMEWORKS="$APP/Contents/Frameworks"

[ -f "$BIN" ] || { echo "error: build '$BIN' first (run make)"; exit 1; }

rm -rf "$APP"
mkdir -p "$MACOS" "$RESOURCES" "$FRAMEWORKS"

cp "$BIN" "$MACOS/$BIN"
cp packaging/Info.plist "$APP/Contents/Info.plist"
cp -R assets "$RESOURCES/assets"
find "$RESOURCES" -name .DS_Store -delete

# --- icon: pad title.png to a square, render all iconset sizes ---
ICONSET="$STAGE/icon.iconset"
SQUARE="$STAGE/icon_square.png"
mkdir -p "$ICONSET"
sips assets/title.png --padToHeightWidth 1280 1280 --padColor 000000 \
    --out "$SQUARE" >/dev/null
for size in 16 32 128 256 512; do
    sips -z $size $size "$SQUARE" --out "$ICONSET/icon_${size}x${size}.png" >/dev/null
    sips -z $((size * 2)) $((size * 2)) "$SQUARE" \
        --out "$ICONSET/icon_${size}x${size}@2x.png" >/dev/null
done
iconutil -c icns "$ICONSET" -o "$RESOURCES/icon.icns"
rm -rf "$ICONSET" "$SQUARE"

# --- bundle dylibs recursively ---
# Homebrew dylibs reference each other both by absolute /opt/homebrew paths
# and by @rpath/@loader_path (e.g. libjxl -> @rpath/libjxl_cms), so resolve
# both forms: relative ones against the source lib's own dir, then the
# Homebrew link farm.
copy_deps() {
    local target="$1" srcdir="$2"
    local dep name src
    otool -L "$target" | awk 'NR > 1 {print $1}' |
        while read -r dep; do
            name=$(basename "$dep")
            case "$dep" in
            /opt/homebrew/*) src="$dep" ;;
            @rpath/* | @loader_path/*)
                if [ -f "$srcdir/$name" ]; then
                    src="$srcdir/$name"
                elif [ -f "/opt/homebrew/lib/$name" ]; then
                    src="/opt/homebrew/lib/$name"
                else
                    continue # self-id line or already-bundled name
                fi
                ;;
            *) continue ;; # system lib
            esac
            if [ ! -f "$FRAMEWORKS/$name" ]; then
                cp "$src" "$FRAMEWORKS/$name" # cp follows symlinks
                chmod u+w "$FRAMEWORKS/$name"
                install_name_tool -id "@rpath/$name" "$FRAMEWORKS/$name" 2>/dev/null
                copy_deps "$FRAMEWORKS/$name" "$(dirname "$(realpath "$src")")"
            fi
            install_name_tool -change "$dep" "@rpath/$name" "$target" 2>/dev/null
        done
}
copy_deps "$MACOS/$BIN" "/opt/homebrew/lib"
install_name_tool -add_rpath "@executable_path/../Frameworks" "$MACOS/$BIN" 2>/dev/null

# self-check: every @rpath dep of every bundled binary must exist in Frameworks
missing=0
for f in "$MACOS/$BIN" "$FRAMEWORKS"/*.dylib; do
    while read -r dep; do
        name=$(basename "$dep")
        if [ ! -f "$FRAMEWORKS/$name" ]; then
            echo "error: unresolved dependency $dep (from $(basename "$f"))" >&2
            missing=1
        fi
    done < <(otool -L "$f" | awk 'NR > 1 && ($1 ~ /^@/ || $1 ~ /^\/opt\//) {print $1}')
done
[ "$missing" -eq 0 ] || exit 1

xattr -cr "$APP"

# install_name_tool invalidates signatures; re-sign everything ad hoc
codesign --force --sign - "$FRAMEWORKS"/*.dylib
codesign --force --sign - "$APP"
codesign --verify --deep --strict "$APP"

# zip from the clean staging copy, then place the app in build/
mkdir -p "$PROJECT_DIR/build"
rm -rf "$PROJECT_DIR/build/$APP_NAME.app"
ZIP="$PROJECT_DIR/build/${APP_NAME}-${VERSION:-1.0.0}-macos-arm64.zip"
rm -f "$ZIP"
ditto -c -k --keepParent --norsrc --noextattr --noqtn "$APP" "$ZIP"
ditto --norsrc --noextattr --noqtn "$APP" "$PROJECT_DIR/build/$APP_NAME.app"

echo "bundled $(ls "$FRAMEWORKS" | wc -l | tr -d ' ') dylibs"
echo "created build/$APP_NAME.app"
echo "created $ZIP"
