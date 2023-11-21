#pragma once
#include <cstdint> // uint16_t, uint64_t
typedef std::uint16_t   welltype; // Basically the same as an unsigned int (max size is 65535)
typedef std::uint16_t   stattype;
typedef std::uint64_t   exptype;
typedef std::uint16_t   leveltype;

#define MAX_WELLTYPE    UINT16_MAX
#define MAX_STATTYPE    UINT16_MAX
#define MAX_EXPTYPE     UINT64_MAX
#define MAX_LEVELTYPE   UINT16_MAX
