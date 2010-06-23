/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2009 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/HPSF.cpp,v $
 * $Revision: 1.3 $
 * $Author: dhoerl $
 * $Date: 2009/03/02 04:08:43 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlsys.h>

#include "HPSF.h"

using namespace xlslib_core;

const unsigned32_t xlslib_core::summaryFormat[] = {
	0xf29f85e0,
	0x10684ff9,
	0x000891ab,
	0xd9b3272b
};
const unsigned32_t xlslib_core::docSummaryFormat[] = {
	0xd5cdd502,
	0x101b2e9c,
	0x00089793,
	0xaef92c2b
};
const unsigned32_t xlslib_core::hpsfValues[] = {
	30,		// HPSF_STRING,
	11,		// HPSF_BOOL,
	2,		// HPSF_INT16,
	3,		// HPSF_INT32,
	64,		// HPSF_INT64
};

HPSFitem::HPSFitem(unsigned16_t v, const std::string& str) :
	propID(v),
	variant(HPSF_STRING),
	value(),
	offset(0)
{
	value.str = new std::string(str);
}
HPSFitem::HPSFitem(unsigned16_t v, bool val) :
	propID(v),
	variant(HPSF_BOOL),
	value(),
	offset(0)
{
	value.isOn = val;
}
HPSFitem::HPSFitem(unsigned16_t v, unsigned16_t val) :
	propID(v),
	variant(HPSF_INT16),
	value(),
	offset(0)
{
	value.val16 = val;
}
HPSFitem::HPSFitem(unsigned16_t v, unsigned32_t val) :
	propID(v),
	variant(HPSF_INT32),
	value(),
	offset(0)
{
	value.val32 = val;
}
HPSFitem::HPSFitem(unsigned16_t v, unsigned64_t val) :
	propID(v),
	variant(HPSF_INT64),
	value(),
	offset(0)
{
	value.val64 = val;
}

HPSFitem::~HPSFitem()
{
	if(variant == HPSF_STRING)
		delete value.str;
}

size_t HPSFitem::GetSize()
{
	size_t size;
	
	switch(variant) 
	{
	case HPSF_STRING:
		size = value.str->length() + 1 + 4;         // 1 for null terminator, 4 for length field
		// round up to the next 4-byte boundary:
		size = (size + 4 - 1) & ~3;
		XL_ASSERT(size >= 4);
		XL_ASSERT((size % 4) == 0);
		break;
	case HPSF_BOOL:
		size = 2 + 2; // 2 + 2 padding
		break;
	case HPSF_INT16:
		size = 2 + 2; // 2 + 2 padding
		break;
	case HPSF_INT32:
		size = 4; // 0 padding
		break;
	case HPSF_INT64:
		size = 8; // 0 padding
		break;
	default:
		size = 0; // 0 padding
		break;
	}
	
	return size + 4;	// variant at the start
}

HPSFdoc::HPSFdoc(CDataStorage &datastore, docType_t dt) :
	docType(dt),
	CUnit(datastore)
{

}

HPSFdoc::~HPSFdoc()
{
	HPSF_Set_Itor_t		hBegin, hEnd, hIter;
	
	hBegin		= itemList.begin();
	hEnd		= itemList.end();
	for(hIter=hBegin; hIter != hEnd; ++hIter)
		delete *hIter;
}

void HPSFdoc::insert(HPSFitem *item)
{
	HPSFitem*		existingItem;
	bool			success;

	do 
	{
		std::pair<HPSF_Set_Itor_t, bool> ret = itemList.insert(item);
		success = ret.second;
		
		if(!success) 
		{
			existingItem = *(ret.first);
			delete existingItem;
			itemList.erase(existingItem);
		}
	} while(!success);
}

unsigned64_t HPSFdoc::unix2mstime(time_t unixTime)
{
	unsigned64_t	msTime;

	msTime	 = unixTime * (unsigned64_t)1000000 + FILETIME2UNIX_NS;
	msTime	*= (unsigned64_t)10;
	
	return msTime;
}

