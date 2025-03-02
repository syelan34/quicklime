COMPILE_COMMANDS="$(pwd)/compile_commands.json"
[ -f "$COMPILE_COMMANDS" ] && rm "$COMPILE_COMMANDS"

MAKE="make"

# VERBOSE=1 make --always-make --dry-run | grep -wE 'arm-none-eabi-g\+\+' | grep -v 'true add' | grep -w '\-c' | jq -nR '[inputs|{directory:"'$(pwd)'", command:., file:(match("(?<=[^-]-c )(?:[^ \\\\]|\\\\.)*").string), output: match("(?<=-o )(?:[^ \\\\]|\\\\.)*").string}]' >> "$COMPILE_COMMANDS"

# VERBOSE=1 compiledb -n make --always-make --dry-run # don't actually run the build yet, just generate the compile_commands.json file

[ -f /proc/cpuinfo ] && MAKE="make -j$(grep -c ^processor /proc/cpuinfo)" || echo "No /proc/cpuinfo found, using single-threaded build" && $MAKE

cp build/compile_commands.json "$COMPILE_COMMANDS" || echo "Could not find compile_commands.json file"
cp build/compile_commands.part "$COMPILE_COMMANDS" && echo "]" >> "$COMPILE_COMMANDS" || echo "Could not find compile_commands.part file"

#sed -z 's/\n\]\n\[/,/g' -i "$COMPILE_COMMANDS"

LIBDIR="/lib/libquicklime"
sudo [ -d "$LIBDIR" ] && sudo rm -r "$LIBDIR" && sudo mkdir "$LIBDIR" || sudo mkdir "$LIBDIR"
sudo cp -r $(pwd)/include "$LIBDIR"
sudo cp -r $(pwd)/lib "$LIBDIR"
