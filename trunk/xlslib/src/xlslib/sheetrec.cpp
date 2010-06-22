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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/sheetrec.cpp,v $
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

#include <sheetrec.h>

#include <datast.h>

using namespace std;
using namespace xlslib_core;


#define MAX_ROWBLOCK_SIZE 16 // was: 32, but CONTINUE-d DBCELLs are not liked by 2003 ???

#define RB_DBCELL_MINSIZE          8
#define RB_DBCELL_CELLSIZEOFFSET   2

#define MAX_COLUMNS_PER_ROW			256 // Excel 2003 limit: 256 columns per row. (Update this when we upgrade this lib to support BIFF12 !)


using namespace std;

/*
**********************************************************************
worksheet class implementation
**********************************************************************
*/
worksheet::worksheet(CGlobalRecords& gRecords, unsigned16_t idx) :
	m_GlobalRecords(gRecords),
	m_DumpState(SHEET_INIT),
	m_pCurrentData(NULL),
	
	m_MergedRanges(),
	m_Colinfos(),
	m_Current_Colinfo(),
	m_RowHeights(),
	m_Current_RowHeight(),
	minRow((unsigned32_t)(-1)), minCol((unsigned32_t)(-1)),
	maxRow(0), maxCol(0),
	sheetIndex(idx),
	m_Cells(),
	m_CurrentCell(),
	m_CurrentSizeCell(),
#ifdef RANGE_FEATURE
	m_Ranges(),
#endif
	m_RBSizes(),
	m_Current_RBSize(),
	m_SizesCalculated(false),
	m_DumpRBState(RB_INIT),
	m_RowCounter(0),
	m_CellCounter(0),
	m_DBCellOffset(0),
	m_CellOffsets(),
	//m_CurrentRowBlock(0),
	m_Starting_RBCell(),
	cellIterHint(),
	cellHint(NULL)
{
}

worksheet::~worksheet()
{
#if 0 // [i_a] not needed any longer due to new dump preparation/estimation logic
	if(!m_RBSizes.empty())
      for(RBSize_Vect_Itor_t rbs = m_RBSizes.begin(); rbs != m_RBSizes.end(); rbs++)
	     delete *rbs;
#endif

   // Delete the dynamically created cell objects (pointers)
   if(!m_Cells.empty())
   {
	  // cout<<"worksheet::~worksheet(), this = "<<this<<endl;
      for(Cell_Set_Itor_t cell = m_Cells.begin(); cell != m_Cells.end(); cell++)
         delete *cell;
      m_Cells.clear();
   }

   // Delete dynamically allocated ranges of merged cells.
   if(!m_MergedRanges.empty())
   {
	  // cout<<"worksheet::~worksheet(), this = "<<this<<endl;
      for(Range_Vect_Itor_t mr = m_MergedRanges.begin(); mr != m_MergedRanges.end(); mr++)
         delete *mr;
      m_MergedRanges.clear();
   }

   // Delete dynamically allocated Cellinfo record definitions
   if(!m_Colinfos.empty())
   {
      for(Colinfo_Set_Itor_t ci = m_Colinfos.begin(); ci != m_Colinfos.end(); ci++)
         delete *ci;
      m_Colinfos.clear();
   }

/*
  if(!m_Colinfos.empty())
  {
  do
  {
  delete m_Colinfos.front();
  m_Colinfos.pop_front();
  }while(!m_Colinfos.empty());

  }
*/

   // Delete dynamically allocated Cellinfo record definitions
   if(!m_RowHeights.empty())
   {
      for(RowHeight_Vect_Itor_t rh = m_RowHeights.begin(); rh != m_RowHeights.end(); rh++)
         delete *rh;
      m_RowHeights.clear();
   }
   
/*
  if(!m_RowHeights.empty())
  {
  do
  {
  delete m_RowHeights.front();
  m_RowHeights.pop_front();
  }while(!m_RowHeights.empty());

  }
*/
#ifdef RANGE_FEATURE
   // Delete dynamically allocated range definitions
   if(!m_Ranges.empty())
   {
      for(RangeObj_Vect_Itor_t rh = m_Ranges.begin(); rh != m_Ranges.end(); rh++)
         delete *rh;
      m_Ranges.clear();
   }
#endif // RANGE_FEATURE
}



#if 0
/*
***********************************
***********************************
*/
void worksheet::SortCells()
{
// Set containers don't need sorting
}
#endif

/*
***********************************
***********************************
*/

#define RB_INDEX_MINSIZE 20

