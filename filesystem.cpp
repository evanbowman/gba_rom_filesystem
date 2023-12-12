////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2023  Evan Bowman.
//
// The source code in this file is public domain.
//
////////////////////////////////////////////////////////////////////////////////


#pragma GCC diagnostic push
// Accessing data past __rom_end__ raises -Warray-bounds errors. These would be
// real errors, except that they aren't problematic on the gameboy advance, for
// various reasons, and we're mounting a filesystem at __rom_end__ anyway, so...
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wstringop-overread"


#include "filesystem.hpp"



// Symbol declared by the linker script, points to the end of the ROM, where we
// will mount the files.
extern char __rom_end__;



namespace filesystem
{



inline bool str_eq(const char* p1, const char* p2)
{
    while (true) {
        if (*p1 not_eq *p2) {
            return false;
        }
        if (*p1 == '\0' or *p2 == '\0') {
            return true;
        }
        ++p1;
        ++p2;
    }
}



Root* get_root()
{
    auto root = (Root*)&__rom_end__;
    if (not(root->magic_[0] == '_' and root->magic_[1] == 'F' and
            root->magic_[2] == 'S' and root->magic_[3] == '_')) {
        // The filesystem root must begin with the characters "_FS_". This is
        // how we detect whether there's a filesystem attached to the ROM.
        return nullptr;
    }
    return root;
}



uint32_t size()
{
    if (not is_mounted()) {
        return 0;
    }

    uint32_t size = sizeof(Root);

    const char* current = &__rom_end__;
    current += sizeof(Root);

    const auto root = get_root();

    uint32_t files_remaining = root->file_count_.get();


    while (files_remaining) {
        auto hdr = (FileHeader*)current;

        size += sizeof(FileHeader) + hdr->size_.get();

        --files_remaining;
        current += sizeof(FileHeader) + hdr->size_.get();
    }

    return size;
}



bool is_mounted()
{
    return get_root() not_eq nullptr;
}



void walk(Function<8 * sizeof(void*), void(const char* path)> callback)
{
    const char* current = &__rom_end__;
    current += sizeof(Root);

    const auto root = get_root();

    uint32_t files_remaining = root->file_count_.get();


    while (files_remaining) {
        auto hdr = (FileHeader*)current;

        callback(hdr->path_);

        --files_remaining;
        current += sizeof(FileHeader) + hdr->size_.get();
    }
}



std::pair<FileContents, FileSize> load(FilePath path)
{
    const char* current = &__rom_end__;
    current += sizeof(Root);

    const auto root = get_root();

    uint32_t files_remaining = root->file_count_.get();


    while (files_remaining) {
        auto hdr = (FileHeader*)current;

        if (str_eq(hdr->path_, path)) {
            if ((uint32_t)(intptr_t)(current + sizeof(FileHeader)) % 4 not_eq 0) {
                // ERROR: unaligned file!
                return {nullptr, 0};
            }
            return {current + sizeof(FileHeader), hdr->size_.get()};
        }

        --files_remaining;
        current += sizeof(FileHeader) + hdr->size_.get();
    }

    return {nullptr, 0};
}


} // namespace filesystem
#pragma GCC diagnostic pop
