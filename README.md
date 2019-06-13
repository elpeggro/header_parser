# Build
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

# Use
```
./header_parser <video> <csv-file>
```

# Debug Output
By default, nothing is printed to stdout. If you want to get basic
information, define the `INFO` flag in defines.h. For detailed debug
output, define the `DEBUG` flag in defines.h