CUnit* worksheet::DumpData(CDataStorage &datastore, size_t offset)
{
   bool repeat = false;

   XTRACE("worksheet::DumpData");

   do
   {
	 switch(m_DumpState) {
	 case SHEET_INIT:
		m_DumpState = SHEET_BOF;
		m_Current_Colinfo = m_Colinfos.begin();
		m_CurrentSizeCell = m_Cells.begin();
		m_CurrentCell = m_Cells.begin();
		repeat  = true;
		break;

	 case SHEET_BOF:
		XTRACE("\tBOF");

		repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		m_pCurrentData = datastore.MakeCBof(BOF_TYPE_WORKSHEET);
#else
		//Delete_Pointer(m_pCurrentData);
		m_pCurrentData = (CUnit*)(new CBof(datastore, BOF_TYPE_WORKSHEET));
#endif
		m_DumpState = SHEET_INDEX;
		break;

	 case SHEET_INDEX:
		XTRACE("\tINDEX");
		{
			// NOTE: GetNumRowBlocks() has the side-effect of coughing up first/last col/row for the given sheet :-)
			rowblocksize_t rbsize;
			size_t numrb = GetNumRowBlocks(&rbsize);

			// Get first/last row from the list of row blocks
			//unsigned32_t first_row, last_row;
			//GetFirstLastRowsAndColumns(&first_row, &last_row, NULL, NULL);
	   
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			m_pCurrentData = datastore.MakeCIndex(rbsize.first_row, rbsize.last_row);
#else
			m_pCurrentData = (CUnit*)(new CIndex(datastore, rbsize.first_row, rbsize.last_row));
#endif

			size_t rb_size_acc = 0;
			size_t index_size = RB_INDEX_MINSIZE + 4*numrb;

			for(size_t rb = 0; rb < numrb; rb++)
			{
			   // Get sizes of next RowBlock
			   rowblocksize_t rbsize;
			   bool state = GetRowBlockSizes(rbsize);
			   XL_ASSERT(rb == numrb - 1 ? state == false : state == true);

			   // Update the offset accumulator and create the next DBCELL's offset
			   rb_size_acc += rbsize.rowandcell_size;
			   size_t dbcelloffset = offset + BOF_RECORD_SIZE + index_size + rb_size_acc;
			   ((CIndex*)m_pCurrentData)->AddDBCellOffset(dbcelloffset);

			   // Update the offset for the next DBCELL's offset
			   rb_size_acc += rbsize.dbcell_size;
			}
		}
		m_DumpState = SHEET_DIMENSION; // Change to the next state
		break;

	 case SHEET_DIMENSION:
		XTRACE("\tDIMENSION");

		repeat = false;

		//Delete_Pointer(m_pCurrentData);
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		m_pCurrentData = datastore.MakeCDimension(minRow, maxRow, minCol, maxCol);
#else
		m_pCurrentData = (CUnit*)(new CDimension(datastore, minRow, maxRow, minCol, maxCol));
#endif
		m_DumpState = SHEET_ROWBLOCKS;
		break;

	 case SHEET_ROWBLOCKS:
		XTRACE("\tROWBLOCKS");
		if(!m_Cells.empty()) // was if(GetNumRowBlocks())
		{
		   // First check if the list of RBs is not empty...
		   m_pCurrentData = RowBlocksDump(datastore);

		   if(m_pCurrentData == NULL)
		   {
			  repeat = true;
			  m_DumpState = SHEET_MERGED;
		   }  
		} else {
		   // if the list is empty, change the dump state.
		   repeat = true;
		   m_DumpState = SHEET_MERGED;
		}
		break;

	 case SHEET_MERGED:
		repeat = false;
		XTRACE("\tMERGED");

		if(!m_MergedRanges.empty())
		{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		   m_pCurrentData = datastore.MakeCMergedCells();
#else
		   m_pCurrentData = (CUnit*)(new CMergedCells(datastore));
#endif
			((CMergedCells*)m_pCurrentData)->SetNumRanges(m_MergedRanges.size());
		   for(Range_Vect_Itor_t mr = m_MergedRanges.begin(); mr != m_MergedRanges.end(); mr++)
		   {
			  ((CMergedCells*)m_pCurrentData)->AddRange(*mr);
		   }
		} else {
		   repeat = true;
		}

		m_DumpState = SHEET_COLINFO;
		break;

	 case SHEET_COLINFO:
		repeat = false;
		XTRACE("\tCOLINFO");

		if(!m_Colinfos.empty())
		{
		   // First check if the list of fonts is not empty...

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		   m_pCurrentData = datastore.MakeCColInfo(*m_Current_Colinfo);
#else
		   //Delete_Pointer(m_pCurrentData);
		   m_pCurrentData = (CUnit*)(new CColInfo(datastore, *m_Current_Colinfo));
#endif

		   if(m_Current_Colinfo != (--m_Colinfos.end()))
		   {
			  // if it wasn't the last font from the list, increment to get the next one
			  m_Current_Colinfo++;
		   } else {
			  // if it was the last from the list, change the DumpState
			  m_DumpState = SHEET_WINDOW2;
			  m_Current_Colinfo = m_Colinfos.begin();
		   }
		} else {
		   // if the list is empty, change the dump state.
		   m_DumpState = SHEET_WINDOW2;
		   //font = m_Fonts.begin();
		   repeat = true;
		}
		break;

	 case SHEET_WINDOW2:
		XTRACE("\tWINDOW2");
		repeat = false;
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		m_pCurrentData = datastore.MakeCWindow2(sheetIndex == m_GlobalRecords.GetWindow1().GetActiveSheet());
#else
		//Delete_Pointer(m_pCurrentData);
		m_pCurrentData = (CUnit*)(new CWindow2(datastore, (sheetIndex == m_GlobalRecords.GetWindow1().GetActiveSheet())));
#endif
		m_DumpState = SHEET_EOF;
		break;

	 case SHEET_EOF:
		XTRACE("\tEOF");
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		m_pCurrentData = datastore.MakeCEof();
#else
		//Delete_Pointer(m_pCurrentData);
		m_pCurrentData = (CUnit*)(new CEof(datastore));
#endif
		m_DumpState = SHEET_FINISH;
		break;

	 case SHEET_FINISH:
		XTRACE("\tFINISH");
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
#else
		//Delete_Pointer(m_pCurrentData);
#endif
		m_pCurrentData = NULL;
		m_DumpState = SHEET_INIT;
		break;
      }
   } while(repeat);

   return m_pCurrentData;
}

