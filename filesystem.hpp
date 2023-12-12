////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2023  Evan Bowman.
//
// The source code in this file is public domain.
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "function.hpp"
#include "endian.hpp"



namespace filesystem
{



struct Root
{
    char magic_[4];
    host_u32 file_count_;
};



struct FileHeader
{
    char path_[62]; // Must be null-terminated.
    host_u16 flags_;
    host_u32 size_;
};



using NullTerminatedString = const char*;
using FileContents = NullTerminatedString;
using FilePath = NullTerminatedString;
using FileSize = uint32_t;



bool is_mounted();



std::pair<FileContents, FileSize> load(FilePath path);


void walk(Function<8 * sizeof(void*), void(const char* path)> callback);



uint32_t size();



} // namespace filesystem
