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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/workbook.cpp,v $
 * $Revision: 1.13 $
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

#include <globalrec.h>

#include <workbook.h>

#ifdef HAVE_ICONV
#include <errno.h>
#endif

using namespace std;
using namespace xlslib_core;

#define CHANGE_DUMPSTATE(state) {               \
   m_PreviousDumpState = m_DumpState;           \
   m_DumpState = state;                         \
}

/*
**********************************************************************
workbook class implementation
**********************************************************************
*/
workbook::workbook() :
	m_GlobalRecords(),
	m_ExprFactory(m_GlobalRecords),
	m_SummaryInfo(),
	m_DocSummaryInfo(),
	m_Sheets(),
	m_DumpState(WB_INIT),
	m_PreviousDumpState(WB_FINISH),
	sheetIndex(0),
	m_pCurrentData(NULL),
	writeLen(0),
	m_pContinueRecord(NULL),
	m_ContinueIndex(0),
	current_sheet(0),
	offset(0)
{
#if HAVE_ICONV
	m_GlobalRecords.SetIconvCode("wchar_t");
#endif
}

workbook::~workbook()
{
   if(!m_Sheets.empty())
   {
      for(size_t i = 0; i<m_Sheets.size(); i++)
      {
		delete m_Sheets[i];
      }
   }
 //  CGlobalRecords::Clean();
}

#ifdef HAVE_ICONV
int workbook::iconvInType(const char *inType)
{
	int			ret;	
	iconv_t		cd;
	
	cd = iconv_open("UCS-2-INTERNAL", inType);
	if(cd != (iconv_t)(-1)) {
		iconv_close(cd);
		m_GlobalRecords.SetIconvCode(inType);
		ret = 0;
	} else {
		ret = errno;
	}
	return ret;
}
#endif

/*
***********************************
***********************************
*/
worksheet* workbook::sheet(const string& sheetname)
{
	u16string	str16;

	worksheet* pnewsheet = new worksheet(m_GlobalRecords, sheetIndex++);
	m_GlobalRecords.char2str16(sheetname, str16);

	m_Sheets.push_back(pnewsheet);

	// NOTE: Streampos defaults to 0
	// It has to be set somewhere else
	m_GlobalRecords.AddBoundingSheet(0, BSHEET_ATTR_WORKSHEET, str16);

	// Return a pointer to the just added sheet
	return (m_Sheets.back());
}
worksheet* workbook::sheet(const ustring& sheetname)
{
	u16string	str16;
   
	worksheet* pnewsheet = new worksheet(m_GlobalRecords, sheetIndex++);
	m_GlobalRecords.wide2str16(sheetname, str16);
	
	m_Sheets.push_back(pnewsheet);

	// NOTE: Streampos defaults to 0
	// It has to be set somewhere else
	m_GlobalRecords.AddBoundingSheet(0, BSHEET_ATTR_WORKSHEET, str16);

	// Return a pointer to the just added sheet
	return (m_Sheets.back());
}

/*
***********************************
***********************************
*/
worksheet* workbook::GetSheet(unsigned16_t sheetnum)
{
   if(sheetnum < m_Sheets.size())
   {
      return m_Sheets[sheetnum];
   } else {
      return NULL;
   }
}

/*
***********************************
***********************************
*/
expression_node_factory_t& workbook::GetFormulaFactory(void)
{
	return m_ExprFactory;
}

/*
***********************************
***********************************
*/
font_t* workbook::font(const string& name)
{
   font_t* newfont = new font_t(m_GlobalRecords);
   newfont->SetName(name);
   m_GlobalRecords.AddFont(newfont);

   return newfont;
}
font_t* workbook::font(unsigned8_t fontnum)
{
	return m_GlobalRecords.fontdup(fontnum);
}

/*
***********************************
***********************************
*/
format_t* workbook::format(const string& formatstr)
{
	u16string	str16;
	format_t* newformat;

	m_GlobalRecords.char2str16(formatstr, str16);

	newformat = new format_t(m_GlobalRecords, str16);
	m_GlobalRecords.AddFormat(newformat);

	return newformat;
}
format_t* workbook::format(const ustring& formatstr)
{
	u16string	str16;
	format_t* newformat;

	m_GlobalRecords.wide2str16(formatstr, str16);
	
	newformat = new format_t(m_GlobalRecords, str16);
	m_GlobalRecords.AddFormat(newformat);

	return newformat;
}

xf_t* workbook::xformat(void)
{
   xf_t* newxf = new xf_t(m_GlobalRecords, true);		// bool userXF=true, bool isCell=true, bool isMasterXF=false
   
   return newxf;
}

xf_t* workbook::xformat(font_t* fnt)
{
   xf_t* newxf = new xf_t(m_GlobalRecords, true);
   newxf->SetFont(fnt);

   return newxf;
}

xf_t* workbook::xformat(format_t* fmt)
{
	xf_t* newxf = new xf_t(m_GlobalRecords, true);
	newxf->SetFormat(fmt);

	return newxf;
}