CUnit* worksheet::RowBlocksDump(CDataStorage &datastore)
{
   bool repeat = false;
   CUnit* rb_record = NULL;

   do 
   {
	 switch(m_DumpRBState) 
	 {
	 case RB_INIT:
		m_DumpRBState = RB_ROWS;
		//m_CurrentRowBlock = 0;
		m_RowCounter = 0;
		//m_CurrentCell = m_Cells.begin(); // moved by DFH to sheet init, should be OK

		// Initialize the row widths
		m_Current_RowHeight = m_RowHeights.begin();
		m_DumpRBState = RB_FIRST_ROW;
		repeat = true;

		break;

	 case RB_FIRST_ROW:
		repeat = true;
   
		XL_ASSERT(!m_Cells.empty());
		if(m_CurrentCell != m_Cells.end())  // [i_a]
		{
		   m_Starting_RBCell = m_CurrentCell;      
		   m_CellCounter = 0;
		   m_DBCellOffset = 0;
		   m_CellOffsets.clear();

		   m_DumpRBState = RB_ROWS;
		} else {
		   m_DumpRBState = RB_FINISH;
		}
		break;

	 case RB_ROWS:
	 {
		repeat = false;

		// Initialize first/last cols to impossible values
		// that are appropriate for the following detection algorithm
		unsigned32_t first_col = (unsigned32_t)(-1);
		unsigned32_t last_col  = 0;
		unsigned32_t row_num = 0;

		// Get the row number for the current row;
	    // The order in the conditional statement is important
		for (row_num = (*m_CurrentCell)->GetRow(); 
			m_CurrentCell != m_Cells.end() && (*m_CurrentCell)->GetRow() == row_num;
			m_CurrentCell++)
		{
		   // Determine the first/last column of the current row
		   unsigned32_t col_num = (*m_CurrentCell)->GetCol();
		   if(col_num > last_col)
			  last_col = col_num;
		   if(col_num < first_col)
			  first_col = col_num;

 		   m_CellCounter++;
		}

		// Check if the current row is in the list of height-set
		// rows.
		if(m_Current_RowHeight != m_RowHeights.end())
		{
		   if((*m_Current_RowHeight)->GetRowNum() == row_num)
		   {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			  rb_record = datastore.MakeCRow(row_num, first_col, 
											 last_col, 
											 (*m_Current_RowHeight)->GetRowHeight(),
											 (*m_Current_RowHeight)->GetXF());
#else
			  rb_record = (CUnit*) (new CRow(datastore, 
											 row_num, first_col, 
											 last_col, 
											 (*m_Current_RowHeight)->GetRowHeight()) );
#endif
			   m_Current_RowHeight++;          
			} else {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			  rb_record = datastore.MakeCRow(row_num, first_col, last_col);
#else
			  rb_record = (CUnit*) (new CRow(datastore, row_num, first_col, last_col) );
#endif
		   }
		} else {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		   rb_record = datastore.MakeCRow(row_num, first_col, last_col);
#else
		   rb_record = (CUnit*) (new CRow(datastore, row_num, first_col, last_col) );
#endif
		}

		m_DBCellOffset += ROW_RECORD_SIZE;
		XL_ASSERT(rb_record->GetDataSize() == ROW_RECORD_SIZE);

		// If the current row-block is full OR there are no more cells
		if(++m_RowCounter >= MAX_ROWBLOCK_SIZE || m_CurrentCell == m_Cells.end())
		{
		   if (m_CurrentCell == (--m_Cells.end()))
			  m_CellCounter++;
		   m_RowCounter = 0;
		   m_DumpRBState = RB_FIRSTCELL;        
		}
		break;
	 }

	 case RB_FIRSTCELL:
		rb_record = (*m_Starting_RBCell)->GetData(datastore);
   
		// Update the offset to be used in the DBCell Record
		m_DBCellOffset += rb_record->GetDataSize();

		// The first cell of the rowblock has an offset that includes (among others)
		// the rows size (without counting the first row)
		m_CellOffsets.push_back(m_DBCellOffset - ROW_RECORD_SIZE);

		// Update the pointer (iterator) to the next cell
		if(--m_CellCounter == 0)
		{
		   // The RowBlock's cells are done
		   m_DumpRBState = RB_DBCELL;
		} else {
		   m_Starting_RBCell++;
		   m_DumpRBState = RB_CELLS;
		}
		break;

	 case RB_CELLS:
		repeat = false;
		if(m_CellCounter == 0)
		{
			// The RowBlock's cells are done
		   m_DumpRBState = RB_DBCELL;

		   repeat = true;
		} else {
		   rb_record = (*m_Starting_RBCell)->GetData(datastore);

		   m_DBCellOffset += rb_record->GetDataSize();

		   m_CellOffsets.push_back(rb_record->GetDataSize());
		   m_CellCounter--;
		   m_Starting_RBCell++;
		}
		break;

	 case RB_DBCELL:
	    {
			repeat = false;
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			rb_record = datastore.MakeCDBCell(m_DBCellOffset);
#else
#endif
			CellOffsets_Vect_Itor_t celloffset;
			for(celloffset = m_CellOffsets.begin(); celloffset != m_CellOffsets.end(); celloffset++)
			   ((CDBCell*)rb_record)->AddRowOffset(*celloffset);

			if(m_CurrentCell == (--m_Cells.end()) )
			   m_DumpRBState = RB_FINISH;
			else
			   m_DumpRBState =  RB_FIRST_ROW;

			break;
	    }
	 case RB_FINISH:
		repeat = false;
		rb_record = NULL;
		m_DumpRBState =  RB_INIT;
		break;

	 default:
		break;
     }
   } while(repeat);

   return rb_record;
}


