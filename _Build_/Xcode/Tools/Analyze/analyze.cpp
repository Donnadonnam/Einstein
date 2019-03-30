// ==============================
// File:			analyze.cpp
// Project:			Einstein
//
// Copyright 2019 by Matthais Melcher (proteus@matthiasm.com).
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

/*
 The "analyze" tool is meant to read and interprete the NetonOS debugging image
 that came with Apple Driver Development Kit. The image is available in the .aif
 file format and contains a fully working ROM and REx, and a large number of
 debugging symbols. Many symbols are in C and C++ format, providing basic
 insights into the inner workings of the ROM.

 Usage: analyze --proteus proteusfile --aif path_to_717006.aif

 Xcode: when using Xcode, put your copy of 717006.aif into the _Data_ directory.

 "Analyze" will go through a list of tasks to understand as much as possible of
 the given image and create a map of its knowledge. It then merges its finding
 into the "proteus.cpp" file.

 The "proteus.cpp" file is a monolithic handwritten C++ code file that can
 also contain a readable copy of the .aif image, created by "analyze".

 Task (- = implemented, * = still to do):
  * verify aif
  * read the ROM binary data
  * read the symbol table and demangles C++ symbols
  * (analyze known header files)
  * static analysis of ARM code, following known entry points
  * static analysis of ROM Soup (Newton Script object tree)
  * analyze ROM Extension

  * read proteus file line by line
  * expand lines that start with "// /+ 0x00000000-0x11111111 +/" and
    add the findings from the analysis
  * hand-written code is never changed
  * save squashed file, replacing the original proteus file

 */

// Topic, Issue, Knowledge, Effect, Conclusion, Finding, Discovery, Clue
// gROMMagicPointerTable
// gROMSoupData
// gROMSoupDataSize

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <vector>

std::vector<const char*> proteusFileList;

int readArgs(int argc, const char * argv[])
{
	for (int i=1; i<argc; i++) {
		const char *arg = argv[i];
		if (arg==nullptr || arg[0]==0) {
			fprintf(stderr, "Error: invalid parameter %d\n", i);
			return 30;
		}
		// everything that is not a recognized flag is saved a a path to a proteus flag
		if (::access(arg, R_OK)!=0) {
			fprintf(stderr, "Error: can't read file \"%s\": %s\n", arg, strerror(errno));
			return 30;
		}
		proteusFileList.push_back(argv[i]);
	}
	return 0;
}


int writeSeg(FILE *out, uint32_t startSeg, uint32_t endSeg)
{
	// TODO: loop through all clues in the given range and write the clue to 'out'
	::fprintf(out, "// /* 0x%08X-0x%08X */\n", startSeg, endSeg);

	return 0;
}


int replenish(const char *filename)
{
	int ret = 0;

	FILE *f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "Error: can't open file \"%s\" for reading: %s\n", filename, strerror(errno));
		return 30;
	}
	char targetfilename[2048];
	strcpy(targetfilename, filename);
	strcat(targetfilename, ".sq");
	FILE *out = fopen(targetfilename, "wb");
	if (!out) {
		fprintf(stderr, "Error: can't open file \"%s\" for writing: %s\n", targetfilename, strerror(errno));
		fclose(f);
		return 30;
	}

	char line[2049];
	uint32_t startLine, endLine, startSeg = 0, endSeg = 0;
	bool segPending = false;
	for (;;) {
		if (::feof(f)) break;
		char *fret = ::fgets(line, 2048, f);
		if (fret==nullptr) break; // we could handle eof and errors separately here
		int n = ::sscanf(line, "// /* 0x%08X-0x%08X ", &startLine, &endLine);
		if (n==1) { endLine = startLine+4; n = 2; }
		if (n==2) {
			if (segPending) {
				endSeg = endLine;
			} else {
				startSeg = startLine;
				endSeg = endLine;
				segPending = true;
			}
		} else {
			if (segPending) {
				writeSeg(out, startSeg, endSeg);
				segPending = false;
			}
			::fputs(line, out);
		}
	}
	if (segPending) {
		writeSeg(out, startSeg, endSeg);
	}
	fclose(f);
	fclose(out);

	ret = ::remove(filename);
	if (ret!=0) {
		fprintf(stderr, "Error: can't remove file \"%s\": %s\n", filename, strerror(errno));
		return 30;
	}

	ret = ::rename(targetfilename, filename);
	if (ret!=0) {
		fprintf(stderr, "Error: can't rename file \"%s\" to \"%s\": %s\n", targetfilename, filename, strerror(errno));
		return 30;
	}

	return ret;
}


int main(int argc, const char * argv[])
{
	int ret = 0;
	ret = readArgs(argc, argv);
	if (ret!=0) {
		fprintf(stderr, "Usage: analyze "
				"[--aif path_to_ARM_image_file.aif] "
				"[path_to_proteus_file.cpp ...]\n");
		return ret;
	}
	for (auto &filename: proteusFileList) {
		ret = replenish(filename);
		if (ret!=0) {
			fprintf(stderr, "Aborting: can't replenish file \"%s\"\n", filename);
			return ret;
		}
	}
	return ret;
}


// ============================================================================== //
// I've ended up calling my cat Brexit. It wakes me up meowing like crazy every
// morning because it wants to go out, but as soon as I open the door, it just
// sits there undecided and then looks angry when I put it outside.
//   -- French Europe Minister Nathalie Loiseau
// ============================================================================== //