xf_t* workbook::xformat(font_t* fnt, format_t* fmt)
{
	xf_t* newxf = new xf_t(m_GlobalRecords, true);
	newxf->SetFont(fnt);
	newxf->SetFormat(fmt);

	return newxf;
}

bool workbook::setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx)
{
	return m_GlobalRecords.SetColor(r, g, b, idx);
}

/*
***********************************
***********************************
*/
bool workbook::property(property_t prop, const string& content)
{
	// who gets it?
	if(prop >= PROP_LAST) return false;
	
	if(property2summary[prop] > 0) {
		return m_SummaryInfo.property(prop, content);
	} else
	if(property2docSummary[prop] > 0) {
		return m_DocSummaryInfo.property(prop, content);
	} else {
		return false;
	}
}

void workbook::windPosition(unsigned16_t horz, unsigned16_t vert) { m_GlobalRecords.GetWindow1().SetPosition(horz, vert);}
void workbook::windSize(unsigned16_t width, unsigned16_t height) { m_GlobalRecords.GetWindow1().SetSize(width, height);}
void workbook::firstTab(unsigned16_t fTab) { m_GlobalRecords.GetWindow1().SetFirstTab(fTab);}
void workbook::tabBarWidth(unsigned16_t width) { m_GlobalRecords.GetWindow1().SetTabBarWidth(width);}

/*
***********************************
***********************************
*/
int workbook::Dump(const string& filename)
{
	Sheets_Vector_Itor_t	sBegin, sEnd, sIter;
	size_t cells;
	string					name;
	int						errors;

	if(m_Sheets.empty()) {
		return GENERAL_ERROR;
	}
	
	// pre-allocate an approximation of what will be needed to store the data objects
	sBegin	= m_Sheets.begin();
	sEnd	= m_Sheets.end();
	cells	= 0;
	/*
	Since it's VERY costly to redimension the cell unit store vector when 
	we're using lightweight CUnitStore elements et al
 	  (defined(LEIGHTWEIGHT_UNIT_FEATURE))
    we do our utmost best to estimate the total amount of storage units
	required to 'dump' our spreadsheet. The estimate should be conservative,
	but not too much. After all, we're attempting to reduce the memory 
	footprint for this baby when we process multi-million cell spreadsheets...
	*/
	for(sIter=sBegin; sIter<sEnd; ++sIter) 
	{
		// add a number of units for each worksheet,
		cells += (*sIter)->EstimateNumBiffUnitsNeeded();
	}
	// global units:
	cells += this->m_GlobalRecords.EstimateNumBiffUnitsNeeded4Header();
	cells += 1000; // safety margin

	XTRACE2("ESTIMATED: total storage unit count: ", cells);
#if OLE_DEBUG
	std::cerr << "ESTIMATED: total unit count: " << cells << std::endl;
#endif

	errors = Open(filename);

	if(errors == NO_ERRORS) {
		CDataStorage		biffdata(cells);
		CUnit*				precorddata;
		bool				keep = true;

		do
		{
		  precorddata = DumpData(biffdata);

		  if(precorddata != NULL) {
			 biffdata += precorddata;
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			 //Delete_Pointer(precorddata);

 			// and we can already discard any previous units at lower backpatch levels
			biffdata.FlushLowerLevelUnits(precorddata);
#endif
		  } else {
			 keep = false;
		  }
		} while(keep);

		AddFile("/Workbook", &biffdata);

		name = (char)0x05;
		name += "SummaryInformation";
		m_SummaryInfo.DumpData();
		AddFile(name,  &m_SummaryInfo);
		
		name = (char)0x05;
		name += "DocumentSummaryInformation";
		m_DocSummaryInfo.DumpData();
		AddFile(name, &m_DocSummaryInfo);
		
		errors = DumpOleFile();
		Close();
	}
	return errors;
}