/*
***********************************
***********************************
*/

size_t worksheet::EstimateNumBiffUnitsNeeded(void)
{
	size_t ret = 7;

	ret += m_Colinfos.size();
	ret += m_RowHeights.size();
	ret += m_MergedRanges.size();
	/*
	and also add units for the number of rows: those DBCELL and ROW records...

	Note the order of the calls is important here: 
	  GetNumRowBlocks()
    preps some important per-instance tracking cursors and the RBSize cache
	which is used by the other calls to speed up the process!

	(trackers are: m_CurrentSizeCell, m_Current_RBSize)
	*/
	rowblocksize_t sheet_total;
	size_t dbcells = GetNumRowBlocks(&sheet_total);
	ret += dbcells + (MAX_ROWBLOCK_SIZE * MAX_COLUMNS_PER_ROW * RB_DBCELL_CELLSIZEOFFSET) / MAX_RECORD_SIZE; // worst-case: DBCELL+CONTINUEs for full-width rows
	ret += sheet_total.rows_sofar; // 1 per ROW
	ret += sheet_total.cells_sofar; // 1 per cell

	return ret;
}

/*
***********************************
***********************************
*/

void worksheet::MakeActive() { m_GlobalRecords.GetWindow1().SetActiveSheet(sheetIndex);}

