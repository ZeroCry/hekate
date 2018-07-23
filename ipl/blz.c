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

#include <stdlib.h>

//this function is like 3x faster with -O2 than -Os
unsigned char* kip1_blz_decompress(const unsigned char* compData, unsigned int compDataLen, int* decompLenPtr)
{
	unsigned int compressed_size, init_index, uncompressed_addl_size;
	{
		struct BlzFooter
		{
			unsigned int compressed_size;
			unsigned int init_index;
			unsigned int uncompressed_addl_size;
		} footer;

		if (compDataLen < sizeof(footer))
		{
			*decompLenPtr = compDataLen - sizeof(footer);
			return NULL;
		}

		memcpy(&footer, &compData[compDataLen-sizeof(footer)], sizeof(footer));
		compressed_size = footer.compressed_size;
		init_index = footer.init_index;
		uncompressed_addl_size = footer.uncompressed_addl_size;
	}

	unsigned char* compressedAlloc = malloc(compDataLen);
	unsigned char* compressedPtr = compressedAlloc;
	if (compDataLen > 0)
		memcpy(compressedPtr, compData, compDataLen);

	unsigned int decompressed_size = compDataLen + uncompressed_addl_size;
	unsigned char* decompressedPtr = malloc(decompressed_size);
	if (compDataLen > 0)
	{
		memcpy(decompressedPtr, compData, compDataLen);
		unsigned int remainingBytes = decompressed_size - compDataLen;
		if (remainingBytes > 0)
			memset(&decompressedPtr[compDataLen], 0, remainingBytes);
	}
	else if (decompressed_size > 0)
		memset(decompressedPtr, 0, decompressed_size);

	if (compDataLen != compressed_size)
	{
		if (compDataLen < compressed_size)
		{
			*decompLenPtr = compDataLen - compressed_size;
			free(decompressedPtr);
			free(compressedAlloc);
			return NULL;
		}

		unsigned int numSkipBytes = compDataLen - compressed_size;
		compressedPtr = &compressedPtr[numSkipBytes];
	}
	if ((compressed_size + uncompressed_addl_size) == 0)
	{
		*decompLenPtr = 0;
		free(decompressedPtr);
		free(compressedAlloc);
		return NULL;
	}

	unsigned int index = compressed_size - init_index;
	unsigned int outindex = decompressed_size;
    while (outindex > 0)
	{
		index -= 1;
		unsigned char control = compressedPtr[index];
		for (unsigned int i=0; i<8; i++)
		{
            if ((control & 0x80) != 0)
			{
				if (index < 2)
				{
					*decompLenPtr = -1001;
					free(decompressedPtr);
					free(compressedAlloc);
					return NULL;
				}

				index -= 2;
				unsigned int segmentoffset = (unsigned int)(compressedPtr[index]) | ((unsigned int)(compressedPtr[index+1]) << 8);
				unsigned int segmentsize = ((segmentoffset >> 12) & 0xF) + 3;
				segmentoffset &= 0x0FFF;
				segmentoffset += 2;
				if (outindex < segmentsize)
				{
					*decompLenPtr = -1002;
					free(decompressedPtr);
					free(compressedAlloc);
					return NULL;
				}

                for (unsigned int j=0; j<segmentsize; j++)
				{
					if ((outindex + segmentoffset) >= decompressed_size)
					{
						*decompLenPtr = -1003;
						free(decompressedPtr);
						free(compressedAlloc);
						return NULL;
					}

					unsigned char data = decompressedPtr[outindex+segmentoffset];
					outindex -= 1;
					decompressedPtr[outindex] = data;
				}
			}
            else
			{
				if (outindex < 1)
				{
					*decompLenPtr = -1004;
					free(decompressedPtr);
					free(compressedAlloc);
					return NULL;
				}

				outindex -= 1;
				index -= 1;
				decompressedPtr[outindex] = compressedPtr[index];
			}

			control <<= 1;
			control &= 0xFF;
			if (outindex == 0)
				break;
		}
	}

	*decompLenPtr = decompressed_size;
	free(compressedAlloc);
	return decompressedPtr;
}