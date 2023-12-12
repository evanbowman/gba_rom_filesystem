# What is this?

A simple filesystem library for C++. It appends an archive to a gba rom, and provides utilities for reading the files within the gba program.

# Are there any examples?

See example.cpp

# How do I build it?

Run build.sh. The build.sh script runs encode_files.py, and then uses the unix `cat` tool to append the resulting filesystem archive to the compiled gba code.

# Is there a C interface?

Not yet.

# Are there any drawbacks to using this code?

1) It's written in C++
2) The implementation finds files with linear search. But you can cache the results of previous file lookups.

# How do I unpack files from an existing archive?

Run `python unpack_rom.py <rom_with_appended_archive>`

# Why are there null bytes at the end of my files when running the rom unpacking tool?

The implementation pads files to the GBA cpu alignment size for faster access to fields in the file headers.