/*
***********************************
***********************************
*/
cell_t* worksheet::label(unsigned32_t row, unsigned32_t col, 
                         const string& strlabel, xf_t* pxformat)
{
	u16string	str16;
	label_t* lbl;

	m_GlobalRecords.char2str16(strlabel, str16);

	lbl = new label_t(m_GlobalRecords, row, col, str16, pxformat);
	AddCell(lbl);

	return lbl;
}
cell_t* worksheet::label(unsigned32_t row, unsigned32_t col, 
                         const ustring& strlabel, xf_t* pxformat)
{
	u16string str16;
	label_t* lbl;

	m_GlobalRecords.wide2str16(strlabel, str16);
	lbl = new label_t(m_GlobalRecords, row, col, str16, pxformat);

	AddCell(lbl);

	return lbl;
}
/*
***********************************
***********************************
*/
#if 0 // no good - forces a format and thus xf_t
cell_t* worksheet::number(unsigned32_t row, unsigned32_t col, 
                          double numval, format_number_t fmtval,
                          xf_t* pxformat)
{
   number_t* number = new number_t(row, col, numval, pxformat);
   AddCell(number);
   number->format(fmtval);

   return number;
}
#endif
cell_t* worksheet::number(unsigned32_t row, unsigned32_t col, // Deprecated
                          double numval, format_number_t fmtval,
                          xf_t* pxformat)
{
	// this routine cannot handle greater values
	XL_ASSERT(fmtval <= FMT_TEXT);

	number_t* num = new number_t(m_GlobalRecords, row, col, numval, pxformat);
	AddCell(num);

	if(fmtval == FMT_GENERAL && pxformat == NULL) {
		; // OK
	} 
	else if(pxformat == NULL || pxformat->GetFormat() != fmtval) {
		// got to add it, will create a new xf_t
		//number->formatIndex(format2index(fmtval));
		num->format(fmtval);
	}
	return num;
}
cell_t* worksheet::number(unsigned32_t row, unsigned32_t col, 
                          double numval, xf_t* pxformat)
{
	number_t* num = new number_t(m_GlobalRecords, row, col, numval, pxformat);
	AddCell(num);
	return num;
}
cell_t* worksheet::number(unsigned32_t row, unsigned32_t col, // 536870911 >= numval >= -536870912
                          signed32_t numval, xf_t* pxformat)
{
	number_t* num = new number_t(m_GlobalRecords, row, col, numval, pxformat);
	AddCell(num);
	return num;
}


/*
***********************************
***********************************
*/
cell_t* worksheet::boolean(unsigned32_t row, unsigned32_t col, 
				bool boolval, xf_t* pxformat)
{
	boolean_t* num = new boolean_t(m_GlobalRecords, row, col, boolval, pxformat);
	AddCell(num);
	return num;
}

/*
***********************************
***********************************
*/
cell_t* worksheet::error(unsigned32_t row, unsigned32_t col, 
			  errcode_t errorcode, xf_t* pxformat)
{
	err_t* num = new err_t(m_GlobalRecords, row, col, errorcode, pxformat);
	AddCell(num);
	return num;
}

/*
***********************************
***********************************
*/
cell_t* worksheet::note(unsigned32_t row, unsigned32_t col, 
						const std::string& remark, const std::string& author, xf_t* pxformat)
{
	note_t* note = new note_t(m_GlobalRecords, row, col, remark, author, pxformat);
	AddCell(note);
	return note;
}

cell_t* worksheet::note(unsigned32_t row, unsigned32_t col, 
						const std::ustring& remark, const std::ustring& author, xf_t* pxformat)
{
	note_t* note = new note_t(m_GlobalRecords, row, col, remark, author, pxformat);
	AddCell(note);
	return note;
}


/*
***********************************
***********************************
*/
cell_t* worksheet::formula(unsigned32_t row, unsigned32_t col, 
						 expression_node_t* expression_root, 
						 bool auto_destruct_expression_tree,
						 xf_t* pxformat)
{
	formula_t* expr = new formula_t(m_GlobalRecords, row, col, expression_root, auto_destruct_expression_tree, pxformat);
	AddCell(expr);
	return expr;
}

/*
***********************************
***********************************
*/
cell_t* worksheet::blank(unsigned32_t row, unsigned32_t col, xf_t* pxformat)
{
   blank_t* blk = new blank_t(m_GlobalRecords, row, col, pxformat);
   AddCell(blk);
   return blk;
}

/*
***********************************
***********************************
*/
void worksheet::AddCell(cell_t* pcell)
{
	unsigned32_t		row, col; // [i_a]

	row = pcell->GetRow();
	col = pcell->GetCol();

	if(row < minRow) minRow = row;
	if(row > maxRow) maxRow = row;
	if(col < minCol) minCol = col;
	if(col > maxCol) maxCol = col;

	//SortCells(); does nothing now

#if 0 // original
	Cell_Set_Itor_t		existing_cell;
	// lookup the cell
	existing_cell = m_Cells.find(pcell);
	if(existing_cell != m_Cells.end())
	{
		if((*existing_cell)->GetXF())
			(*existing_cell)->GetXF()->UnMarkUsed();
		
		delete (*existing_cell);
		m_Cells.erase(existing_cell);
	}
	m_Cells.insert(pcell);
#endif

#if 1 // first try
	bool success;
	do {
		Cell_Set_Itor_t		ret;
		
		if(cellHint && pcell->row >= cellHint->row) {
			ret		= m_Cells.insert(cellIterHint, pcell);
			success	= (*ret == pcell);
		} else {
			pair<Cell_Set_Itor_t, bool> pr;
			
			pr = m_Cells.insert(pcell);
			ret		= pr.first;
			success	= pr.second;
		}
		
		if(!success) {
			cell_t* existing_cell;

			// means we got a duplicate - the user is overwriting an existing cell
			existing_cell = *(ret);
			m_Cells.erase(existing_cell);   // Bugs item #2840227:  prevent crash: first erase, then delete
			delete existing_cell;
			
			cellHint = NULL;
		} else {
			cellIterHint = ret;
			cellHint = pcell;
		}
	} while(!success);
#endif

	/*m_CellsSorted = false; */                  
	m_SizesCalculated = false;                   
	m_RBSizes.clear();                           
}