//
// http://poi.apache.org/hpsf/internals.html
// or google "DocumentSummaryInformation and UserDefined Property Sets" and look for MSDN hits
//
void HPSFdoc::DumpData(void)
{
	HPSF_Set_Itor_t		hBegin, hEnd, hIter;
	const unsigned32_t	*fmt;
	size_t sectionListOffset;
	size_t numProperties;
	size_t itemOffset;
	
	numProperties = itemList.size();	
	fmt = (docType == HPSF_SUMMARY ? summaryFormat : docSummaryFormat);

	Inflate(SUMMARY_SIZE);	// this file will only be this size, ever (zero padded)

	// Header
	AddValue16(0xfffe);	// signature
	AddValue16(0);
#ifdef __OSX__
	AddValue32(1);		// Macintosh
#else
	AddValue32(2);		// WIN32
#endif
	AddValue32(0); AddValue32(0); AddValue32(0); AddValue32(0);		// CLASS
	AddValue32(1);		// One section

	// The section (this is a list but in this case just 1 section so can shorten logic)
	AddValue32(fmt[0]);	// Class ID
	AddValue32(fmt[1]);
	AddValue32(fmt[2]);
	AddValue32(fmt[3]);

	// offset to the data (would vary if multiple sections, but since one only its easy
	sectionListOffset = GetDataSize() + 4;		// offset from the start of this stream to first byte after this offset     [i_a]
	AddValue32((unsigned32_t)sectionListOffset);				// where this section starts (right after this tag!)
	
	// Start of Section 1: sectionListOffset starts here
	AddValue32(0);								// length of the section - updated later
	AddValue32((unsigned32_t)numProperties);					// 

	// now write the propertyLists - the values, and where to find the payloads
	itemOffset	= 8 + numProperties * 8;	// where I am now, then allow for propertyLists
	hBegin		= itemList.begin();
	hEnd		= itemList.end();
	for(hIter=hBegin; hIter != hEnd; ++hIter) 
	{
		HPSFitem	*item = *hIter;
		
		item->SetOffset(itemOffset);

		AddValue32(item->GetPropID());			// Variant (ie type)
		AddValue32((unsigned32_t)itemOffset);	// where the actual data will be found
		
		itemOffset += item->GetSize();
	}
	SetValueAt32((unsigned32_t)itemOffset, (unsigned32_t)sectionListOffset);
	//printf("Think size is %d\n", (int)itemOffset);

	// Now we can write out the actual data
	hBegin		= itemList.begin();
	hEnd		= itemList.end();
	for(hIter=hBegin; hIter != hEnd; ++hIter) 
	{
		HPSFitem		*item = *hIter;
		size_t	len;
		size_t padding;
		unsigned16_t variant;
		hValue			value;
		
		value	= item->GetValue();
		variant	= item->GetVariant();
		//printf("PROPERTY[%d]: ActualOffset=%d savedOffset=%d variant=%d realVariant=%d val64=%llx\n", item->GetPropID(), 
		//  m_nDataSize - sectionListOffset, item->GetOffset(), variant,  hpsfValues[variant], value.val64);
		AddValue32(hpsfValues[variant]);
		
		switch(variant) 
		{
		case HPSF_STRING:
			len = value.str->length() + 1;	// length of string plus null terminator
			// padding = (len % 4) + 1;		// string terminator is the "1"
			// round up to the next 4-byte boundary to determine the padding; 
			// take the mandatory NUL sentinel into account as well:
			padding = 1 + ((4 - len) & 3);
			XL_ASSERT(padding + len - 1 >= 4);
			XL_ASSERT((padding + len - 1) % 4 == 0);
			AddValue32((unsigned32_t)len);
			AddDataArray((const unsigned8_t *)value.str->c_str(), len-1);
			break;
		case HPSF_BOOL:
			padding = 2;
			AddValue16(value.isOn ? 0xFFFF : 0x0000);	// per MSDN google of VT_BOOL
			break;
		case HPSF_INT16:
			padding = 2;
			AddValue16(value.val16);
			break;
		case HPSF_INT32:
			padding = 0;
			AddValue32(value.val32);
			break;
		case HPSF_INT64:
			padding = 0;
			AddValue64(value.val64);
			break;
		default:
			padding = 0;
			break;
		}
		AddFixedDataArray(0, padding);	
	}

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
#endif

	//printf("Actual size = %d\n", m_nDataSize - sectionListOffset);
	XL_ASSERT(GetDataSize() <= GetSize());
	XL_ASSERT(GetDataSize() <= SUMMARY_SIZE);
	AddFixedDataArray(0, SUMMARY_SIZE - GetDataSize());
	XL_ASSERT(GetDataSize() <= GetSize());
}
