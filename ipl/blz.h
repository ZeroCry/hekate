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

//blz decompression algorithm from nxo64.py
//TODO: replace this with reverse engineered version from SecMon
unsigned char* kip1_blz_decompress(const unsigned char* compData, unsigned int compDataLen, int* decompLenPtr);

#endif