/*
***********************************
***********************************
*/
cell_t* worksheet::FindCell(unsigned32_t row, unsigned32_t col) const
{
   Cell_Set_CItor_t existing_cell;
   
   // need a cell to find a cell! So create simplest possible one
   cell_t* cell = new blank_t(m_GlobalRecords, row, col);
   existing_cell = m_Cells.find(cell);
   delete cell;

   // The find operation returns the end() iterator
   // if the cell wasn't found
   if(existing_cell != m_Cells.end())
   {
      return *existing_cell;
   } else {
      return NULL;
   }
}
cell_t* worksheet::FindCellOrMakeBlank(unsigned32_t row, unsigned32_t col)
{
	cell_t*	cell = worksheet::FindCell(row, col);
	
	return cell ? cell : blank(row,col);
}

/*
  void worksheet::AddCell(cell_t* pcell, bool overwrite)
  {

  #if STORAGE_CELL == LIST_CONTAINER
  m_Cells.push_back(pcell);

  #elif STORAGE_CELL == SET_CONTAINER
  m_Cells.insert(pcell);
  #endif
  MARK_CELLS_UNSORTED();
  }
*/



#if 0
/*
***********************************
***********************************
*/
unsigned32_t worksheet::Get-Size()
{
   unsigned32_t numrb;
   unsigned16_t merged_size;
   unsigned16_t colinfo_size;
   
   m_CurrentSizeCell = m_Cells.begin();
      
   numrb = GetNumRowBlocks();
	
   // The size of the merged cells record (if any) has to be taken in count
   if(!m_MergedRanges.empty())
   {
      //            [HEADER] + [NUMRANGESFIELD] + [RANGES]
      merged_size =  4       +  2               +  m_MergedRanges.size()*8; 
   } else {
      merged_size = 0;
   }
   
   // The size of the Colinfo records (if any) has to be taken in count
   if(!m_Colinfos.empty())
   {
      colinfo_size =  m_Colinfos.size()*16;		// was 14
   } else {
      colinfo_size = 0;
   }

	unsigned32_t size =
		BOF_SIZE + 
		RB_INDEX_MINSIZE + 
		4*numrb          + 
		merged_size      +
		colinfo_size     +
		WINDOW2_SIZE     +
		DIMENSION_SIZE   +
		EOF_SIZE;
   
   for(unsigned32_t rb = 0; rb < numrb; rb++)
   {
      // Get sizes of next RowBlock
      unsigned32_t rowandcell_size, dbcell_size;
      GetRowBlockSizes( &rowandcell_size, &dbcell_size);

      // Update the offset accumulator and cerate the next DBCELL's offset
      size += rowandcell_size;
      size += dbcell_size;
   }

   m_CurrentSizeCell = m_Cells.begin();
   
   return size;
}
#endif
/*
***********************************
Returns FALSE when the last row block was processed (and the cursors have been reset
to point at the start/top of the sheet again); returns TRUE when another row block
awaits after this one.
***********************************
*/
bool worksheet::GetRowBlockSizes(rowblocksize_t& rbsize)
{
   //SortCells();
   
   size_t row_counter = 0;
   size_t cell_counter = 0;
   size_t payload;

   //Cell_Set_Itor_t beginning_cell = m_CurrentSizeCell;

   // Initialize the size values (since they work as accumulators)
   rbsize.rowandcell_size = 0;
   rbsize.dbcell_size = 0;

   if(!m_SizesCalculated)
   {
      // Check if there are no cells
      if(!m_Cells.empty())
      {
		  unsigned32_t row_num;

          // The first cell is inside a row that has to be counted
		  row_counter = 1;

			row_num = (*m_CurrentSizeCell)->GetRow();
			if (rbsize.first_row > row_num)
				rbsize.first_row = row_num;
			if (rbsize.last_row < row_num)
				rbsize.last_row = row_num;

		  // Since the list of cells is sorted by rows, continuously equal cells (compared by row)
		  // conform one row... if the next one is different, increment the row counter
		  for ( ; m_CurrentSizeCell != m_Cells.end(); m_CurrentSizeCell++)
		  {
			if ((*m_CurrentSizeCell)->GetRow() != row_num)
			{
				// exit the loop if we have run through MAX rows:
				if (row_counter >= MAX_ROWBLOCK_SIZE)
					break;

				row_counter++;
				row_num = (*m_CurrentSizeCell)->GetRow();

				if (rbsize.first_row > row_num)
					rbsize.first_row = row_num;
				if (rbsize.last_row < row_num)
					rbsize.last_row = row_num;
			}

            cell_counter++;
	        // Get the size of the cell as well:
            rbsize.rowandcell_size += (*m_CurrentSizeCell)->GetSize();
			
			unsigned32_t col_num = (*m_CurrentSizeCell)->GetCol();
			if (rbsize.first_col > col_num)
				rbsize.first_col = col_num;
			if (rbsize.last_col < col_num)
				rbsize.last_col = col_num;
         }

         rbsize.rows_sofar += row_counter;
         // Get the size of the rows
         rbsize.rowandcell_size += ROW_RECORD_SIZE*row_counter;

		rbsize.cells_sofar += cell_counter;

		// Now get the size of the DBCELL
		 payload = RB_DBCELL_CELLSIZEOFFSET*cell_counter;

         rbsize.dbcell_size += RB_DBCELL_MINSIZE;
         rbsize.dbcell_size += payload;

         // Check the size of the data in the DBCELL record (without the header)
         // to take in account the overhead of the CONTINUE record (4bytes/CONTrec)
         if(payload > MAX_RECORD_SIZE)
         {
            size_t cont_overhead = ((payload + MAX_RECORD_SIZE - 1) / MAX_RECORD_SIZE); // continue payloads; account for trailing partial payloads: round up!

            rbsize.dbcell_size += (cont_overhead-1)*4;
         }

         //rbsize->rowandcell_size = *rowandcell_size;
         //rbsize->dbcell_size = *dbcell_size;
         //rbsize.rows_sofar = row_counter;
		 //rbsize.cells_sofar = cell_counter;
         m_RBSizes.push_back(rbsize);
    
         // If it was the last block, reset the current-label pointer
         if(m_CurrentSizeCell == m_Cells.end())
         {
            m_CurrentSizeCell = m_Cells.begin();
            m_Current_RBSize = m_RBSizes.begin();
            m_SizesCalculated = true;
       
            return false;
         }
      }   
      
      // If there are no cells in the sheet, return sizes = 0.
      if(m_Cells.empty())
         return false;
      else
         return true;
   } else {
      rbsize = *m_Current_RBSize;
      m_Current_RBSize++;

      // Reset the current RBSize
      if(m_Current_RBSize == m_RBSizes.end())
      {
         m_Current_RBSize = m_RBSizes.begin();
         return false;
      }
   }
   return true;
}


#if 0
/*
***********************************
***********************************
*/
void  worksheet::GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row)
{
   // First check that the m_Cells list is not empty, so we won't dereference
   // an empty iterator.
   if(!m_Cells.empty())
   {
      //SortCells();

      cell_t* pcell;
      pcell = *(m_Cells.begin());
      *first_row = pcell->GetRow();

      pcell = *(--m_Cells.end());
      *last_row = pcell->GetRow();
   } else {
      // If there is no cells in the list the first/last rows
      // are defaulted to zero.
      *first_row = 0;
      *last_row = 0;
   }
}
#endif


/*
***********************************
***********************************
*/
void worksheet::GetFirstLastRowsAndColumns(unsigned32_t* first_row, unsigned32_t* last_row, unsigned32_t* first_col, unsigned32_t* last_col) /* [i_a] */
{
   // First check that the m_Cells list is not empty, so we won't dereference
   // an empty iterator.
   if(!m_Cells.empty())
   {
      //SortCells();

	  /*
	  speedup for when we are not interested to hear about the columns:
	  since this vector is sorted by row first, we can simply grab the 
	  last unit to get the highest rownum:
	  */
	  if (!first_col && !last_col)
	  {
		  cell_t *f = *m_Cells.begin();      // .front()
		  cell_t *l = *(--m_Cells.end());  // .back()

		  XL_ASSERT(f);
		  XL_ASSERT(l);

		  if (first_row) 
			*first_row = f->GetRow();
		  if (last_row)
			*last_row = l->GetRow();
	  }
	  else
	  {
			rowblocksize_t rbsize;
			GetNumRowBlocks(&rbsize);

		  if (first_row) 
			  *first_row = rbsize.first_row;
		  if (last_row)
			  *last_row = rbsize.last_row;
		  if (first_col) 
			  *first_col = rbsize.first_col;
		  if (last_col)
			  *last_col = rbsize.last_col;
	  }
   } else {
      // If there is no cells in the list the first/last rows
      // are defaulted to zero.
	  if (first_row) 
		*first_row = 0;
	  if (last_row)
		*last_row = 0;
	  if (first_col) 
		*first_col = 0;
	  if (last_col)
		*last_col = 0;
   }
}

/*
***********************************
***********************************
*/
size_t worksheet::GetNumRowBlocks(rowblocksize_t* rbsize_ref)
{
   size_t numrb;

   if (!m_Cells.empty())
   {
    m_CurrentSizeCell = m_Cells.begin();
    m_Current_RBSize = m_RBSizes.begin();

	rowblocksize_t rbsize;
	if (rbsize_ref)
	{
		rbsize_ref->reset();
	}
	else
	{
		rbsize_ref = &rbsize;
	}

   bool cont = false;
   do
   {
		rowblocksize_t rb1;
		cont = GetRowBlockSizes(rb1);

		rbsize_ref->cells_sofar += rb1.cells_sofar;
		rbsize_ref->dbcell_size += rb1.dbcell_size;
		if (rbsize_ref->first_col > rb1.first_col)
			rbsize_ref->first_col = rb1.first_col;
		if (rbsize_ref->first_row > rb1.first_row)
			rbsize_ref->first_row = rb1.first_row;
		if (rbsize_ref->last_col < rb1.last_col)
			rbsize_ref->last_col = rb1.last_col;
		if (rbsize_ref->last_row < rb1.last_row)
			rbsize_ref->last_row = rb1.last_row;
		rbsize_ref->rowandcell_size += rb1.rowandcell_size;
		rbsize_ref->rows_sofar += rb1.rows_sofar;
   } while(cont);

/*
  Cell_Vect_t temp_cell_list = m_Cells;
  temp_cell_list.sort();
  temp_cell_list.unique();
*/

#if 0
	   // take trailing partial chunk into account: round up!
      numrb = (rbsize_ref->rows_sofar + MAX_ROWBLOCK_SIZE - 1) / MAX_ROWBLOCK_SIZE;
#else
	  numrb = m_RBSizes.size();
#endif
   } 
   else 
   {
      // If the m_Cell list is empty, there are no rowblocks in the sheet.
      numrb = 0;
   }

   return numrb;
}

/*
***********************************
***********************************
*/
void worksheet::merge(unsigned32_t first_row, unsigned32_t first_col, 
                      unsigned32_t last_row, unsigned32_t last_col)
{
   range_t* newrange = new range_t;

   newrange->first_row = first_row;
   newrange->last_row  = last_row;
   newrange->first_col = first_col;
   newrange->last_col  = last_col;

   m_MergedRanges.push_back(newrange);
}

/*
***********************************
***********************************
*/
void worksheet::colwidth(unsigned32_t col, unsigned16_t width, xf_t* pxformat)
{
	colinfo_t* newci = new colinfo_t;
	Colinfo_Set_Itor_t existing_ci;

	if(pxformat)
		pxformat->MarkUsed();

	// TODO: not sure why these are all public...
	newci->colfirst	= col;
	newci->collast	= col;
	newci->flags	= 0x00;
	newci->width	= width;				// sets column widths to 1/256 x width of "0"
	newci->xformat	= pxformat;

	// already have one?
	existing_ci = m_Colinfos.find(newci);
	  
	if(existing_ci != m_Colinfos.end())
	{
		if((*existing_ci)->xformat)
			(*existing_ci)->xformat->UnMarkUsed();
		
		delete (*existing_ci);
		
		m_Colinfos.erase(existing_ci);
		m_Colinfos.insert(newci);
	} else {
		m_Colinfos.insert(newci);
	}
}


/*
***********************************
***********************************
*/
void worksheet::rowheight(unsigned32_t row, unsigned16_t height, xf_t* pxformat)
{
	rowheight_t* newrh = new rowheight_t(row,height*20,pxformat);
	RowHeight_Vect_Itor_t existing_rh;

	// should be in rowheight_t but too much trouble
	if(pxformat)
		pxformat->MarkUsed();

	//m_RowHeights.insert(newrh);
	existing_rh = m_RowHeights.find(newrh);
	  
	if(existing_rh != m_RowHeights.end())
	{
		if((*existing_rh)->GetXF())
			(*existing_rh)->GetXF()->UnMarkUsed();

		delete (*existing_rh);
		m_RowHeights.erase(existing_rh);
		m_RowHeights.insert(newrh);
	} else {
		m_RowHeights.insert(newrh);
   }
}

#ifdef RANGE_FEATURE
range* worksheet::rangegroup(unsigned32_t row1, unsigned32_t col1,
                             unsigned32_t row2, unsigned32_t col2)
{
	range* newrange = new range(row1, col1, row2, col2, this); 
	m_Ranges.push_back(newrange);

	return newrange;
}
#endif // RANGE_FEATURE


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: sheetrec.cpp,v $
 * Revision 1.13  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.12  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.11  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.10  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.9  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.8  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.7  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.6  2008/12/11 21:12:02  dhoerl
 * Cleanup
 *
 * Revision 1.5  2008/12/10 03:34:41  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.4  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:51  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