/*
***********************************
***********************************
*/
CUnit* workbook::DumpData(CDataStorage &datastore)
{
   bool repeat = false;

   XTRACE("\nworkbook::DumpData");

   do
   {
      switch(m_DumpState)
      {
         case WB_INIT:

			writeLen = 0;
            current_sheet = 0;
			offset = 0;

            CHANGE_DUMPSTATE(WB_GLOBALRECORDS);

            repeat = true;
            break;

         case WB_GLOBALRECORDS:

            XTRACE("\tGLOBALRECORDS");
 
            m_pCurrentData = m_GlobalRecords.DumpData(datastore);
            if(m_pCurrentData == NULL)
            {
				offset = writeLen;
				writeLen = 0;
			   
				repeat = true;
				CHANGE_DUMPSTATE(WB_SHEETS);
            } else {
				writeLen += m_pCurrentData->GetDataSize();
               // Do nothing. Continue in this state.
				repeat = false;
            }
            break;

         case WB_SHEETS:
         {
            XTRACE("\tSHEETS");

            m_pCurrentData = m_Sheets[current_sheet]->DumpData(datastore, offset);
            if(m_pCurrentData == NULL)
            {
				Boundsheet_Vect_Itor_t bs = m_GlobalRecords.GetBoundSheetAt(current_sheet);

				(*bs)->SetSheetStreamPosition(offset);

				if((current_sheet+1) < (unsigned16_t)m_Sheets.size()) // [i_a]
				{
					// Update the offset for the next sheet
					offset += writeLen;
					writeLen = 0;
					current_sheet++;
				} else {
					// I'm done with all the sheets
					// Nothing else to do. Branch to the FINISH state
					CHANGE_DUMPSTATE(WB_FINISH);
				}
				repeat = true;
           } else {
				writeLen += m_pCurrentData->GetDataSize();
				repeat = false;
			}
            break;
         }
         case WB_FINISH:
            XTRACE("\tFINISH");

            repeat = false;
            m_pCurrentData  = NULL;

            CHANGE_DUMPSTATE(WB_INIT);

            break;

         case WB_CONTINUE_REC:
            XTRACE("\tCONTINUE-REC");

            repeat = false;

            if(m_ContinueIndex == 0)
            {
               //Create a new data unit containing the max data size
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pContinueRecord = datastore.MakeCRecord();
#else
               m_pContinueRecord = (CUnit*)(new CRecord(datastore));
#endif

               // The real size of the record is the size of the buffer minus the
               // size of the header record

               ((CUnit*)(m_pContinueRecord))->AddDataArray(((CRecord*)m_pCurrentData)->GetRecordDataBuffer(), MAX_RECORD_SIZE);
               ((CRecord*)(m_pContinueRecord))->SetRecordType(((CRecord*)m_pCurrentData)->GetRecordType());
               ((CRecord*)(m_pContinueRecord))->SetRecordLength(MAX_RECORD_SIZE);

				//m_pContinueRecord->SetValueAt(MAX_RECORD_SIZE-4,2);
               m_ContinueIndex++;

               return m_pContinueRecord;
            } else {
               //Delete_Pointer(m_pContinueRecord);

               // Get a pointer to the next chunk of data
               const unsigned8_t* pdata = (((CRecord*)m_pCurrentData)->GetRecordDataBuffer()) + m_ContinueIndex*MAX_RECORD_SIZE;

               // Get the size of the chunk of data (that is the MAX_REC_SIZE except by the last one)
               size_t csize = 0;
               if(( ((CRecord*)m_pCurrentData)->GetRecordDataSize()/MAX_RECORD_SIZE) > m_ContinueIndex)
               {
                  csize = MAX_RECORD_SIZE;
                  m_ContinueIndex++;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
                  m_pContinueRecord = datastore.MakeCContinue(pdata, csize);
#else
                  m_pContinueRecord =(CUnit*) ( new CContinue(datastore, pdata, csize));
#endif

                  return m_pContinueRecord;
               } else {
				  size_t data_size = ((CRecord*)m_pCurrentData)->GetRecordDataSize();

                  csize = data_size - m_ContinueIndex * MAX_RECORD_SIZE;

                  // Restore the previous state (*Don't use the macro*)
                  m_DumpState = m_PreviousDumpState;
                  m_PreviousDumpState = WB_CONTINUE_REC;

                  m_ContinueIndex = 0;

                  if(csize)
                  {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
                     m_pContinueRecord = datastore.MakeCContinue(pdata, csize);
					 //((CRecord*)m_pCurrentData)->SetRecordLength(MAX_RECORD_SIZE);
#else
                     m_pContinueRecord = (CUnit*) new CContinue(datastore, pdata, csize);
					 Delete_Pointer(m_pCurrentData);
#endif
					 return m_pContinueRecord;
                  } else {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
					 //((CRecord*)m_pCurrentData)->SetRecordLength(MAX_RECORD_SIZE);
#else
					 Delete_Pointer(m_pCurrentData);
#endif
                     repeat = true;
                  }
               }
            }

            break;

         default:
            XTRACE("\tDEFAULT");
            break;
      }

      if(m_pCurrentData != NULL) {
		 // WARNING: the test below was >= MAX_..., but MAX size is OK - only continue if > (I think!) DFH 12-12-08
		 // Should only happen with single cells having data > MAX_RECORD_SIZE. Have no idea if this works or not (DFH)
         if(((CRecord*)m_pCurrentData)->GetRecordDataSize() > MAX_RECORD_SIZE && m_DumpState != WB_CONTINUE_REC)
         {
            // Save the current dump state and change to the CONTINUE Record state
            CHANGE_DUMPSTATE(WB_CONTINUE_REC);

            m_ContinueIndex = 0;

            repeat = true;
         }
	   }
   } while(repeat);

   return m_pCurrentData;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: workbook.cpp,v $
 * Revision 1.13  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.12  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.11  2009/01/10 21:10:51  dhoerl
 * More tweaks
 *
 * Revision 1.10  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.9  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.8  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.7  2009/01/08 02:52:31  dhoerl
 * December Rework
 *
 * Revision 1.6  2008/12/20 15:47:41  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.5  2008/12/10 03:34:54  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.4  2008/10/27 01:12:20  dhoerl
 * Remove PHP
 *
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */