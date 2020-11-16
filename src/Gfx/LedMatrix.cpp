/* MIT License
 *
 * Copyright (c) 2019 - 2020 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  LED matrix
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "LedMatrix.h"

#include <Util.h>

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/* Initialize LED matrix instance. */
LedMatrix LedMatrix::m_instance;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

LedMatrix::LedMatrix() :
    IGfx(Board::LedMatrix::width, Board::LedMatrix::height),
    m_topology(TOPOLOGY_COLUMN_MAJOR_ALTERNATING),
    m_strip(Board::LedMatrix::width * Board::LedMatrix::height, Board::Pin::ledMatrixDataOutPinNo),
    m_topo(nullptr)
{
    createLayout(m_topology);
}

LedMatrix::~LedMatrix()
{
    if (nullptr != m_topo)
    {
        delete m_topo;
    }
}

Color LedMatrix::getColor(int16_t x, int16_t y) const
{
    HtmlColor htmlColor = ColorDef::BLACK;

    if (nullptr != m_topo)
    {
        htmlColor = m_strip.GetPixelColor(m_topo->map(x, y));
    }

    return Color(htmlColor.Color);
}

void LedMatrix::createLayout(LedMatrix::Topology topo)
{
    if (nullptr != m_topo)
    {
        delete m_topo;
        m_topo = nullptr;
    }

    switch(m_topology)
    {
    case TOPOLOGY_ROW_MAJOR:
        m_topo = new LedMatrixLayout<RowMajorLayout>(m_width, m_height);
        break;

    case TOPOLOGY_ROW_MAJOR_90:
        m_topo = new LedMatrixLayout<RowMajor90Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_180:
        m_topo = new LedMatrixLayout<RowMajor180Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_270:
        m_topo = new LedMatrixLayout<RowMajor270Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_ALTERNATING:
        m_topo = new LedMatrixLayout<RowMajorAlternatingLayout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_ALTERNATING_90:
        m_topo = new LedMatrixLayout<RowMajorAlternating90Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_ALTERNATING_180:
        m_topo = new LedMatrixLayout<RowMajorAlternating180Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_ROW_MAJOR_ALTERNATING_270:
        m_topo = new LedMatrixLayout<RowMajorAlternating270Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR:
        m_topo = new LedMatrixLayout<ColumnMajorLayout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_90:
        m_topo = new LedMatrixLayout<ColumnMajor90Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_180:
        m_topo = new LedMatrixLayout<ColumnMajor180Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_270:
        m_topo = new LedMatrixLayout<ColumnMajor270Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_ALTERNATING:
        m_topo = new LedMatrixLayout<ColumnMajorAlternatingLayout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_ALTERNATING_90:
        m_topo = new LedMatrixLayout<ColumnMajorAlternating90Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_ALTERNATING_180:
        m_topo = new LedMatrixLayout<ColumnMajorAlternating180Layout>(m_width, m_height);
        break;
        
    case TOPOLOGY_COLUMN_MAJOR_ALTERNATING_270:
        m_topo = new LedMatrixLayout<ColumnMajorAlternating270Layout>(m_width, m_height);
        break;
        
    default:
        break;
    }
}

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/
