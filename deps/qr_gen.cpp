///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "qr_gen.hpp"

#include "libbmp/CPP/libbmp.h"

#include "QR-Image-embedded/QR_Encode.h"

void run_qr_gen(const char* data, std::string filename)
{
    uint8_t encoded[MAX_BITDATA];
    int width = EncodeData(QR_LEVEL_L, 0, QR_MaskAuto, data, 0, encoded);
    int size = ((width*width)/8) + (((width*width)%8)?1:0);

    BmpImg image(width, width);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < width; y++)
        {
            int byte = (x * width + y) / 8;
            int bit = (x * width + y) % 8;
            int value = encoded[byte] & (0x80 >> bit);

            if (value)
            {
                image.set_pixel(x, y, 0, 0, 0);
            }
            else
            {
                image.set_pixel(x, y, 255, 255, 255);
            }
        }
    }

    image.write(filename);
}
