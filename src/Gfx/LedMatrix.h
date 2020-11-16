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
 *
 * @addtogroup gfx
 *
 * @{
 */

#ifndef __LEDMATRIX_H__
#define __LEDMATRIX_H__

/******************************************************************************
 * Compile Switches
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <IGfx.hpp>
#include <NeoPixelBrightnessBus.h>
#include <ColorDef.hpp>

#include "Board.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 * LED matrix topology base class.
 * Derived from the NeoTopology class and used for runtime configuration of the
 * physical matrix layout.
 */
class LedMatrixTopology
{
public:

    /**
     * Constructs the matrix topology base object.
     * 
     * @param[in] width     Matrix width in pixels
     * @param[in] height    Matrix height in pixels
     */
    LedMatrixTopology(uint16_t width, uint16_t height) :
        m_width(width),
        m_height(height)
    {
    }

    /**
     * Constructs the matrix topology by assignment.
     */
    LedMatrixTopology(const LedMatrixTopology& topo) :
        m_width(topo.m_width),
        m_height(topo.m_height)
    {
    }

    /**
     * Destroys the matrix topology base object.
     */
    virtual ~LedMatrixTopology()
    {
    }

    /**
     * Get matrix width.
     * 
     * @return Width in pixels
     */
    uint16_t getWidth() const 
    {
        return m_width;
    }

    /**
     * Get matrix height.
     * 
     * @return Height in pixels
     */
    uint16_t getHeight() const 
    {
        return m_height;
    }

    /**
     * Map the framebuffer coordinates to the matrix position, depended on
     * its physical layout. If the given coordinates are outside the defined
     * with/height, they will be limited accordingly.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * 
     * @return Index position in the matrix
     */
    virtual uint16_t map(int16_t x, int16_t y) const = 0;

    /**
     * Map the framebuffer coordinates to the matrix position, depended on
     * its physical layout. If the given coordinates are outside the defined
     * with/height, the returned position counts width * height.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * 
     * @return Index position in the matrix
     */
    virtual uint16_t mapProbe(int16_t x, int16_t y) const = 0;

protected:

    const uint16_t  m_width;    /**< Matrix width in pixels */
    const uint16_t  m_height;   /**< Matrix height in pixels */

    LedMatrixTopology();
};

/**
 * LED matrix layout class.
 * Derived from the NeoTopology class and used for runtime configuration of the
 * physical matrix layout.
 * 
 * @tparam T_LAYOUT Matrix layout, see Layout.h from NeoPixelBus library.
 */
template <typename T_LAYOUT> class LedMatrixLayout : public LedMatrixTopology
{
public:

    /**
     * Constructs the matrix layout object.
     * 
     * @param[in] width     Matrix width in pixels
     * @param[in] height    Matrix height in pixels
     */
    LedMatrixLayout(uint16_t width, uint16_t height) :
        LedMatrixTopology(width, height)
    {
    }

    /**
     * Destroys the matrix layout object.
     */
    ~LedMatrixLayout()
    {
    }

    /**
     * Map the framebuffer coordinates to the matrix position, depended on
     * its physical layout. If the given coordinates are outside the defined
     * with/height, they will be limited accordingly.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * 
     * @return Index position in the matrix
     */
    uint16_t map(int16_t x, int16_t y) const override
    {   
        if (x >= m_width)
        {
            x = m_width - 1;
        }
        else if (x < 0)
        {
            x = 0;
        }
        if (y >= m_height)
        {
            y = m_height - 1;
        }
        else if (y < 0)
        {
            y = 0;
        }
        return T_LAYOUT::Map(m_width, m_height, x, y);
    }

    /**
     * Map the framebuffer coordinates to the matrix position, depended on
     * its physical layout. If the given coordinates are outside the defined
     * with/height, the returned position counts width * height.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * 
     * @return Index position in the matrix
     */
    uint16_t mapProbe(int16_t x, int16_t y) const override
    {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        {
            return m_width  * m_height; // count, out of bounds
        }
        return T_LAYOUT::Map(m_width, m_height, x, y);
    }

private:

    LedMatrixLayout();

};

/**
 * Specific LED matrix.
 */
class LedMatrix : public IGfx
{
public:

    /**
     * Get LED matrix instance.
     *
     * @return LED matrix
     */
    static LedMatrix& getInstance()
    {
        return m_instance;
    }

    /**
     * Initialize base driver for the LED matrix.
     *
     * @return If successful, returns true otherwise false.
     */
    bool begin()
    {
        m_strip.Begin();
        m_strip.Show();

        return true;
    }

    /**
     * Show internal framebuffer on physical LED matrix.
     */
    void show()
    {
        m_strip.Show();
        return;
    }

    /**
     * LED matrix is ready, when the last physical pixel update is finished.
     *
     * @return If ready for another update via show(), it will return true otherwise false.
     */
    bool isReady() const
    {
        return m_strip.CanShow();
    }

    /**
     * Set brightness from 0 to 255.
     *
     * @param[in] brightness    Brightness value [0; 255]
     */
    void setBrightness(uint8_t brightness)
    {
        /* To protect the the electronic parts, the brigntness will be scaled down
         * according to the max. supply current.
         */
        const uint8_t SAFE_BRIGHTNESS =
            (Board::LedMatrix::supplyCurrentMax * brightness) /
            (Board::LedMatrix::maxCurrentPerLed * Board::LedMatrix::width *Board::LedMatrix::height);

        m_strip.SetBrightness(SAFE_BRIGHTNESS);
        return;
    }

    /**
     * Clear LED matrix.
     */
    void clear()
    {
        m_strip.ClearTo(ColorDef::BLACK);
        return;
    }

    /**
     * Get pixel color at given position.
     *
     * @param[in] x x-coordinate
     * @param[in] y y-coordinate
     *
     * @return Color in RGB888 format.
     */
    Color getColor(int16_t x, int16_t y) const;

    /** List of possible physical matrix layouts. */
    enum Topology
    {
        TOPOLOGY_ROW_MAJOR = 0,                 /**< Row major layout */
        TOPOLOGY_ROW_MAJOR_90,                  /**< Row major 90 degree turned layout */
        TOPOLOGY_ROW_MAJOR_180,                 /**< Row major 180 degree turned layout */
        TOPOLOGY_ROW_MAJOR_270,                 /**< Row major 270 degree turned layout */
        TOPOLOGY_ROW_MAJOR_ALTERNATING,         /**< Row major alternating layout */
        TOPOLOGY_ROW_MAJOR_ALTERNATING_90,      /**< Row major alternating 90 degree turned layout */
        TOPOLOGY_ROW_MAJOR_ALTERNATING_180,     /**< Row major alternating 180 degree turned layout */
        TOPOLOGY_ROW_MAJOR_ALTERNATING_270,     /**< Row major alternating 270 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR,                  /**< Column major layout */
        TOPOLOGY_COLUMN_MAJOR_90,               /**< Column major 90 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR_180,              /**< Column major 180 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR_270,              /**< Column major 270 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR_ALTERNATING,      /**< Column major alternating layout */
        TOPOLOGY_COLUMN_MAJOR_ALTERNATING_90,   /**< Column major alternating 90 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR_ALTERNATING_180,  /**< Column major alternating 180 degree turned layout */
        TOPOLOGY_COLUMN_MAJOR_ALTERNATING_270,  /**< Column major alternating 270 degree turned layout */
        TOPOLOGY_MAX                            /**< Max. number of topologies. */
    };

    /**
     * Get current configured physical matrix layout.
     * 
     * @return Topology
     */
    Topology getLayout() const
    {
        return m_topology;
    }

    /**
     * Configure physical matrix layout.
     * 
     * @param[in] topo  Topology
     */
    void setLayout(Topology topo)
    {
        if (TOPOLOGY_MAX > topo)
        {
            createLayout(topo);
        }
    }

private:

    /** LedMatrix instance */
    static LedMatrix    m_instance;

    /** Physical matrix layout */
    Topology                                                m_topology;

    /** Pixel representation of the LED matrix */
    NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>  m_strip;

    /**
     * The matrix topology is used to transform from a 2d-matrix coordinate system
     * to the LED string array position.
     */
    LedMatrixTopology*                                      m_topo;

    /**
     * Construct LED matrix.
     */
    LedMatrix();

    /**
     * Destroys LED matrix.
     */
    ~LedMatrix();

    LedMatrix(const LedMatrix& matrix);
    LedMatrix& operator=(const LedMatrix& matrix);

    /**
     * Draw a single pixel in the matrix.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * @param[in] color Pixel color in RGB888 format
     */
    void drawPixel(int16_t x, int16_t y, const Color& color)
    {
        if ((0 <= x) &&
            (Board::LedMatrix::width > x) &&
            (0 <= y) &&
            (Board::LedMatrix::height > y) &&
            (nullptr != m_topo))
        {
            HtmlColor htmlColor = static_cast<uint32_t>(color);

            m_strip.SetPixelColor(m_topo->map(x, y), htmlColor);
        }

        return;
    }

    /**
     * Dim color to black.
     * A dim ratio of 0 means no change.
     * 
     * Note, the base colors may be destroyed, depends on the color type.
     *
     * @param[in] x     x-coordinate
     * @param[in] y     y-coordinate
     * @param[in] ratio Dim ration [0; 255]
     */
    void dimPixel(int16_t x, int16_t y, uint8_t ratio)
    {
        if ((0 <= x) &&
            (Board::LedMatrix::width > x) &&
            (0 <= y) &&
            (Board::LedMatrix::height > y) &&
            (nullptr != m_topo))
        {
            RgbColor rgbColor = m_strip.GetPixelColor(m_topo->map(x, y)).Dim(UINT8_MAX - ratio);

            m_strip.SetPixelColor(m_topo->map(x, y), rgbColor);
        }

        return;
    }

    /**
     * Create physical matrix layout.
     * 
     * @param[in] topo  Topology
     */
    void createLayout(Topology topo);
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif  /* __LEDMATRIX_H__ */

/** @} */