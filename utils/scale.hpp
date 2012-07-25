/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Product name: redemption, a FLOSS RDP proxy
 *   Copyright (C) Wallix 2010-2012
 *   Author(s): Christophe Grosjean, Dominique Lafages, Jonathan Poelen
 */

#if !defined(__UTILS_SCALE_HPP__)
#define __UTILS_SCALE_HPP__

/**
 * \brief Scaling with Bresenham
 */
inline void scale_data(uint8_t *dest, uint8_t *src,
                       unsigned int dest_width, unsigned int src_width,
                       unsigned int dest_height, unsigned int src_height,
                       unsigned int src_rowsize)
{
    unsigned int y_pixels = dest_height;
    unsigned int y_int_part = src_height / dest_height * src_rowsize;
    unsigned int y_fract_part = src_height % dest_height;
    unsigned int yE = 0;
    unsigned int x_int_part = src_width / dest_width * 3;
    unsigned int x_fract_part = src_width % dest_width;

    while (y_pixels-- > 0)
    {
        unsigned int xE = 0;
        uint8_t * x_src = src;
        unsigned int x_pixels = dest_width;
        while (x_pixels-- > 0)
        {
            *dest++ = *x_src;
            *dest++ = x_src[1];
            *dest++ = x_src[2];
            x_src += x_int_part;
            xE += x_fract_part;
            if (xE >= dest_width)
            {
                xE -= dest_width;
                x_src += 3;
            }
        }
        src += y_int_part;
        yE += y_fract_part;
        if (yE >= dest_height)
        {
            yE -= dest_height;
            src += src_rowsize;
        }
    }
}

/**
 * \code
 * const unsigned int dest_width = 260;
 * const unsigned int dest_height = 160;
 *
 * dump_png24(fopen("/tmp/scaling.png", "w+"),
 *            AutoScale(drawable.data,
 *                      dest_width, drawable.width,
 *                      dest_height, drawable.height,
 *                      drawable.rowsize
 *                     ).data(),
 *            dest_width, dest_height,
 *            dest_width * 3);
 * \endcode
 */
class AutoScale
{
    static const uint Bpp = 3;

    uint8_t * _data;

public:
    AutoScale(uint8_t *src,
              unsigned int dest_width, unsigned int src_width,
              unsigned int dest_height, unsigned int src_height,
              unsigned int src_rowsize)
    : _data((uint8_t*)malloc(dest_width * dest_height * Bpp))
    {
        scale_data(this->_data, src,
                   dest_width, src_width,
                   dest_height, src_height,
                   src_rowsize);
    }

    ~AutoScale()
    {
        free(this->_data);
    }

    const uint8_t* data() const
    {
        return this->_data;
    }
};

#endif
