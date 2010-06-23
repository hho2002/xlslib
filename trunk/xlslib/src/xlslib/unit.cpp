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
 * Copyright 2004 Yeico S. A. de C. V.
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/unit.cpp,v $
 * $Revision: 1.8 $
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

#include <unit.h>
#include <globalrec.h>
#include <rectypes.h>
#include <datast.h>

using namespace xlslib_core;

/* 
*********************************************************************************
*********************************************************************************
CUnit class implementation
*********************************************************************************
*********************************************************************************
*/

// Default constructor
CUnit::CUnit(CDataStorage &datastore) :
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	m_Index(INVALID_STORE_INDEX),
	m_Store(datastore),
	m_Backpatching_Level(0)
#else
	m_nSize(0),
	m_nDataSize(0),
	m_pData(NULL)
#endif
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	datastore.Push(this);
#else
#endif
}

	CUnit::CUnit(const CUnit& orig) :
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	m_Index(INVALID_STORE_INDEX),
	m_Store(orig.m_Store),
	m_Backpatching_Level(0)
#else
	m_nSize(orig.m_nSize),
	m_nDataSize(orig.m_nDataSize),
	m_pData(orig.m_pData ? (unsigned8_t *)malloc(m_nSize) : NULL)
#endif
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	XL_ASSERT(m_Index == INVALID_STORE_INDEX);
	if (orig.m_Index != INVALID_STORE_INDEX)
	{
		m_Index = m_Store.RequestIndex(orig.GetDataSize());
		memcpy(m_Store[m_Index].GetBuffer(), orig.GetBuffer(), orig.GetDataSize());
	}
#else
	if(m_pData) {
		memcpy(m_pData, orig.m_pData, m_nSize);
	}
#endif
}

CUnit& CUnit::operator=(const CUnit& right)
{
	if(this == &right) {
		return *this;
	}

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

	size_t len = right.GetDataSize();
	if (m_Index == INVALID_STORE_INDEX && right.m_Index != INVALID_STORE_INDEX)
	{
		m_Index = m_Store.RequestIndex(len);
	}
	else if (right.m_Index != INVALID_STORE_INDEX)
	{
		m_Store[m_Index].Resize(len);
	}
	XL_ASSERT(right.m_Index != INVALID_STORE_INDEX ? m_Index != INVALID_STORE_INDEX : 1);
	if (right.m_Index != INVALID_STORE_INDEX)
	{
		memcpy(m_Store[m_Index].GetBuffer(), right.GetBuffer(), len);
		m_Store[m_Index].SetDataSize(len);
	}

#else

	m_nSize			= right.m_nSize;
	m_nDataSize		= right.m_nDataSize;
	if(right.m_pData) {
		m_pData = (unsigned8_t *)malloc(m_nSize);
		memcpy(m_pData, right.m_pData, m_nSize);
	} else {
		m_pData = NULL;
	}

#endif

	return *this;
}

// Default destructor
CUnit::~CUnit()
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	if (m_Index != INVALID_STORE_INDEX)
	{
		XL_ASSERT(m_Index >= 0 ? !m_Store[m_Index].IsSticky() : 1);
		XL_ASSERT(m_Index < 0 ? m_Store[m_Index].IsSticky() : 1);
		if (m_Index >= 0)
		{
			m_Store[m_Index].Reset();
		}
	}
#else
   if(m_pData /*&& !m_ShadowUnit*/)
   {
      delete[] m_pData;
   }
#endif
}

/************************************************
 ************************************************/

const size_t CUnit::DefaultInflateSize = 10;

