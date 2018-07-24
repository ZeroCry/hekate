/*
* Copyright (c) 2018 rajkosto
*
* This program is free software; you can redistribute it and/or modify it
* under the terms and conditions of the GNU General Public License,
* version 2, as published by the Free Software Foundation.
*
* This program is distributed in the hope it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _BLZ_H_
#define _BLZ_H_

#include "types.h"

typedef struct _kip1_blz_footer
{
    u32 cmp_and_hdr_size;
    u32 header_size;
    u32 addl_size;
} kip1_blz_footer;

//returns pointer to footer in compData if present, additionally copies it to outFooter if not NULL
const kip1_blz_footer* kip1_blz_get_footer(const unsigned char* compData, unsigned int compDataLen, kip1_blz_footer* outFooter);
//returns 0 on failure
int kip1_blz_uncompress(unsigned char* dataBuf, unsigned int compSize, const kip1_blz_footer* footer);

#endif