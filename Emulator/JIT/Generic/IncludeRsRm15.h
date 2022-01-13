// ==============================
// File:			IncludeRsRm15.h
// Project:			Einstein
//
// Copyright 2003-2007 by Paul Guyot (pguyot@kallisys.net).
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// ==============================
// $Id$
// ==============================

// Repeatly include a file, setting Rs and Rm from 0 to 14.
// Usage:
// #define INCLUDE_TEMPLATE	"MyFile.h"
// #include "IncludeRsRm15.h"

#define Rs 0
#include "IncludeRm15.h"
#undef Rs
#define Rs 1
#include "IncludeRm15.h"
#undef Rs
#define Rs 2
#include "IncludeRm15.h"
#undef Rs
#define Rs 3
#include "IncludeRm15.h"
#undef Rs
#define Rs 4
#include "IncludeRm15.h"
#undef Rs
#define Rs 5
#include "IncludeRm15.h"
#undef Rs
#define Rs 6
#include "IncludeRm15.h"
#undef Rs
#define Rs 7
#include "IncludeRm15.h"
#undef Rs
#define Rs 8
#include "IncludeRm15.h"
#undef Rs
#define Rs 9
#include "IncludeRm15.h"
#undef Rs
#define Rs 10
#include "IncludeRm15.h"
#undef Rs
#define Rs 11
#include "IncludeRm15.h"
#undef Rs
#define Rs 12
#include "IncludeRm15.h"
#undef Rs
#define Rs 13
#include "IncludeRm15.h"
#undef Rs
#define Rs 14
#include "IncludeRm15.h"
#undef Rs

// =================================================== //
// HELP!!!! I'm being held prisoner in /usr/games/lib! //
// =================================================== //