/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt8(unsigned8_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   if(m_pData != NULL)
   {
      if (index < m_nDataSize)
         m_pData[index] = newval;
      else
         errcode = ERR_INVALID_INDEX;
   } else {
      errcode = ERR_DATASTORAGE_EMPTY;
   }
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue16(unsigned16_t newval)
{
   signed8_t errcode = NO_ERRORS;

   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue32(unsigned32_t newval)
{
   signed8_t errcode = NO_ERRORS;
   
   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_2(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_3(newval))) errcode = GENERAL_ERROR;
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue64(unsigned64_t newval)
{
   signed8_t errcode = NO_ERRORS;

   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_2(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_3(newval))) errcode = GENERAL_ERROR;

   if(AddValue8(BYTE_4(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_5(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_6(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_7(newval))) errcode = GENERAL_ERROR;

   return errcode;
}

/************************************************
************************************************/

signed8_t CUnit::AddValue64FP(double newval)
{
	signed8_t errcode = NO_ERRORS;

#include <xls_pshpack1.h>

	union 
	{
		double f;
		unsigned64_t i;
		unsigned8_t b[8];
	} v;
	
#include <xls_poppack.h>

	v.f = newval;

	if(AddValue8(BYTE_0(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_1(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_2(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_3(v.i))) errcode = GENERAL_ERROR;

	if(AddValue8(BYTE_4(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_5(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_6(v.i))) errcode = GENERAL_ERROR;
	if(AddValue8(BYTE_7(v.i))) errcode = GENERAL_ERROR;

	return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt16(unsigned16_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

   if(SetValueAt8(BYTE_0(newval), index  )) errcode = GENERAL_ERROR;
   if(SetValueAt8(BYTE_1(newval), index+1)) errcode = GENERAL_ERROR;
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt32(unsigned32_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

   if(SetValueAt8(BYTE_0(newval), index  )) errcode = GENERAL_ERROR;
   if(SetValueAt8(BYTE_1(newval), index+1)) errcode = GENERAL_ERROR;
   if(SetValueAt8(BYTE_2(newval), index+2)) errcode = GENERAL_ERROR;
   if(SetValueAt8(BYTE_3(newval), index+3)) errcode = GENERAL_ERROR;

   return errcode;
}


/************************************************
 ************************************************/

signed8_t CUnit::GetValue16From(signed16_t* val, unsigned32_t index) const
{
   signed8_t errcode = NO_ERRORS;

   *val = (signed16_t)(operator[](index) + 
                       operator[](index+1)*0x0100);

   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::GetValue32From(signed32_t* val, unsigned32_t index) const
{
   signed8_t errcode = NO_ERRORS;
   // Yikes! this was signed16_t - DFH
   *val = (signed32_t)(operator[](index)  *0x00000001 + 
                       operator[](index+1)*0x00000100 +
                       operator[](index+2)*0x00010000 +
                       operator[](index+3)*0x01000000  );	// Yikes again, it was
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::GetValue8From(signed8_t* data, unsigned32_t  index) const
{
   signed8_t errcode = NO_ERRORS;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   if(m_pData != NULL)
   {
      if (index < m_nDataSize)
      {
         *data = m_pData[index];
      } else {
         errcode =  ERR_INVALID_INDEX;
      }
   } else {
      errcode =  ERR_DATASTORAGE_EMPTY;
   }
  
   return errcode;

}

/************************************************
 ************************************************/
signed8_t CUnit::AddDataArray(const unsigned8_t* newdata, size_t size)
{
   signed8_t errcode = NO_ERRORS;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	if (m_Index == INVALID_STORE_INDEX)
	{
		m_Index = m_Store.RequestIndex(size);
	}
#endif

	size_t spaceleft = GetSize() - GetDataSize();
  
   if(spaceleft < size) // allocate more space if new to-be-added array won't fit
   {
      Inflate(size-spaceleft/*+1*/);  // [i_a]
   }

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   if(newdata != NULL)
   {
      for(size_t i=0; i<size; i++)
         m_pData[m_nDataSize++] = newdata[i];

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	   m_Store[m_Index].SetDataSize(m_nDataSize);
#endif

   } else {
      //No data to add. Do nothing
	   if (size != 0)
		   return GENERAL_ERROR; // [i_a] at least report this very suspicious situation 
   }

   return errcode;
}

signed8_t CUnit::AddFixedDataArray(const unsigned8_t value, size_t size)
{
   signed8_t errcode = NO_ERRORS;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	if (m_Index == INVALID_STORE_INDEX)
	{
		m_Index = m_Store.RequestIndex(size);
	}
#endif

	size_t spaceleft = GetSize() - GetDataSize();
  
   if(spaceleft < size) // allocate more space if new to-be-added array won't fit
   {
      Inflate(size-spaceleft/*+1*/);  // [i_a]
   }

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   // The following can be a memset
   for(size_t i=0; i<size; i++)
      m_pData[m_nDataSize++] = value;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	   m_Store[m_Index].SetDataSize(m_nDataSize);
#endif

   return errcode;
}
//    signed8_t AddDataArray (const unsigned16_t* newdata, size_t size);
//    signed8_t AddFixedDataArray (const unsigned16_t value, size_t size);

/************************************************
 ************************************************/

/*
[i_a] What The Heck is this routine good for?
*/
signed8_t CUnit::RemoveTrailData(size_t remove_size)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   size_t newlen = GetDataSize() + remove_size;

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   m_Store[m_Index].Resize(newlen);
   memset(m_Store[m_Index].GetBuffer() + m_Store[m_Index].GetDataSize(), 0, remove_size);
   m_Store[m_Index].SetDataSize(newlen);

#else

   /*
     total_to_remove = (m_nSize - m_nDataSize) - remove_size;
     size of temp_data = m_nSize - total_to_remove = m_nDataSize + remove_size
   */
   size_t temp_size = m_nDataSize + remove_size;
   unsigned8_t* temp_data = new unsigned8_t[temp_size];
  
   if(temp_data != NULL)
   {
      for(size_t i=0; i<temp_size; i++)
         temp_data[i] = m_pData[i];
   } else {
      return GENERAL_ERROR;
   }
  
   m_nDataSize = temp_size;
   m_nSize = m_nDataSize;
   delete[] m_pData;
   m_pData = temp_data;

#endif

   return NO_ERRORS;
}

/************************************************
 ************************************************/

signed8_t CUnit::SetArrayAt(const unsigned8_t* newdata, size_t size, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;
   size_t spaceleft = GetSize() - index;

   if(spaceleft < size) // allocate more space if new to-be-added array won't fit
   {
      Inflate(size-spaceleft);
   }

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   //size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   if(newdata != NULL)
   {
      for(size_t i=0; i<size; i++)
      {
         m_pData[index++] = newdata[i];
      }
   } else {
      //No data to add. Do nothing
   }

   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue8(unsigned8_t newdata)
{
   if(GetDataSize() >= GetSize()) 
   {
      Inflate();
   }

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif
 
   m_pData[m_nDataSize++] = newdata;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   m_Store[m_Index].SetDataSize(m_nDataSize);
#endif

   return NO_ERRORS;
}

signed8_t CUnit::AddUnicodeString(CGlobalRecords& gRecords, const std::string& str, XlsUnicodeStringFormat_t fmt)
{
	std::string::const_iterator	cBegin, cEnd;
	signed8_t				errcode = NO_ERRORS;
	size_t strSize = 0;
	size_t strLen;
	size_t spaceleft;
	bool isASCII = CGlobalRecords::IsASCII(str);

	if (!isASCII)
	{
		u16string s16;

		XL_ASSERT(!"Should never happen!");

		gRecords.char2str16(str, s16);
		return AddUnicodeString(gRecords, s16, fmt);
	}

	strLen = str.length();
	
	switch (fmt)
	{
	case LEN1_NOFLAGS_ASCII: // RECTYPE_FONT
		strSize = 1;
		break;

	case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		strSize = 2;
		strSize += 1;	// flags byte
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
		strSize = 2;
		strSize += (strLen % 1);	// padding byte
		break;

	case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
		strSize = 1;
		strSize += 1;	// flags byte
		break;

	case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
		strSize = 0;
		strSize += 1;	// flags byte
		break;

	default:
		XL_ASSERT(!"should never go here!");
		break;
	}
	strSize += strLen;

	spaceleft = GetSize() - GetDataSize();
	if(spaceleft < strSize) // allocate more space if new to-be-added array won't fit
	{
	  Inflate(strSize-spaceleft+1);
	}

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   switch (fmt)
   {
   case LEN1_NOFLAGS_ASCII: // RECTYPE_FONT
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   break;

   case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	   m_pData[m_nDataSize++] = 0x00;	// ASCII
	   break;

   case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	   // the string is padded to be word-aligned with NUL bytes /preceding/ the text:
	   if (strLen % 1)
	   {
		   m_pData[m_nDataSize++] = 0x00;	// padding
	   }
	   break;

   case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = 0x00;	// ASCII
	   break;

   case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
	   m_pData[m_nDataSize++] = 0x00;	// ASCII
	   break;

   default:
	   XL_ASSERT(!"should never go here!");
	   break;
   }
	
	cBegin	= str.begin();
	cEnd	= str.end();
	
	while(cBegin != cEnd) 
	{
		m_pData[m_nDataSize++] = *cBegin++;
	}

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   m_Store[m_Index].SetDataSize(m_nDataSize);
#endif

   return errcode;
}
signed8_t CUnit::AddUnicodeString(CGlobalRecords& gRecords, const u16string& str16, XlsUnicodeStringFormat_t fmt)
{
	u16string::const_iterator	cBegin, cEnd;
	signed8_t					errcode = NO_ERRORS;
	size_t strSize = 0;
	size_t strLen;
	size_t spaceleft;
	bool isASCII = CGlobalRecords::IsASCII(str16);

	strLen = str16.length();
	
	switch (fmt)
	{
	case LEN1_NOFLAGS_ASCII: // RECTYPE_FONT
		strSize = 1;
		break;

	case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
		strSize = 2;
		strSize += 1;	// flags byte
		if (!isASCII)
		{
			strSize += strLen;	// UTF16 takes 2 bytes per char
		}
		break;

	case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
		strSize = 2;
		if (isASCII)
		{
			strSize += (strLen % 1);	// padding byte
		}
		else // if (!isASCII)
		{
			strSize += strLen;	// UTF16 takes 2 bytes per char
		}
		break;

	case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
		strSize = 1;
		strSize += 1;	// flags byte
		if (!isASCII)
		{
			strSize += strLen;	// UTF16 takes 2 bytes per char
		}
		break;

	case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
		strSize = 0;
		strSize += 1;	// flags byte
		if (!isASCII)
		{
			strSize += strLen;	// UTF16 takes 2 bytes per char
		}
		break;

	default:
		XL_ASSERT(!"should never go here!");
		break;
	}
	strSize += strLen; // hence: count 1 byte per char for ASCII, 2 bytes per char for UTF16

	spaceleft = GetSize() - GetDataSize();
	if(spaceleft < strSize) // allocate more space if new to-be-added array won't fit
	{
	  Inflate(strSize-spaceleft+1);
	}

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

   switch (fmt)
   {
   case LEN1_NOFLAGS_ASCII: // RECTYPE_FONT
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   if (!isASCII)
	   {
		   std::string s;

		   gRecords.str16toascii(str16, s);
		   
		   std::string::const_iterator	b, e;

		   b = s.begin();
		   e = s.end();

		   while(b != e) 
		   {
			   unsigned8_t c = *b++;

			   m_pData[m_nDataSize++] = c;
		   }
		   goto string_dump_done;
	   }
	   break;

   case LEN2_FLAGS_UNICODE: // RECTYPE_FORMAT, RECTYPE_LABEL -- 'regular'
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	   m_pData[m_nDataSize++] = (isASCII ? 0x00 : 0x01);	// ASCII or UTF-16
	   break;

   case LEN2_NOFLAGS_PADDING_UNICODE: // RECTYPE_NOTE (RECTYPE_TXO)
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	   // the string is padded to be word-aligned with NUL bytes /preceding/ the text:
	   if (isASCII && (strLen % 1))
	   {
		   m_pData[m_nDataSize++] = 0x00;	// padding
	   }
	   break;

   case LEN1_FLAGS_UNICODE: // RECTYPE_BOUNDSHEET
	   m_pData[m_nDataSize++] = strLen & 0xFF;
	   m_pData[m_nDataSize++] = (isASCII ? 0x00 : 0x01);	// ASCII or UTF-16
	   break;

   case NOLEN_FLAGS_UNICODE: // RECTYPE_NAME
	   m_pData[m_nDataSize++] = (isASCII ? 0x00 : 0x01);	// ASCII or UTF-16
	   break;

   default:
	   XL_ASSERT(!"should never go here!");
	   break;
   }

	
	cBegin	= str16.begin();
	cEnd	= str16.end();
	
	if (isASCII)
	{
		while(cBegin != cEnd) 
		{
			unsigned16_t c = *cBegin++;

			m_pData[m_nDataSize++] = static_cast<unsigned8_t>(c);
		}
	}
	else
	{
		while(cBegin != cEnd) 
		{
			unsigned16_t c = *cBegin++;

			m_pData[m_nDataSize++] = c & 0xFF;
			m_pData[m_nDataSize++] = (c >> 8) & 0xFF;
		}
	}

string_dump_done:

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   m_Store[m_Index].SetDataSize(m_nDataSize);
#endif

	return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::Inflate(size_t increase)
{
   signed8_t errcode = NO_ERRORS;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   if (m_Index == INVALID_STORE_INDEX)
	{
	   if (increase == 0)
	   {
			increase = CUnit::DefaultInflateSize;
	   }
		m_Index = m_Store.RequestIndex(increase);
	}
   else
   {
	   if (increase == 0)
	   {
		   size_t oldlen = m_Store[m_Index].GetSize();
		   if (oldlen < 64)
		   {
				increase = CUnit::DefaultInflateSize;
		   }
		   else
		   {
			   // bigger units grow faster: save on the number of realloc redimension operations...
			   increase = oldlen / 2;
		   }
	   }

	   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
	   m_Store[m_Index].Resize(GetSize() + increase);
   }

#else

   if (increase == 0)
   {
	   size_t oldlen = m_nSize;
	   if (oldlen < 64)
	   {
			increase = CUnit::DefaultInflateSize;
	   }
	   else
	   {
		   // bigger units grow faster: save on the number of realloc redimension operations...
		   increase = oldlen / 2;
	   }
   }

   // Create the new storage with increased size
   // and initialize it to 0.
   unsigned8_t* temp_storage = new unsigned8_t[m_nSize + increase];

   if(temp_storage != NULL)
   {
      memset(temp_storage, 0, (m_nSize+increase)*(sizeof(unsigned8_t)));
      // Copy data to the new storage
      memcpy(temp_storage, m_pData, m_nSize*sizeof(unsigned8_t));
  
      // Update the size
      m_nSize += increase;
  
      if (m_pData != NULL)
         delete []m_pData;

      m_pData = temp_storage;

	  // [i_a] 

	  // No errors... errcode already clean
   } else {
      errcode = ERR_UNABLE_TOALLOCATE_MEMORY;
   }
  
#endif

   return errcode;
}

/************************************************
 ************************************************/

unsigned8_t& CUnit::operator[](const size_t index) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   unsigned8_t *m_pData = m_Store[m_Index].GetBuffer();
   //size_t m_nDataSize = m_Store[m_Index].GetDataSize();

#endif

#if 1
   XL_ASSERT(index < GetSize());	// DFH: need to read ahead when setting bits in 32bit words
   XL_ASSERT(index < GetDataSize());	// [i_a]
   //if(index >= m_nDataSize) printf("ERROR: Short read!! \n");
#else
   // this old code really bad - get bad data and never know it!
   if(index >= m_nDataSize)
      return m_pData[m_nDataSize];
#endif
   return m_pData[index];
}

/************************************************
 ************************************************/

CUnit& CUnit::operator +=(const CUnit& from)
{
   if(&from != this)
   {
      Append(from);
   }
   else
   {
      //CUnit shadow(from.m_Store);
      //shadow  = from;
      Append(from);
   }
   return *this;
}

/************************************************
 ************************************************/

CUnit& CUnit::operator +=(unsigned8_t from)
{
   AddValue8(from);

   return *this;
}


/************************************************
 ************************************************/

signed8_t CUnit::Init(const unsigned8_t* data, const size_t size, const unsigned32_t datasz)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   m_Store[m_Index].Init(data, size, datasz);

#else

   m_nSize		= size;
   m_nDataSize	= datasz;

   XL_ASSERT(m_pData == NULL);
   if(m_pData)
      delete[] m_pData;

   m_pData = new unsigned8_t[m_nSize];
  
   if(data)
   {
      memset(m_pData, 0, m_nSize*sizeof(unsigned8_t));
      // Copy data to the new storage
      memcpy(m_pData, data, m_nSize*sizeof(unsigned8_t));
   }

#endif

   return NO_ERRORS;
}


/************************************************
 ************************************************/

signed8_t CUnit::Append (const CUnit& newunit)
{
   if(AddDataArray(newunit.GetBuffer(), newunit.GetDataSize()) == NO_ERRORS)
      return NO_ERRORS;
   else
      return GENERAL_ERROR;
}

/************************************************
 ************************************************/
signed8_t CUnit::InitFill (unsigned8_t data, size_t size)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   return m_Store[m_Index].InitWithValue(data, size);

#else

   if(m_pData)
      delete[] m_pData;

   m_pData = new unsigned8_t[size];

   if(m_pData)
   {
      memset(m_pData, data, size*sizeof(unsigned8_t));
      m_nSize = m_nDataSize = size;

      return NO_ERRORS;
   } else {
      return GENERAL_ERROR;
   }
#endif
}

/************************************************
 ************************************************/

size_t CUnit::GetSize (void) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   return m_Store[m_Index].GetSize();
#else
   return m_nSize;
#endif
}

/************************************************
 ************************************************/

size_t CUnit::GetDataSize(void) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   return m_Store[m_Index].GetDataSize();
#else
   return m_nDataSize;
#endif
}

/************************************************
 ************************************************/

const unsigned8_t* CUnit::GetBuffer(void) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   XL_ASSERT(m_Index != INVALID_STORE_INDEX);
   return m_Store[m_Index].GetBuffer();
#else
   return m_pData;
#endif
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: unit.cpp,v $
 * Revision 1.8  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.7  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.6  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.5  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/12/20 15:48:34  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

