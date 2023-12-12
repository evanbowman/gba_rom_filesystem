import os
import sys

with open(sys.argv[1], 'rb') as f:
    s = f.read()

fs_loc = s.find(b'_FS_')
fs_loc += 4 # skip filesystem string

output_rom = sys.argv[1].split('.')[0] + ".extracted.gba"

with open(output_rom, "wb") as f:
    print("Extracting engine...")
    f.write(s[:fs_loc-4])
    print("Wrote " + output_rom + "!")

with open("fs.extracted.bin", "wb") as f:
    print("Extracting filesystem...")
    data = s[fs_loc:]
    f.write(data)

    pos = 0

    file_count = int.from_bytes(data[0:4], byteorder='little')
    print("Extracting {} files...".format(file_count))

    pos += 4

    for f in range(0, file_count):
        path = data[pos:pos+62]
        path = path.strip(b'\0')
        path = path.decode("utf-8")[1:] # Ignore leading slash
        pos += 64 # (path field size)
        print("  Extracting {}...".format(path))

        file_size = int.from_bytes(data[pos:pos+4], byteorder='little')
        pos += 4

        dirname = os.path.dirname(path)

        if dirname == '':
            pass
        elif not os.path.exists(dirname):
            os.makedirs(dirname)

        with open(path, "wb") as f:
            contents = data[pos:pos+file_size-1] # -1 for null terminator
            if path.split('.')[-1] == "lisp":
                contents = unminify_lisp(contents.decode("utf-8")).encode('utf-8')
            f.write(contents)

        pos += file_size

    print("Wrote files!")

# end of unpack_rom.py
