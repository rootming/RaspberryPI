#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#include <jpeglib.h>
#include <jerror.h>


#define DECODER_DEBUG

#include "huffman.h"

void decoder_jpeg_decompress(unsigned char *jpeg_buf, unsigned char *out_buf, int jpeg_size)

{
    int row_stride;
    unsigned char *output_index;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    /* step 1: allocate and initialize JPEG decompression object */
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    /* step 2: specify data source */
    jpeg_mem_src(&cinfo, jpeg_buf, jpeg_size);

    /* step 3: read file parameter of jpeg */
    (void)jpeg_read_header(&cinfo, TRUE);

    /* step 4: set parameters for decompression(optional) */
    /* output 1/2 scale image
     * cinfo.scale_num = 1;
     * cinfo.scale_denom = 2; */

    /* step 5: start decompressor */
    (void)jpeg_start_decompress(&cinfo);

    /* step 6: scan lines of jpeg file and read data from lines */
    row_stride = cinfo.output_width * cinfo.output_components;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        output_index = out_buf + cinfo.output_scanline * row_stride;

        /* read data from lines of jpeg and put these data to output_buf */
        (void)jpeg_read_scanlines(&cinfo, &output_index, 1);
    }

    /* step 7: finish decompression */
    (void)jpeg_finish_decompress(&cinfo);

    /* step 8: release JPEG decompression object */
    jpeg_destroy_decompress(&cinfo);
}

unsigned char* mjpeg2jpeg(unsigned char *in_buf, int buf_src_size)
{
    int pos = 0;
    int size_start = 0;
    unsigned char *pdeb = in_buf;
    unsigned char *pcur = in_buf;
    unsigned char *plimit = in_buf + buf_src_size;
    unsigned char *jpeg_buf;



    /* find the SOF0(Start Of Frame 0) of JPEG */
    while ( (((pcur[0] << 8) | pcur[1]) != 0xffc0) && (pcur < plimit) ){
        pcur++;
    }

    /* SOF0 of JPEG exist */
    if (pcur < plimit){
#ifdef DECODER_DEBUG
        printf("SOF0 existed at position\n");
#endif
        jpeg_buf = malloc(buf_src_size + sizeof(dht_data) + 10);

        if (jpeg_buf != NULL){
            /* insert huffman table after SOF0 */
            size_start = pcur - pdeb;
            memcpy(jpeg_buf, in_buf, size_start);
            pos += size_start;
            printf("Copy head done.\n");
            memcpy(jpeg_buf + pos, dht_data, sizeof(dht_data));
            pos += sizeof(dht_data);
            printf("Copy dht done.\n");
            memcpy(jpeg_buf + pos, pcur, buf_src_size - size_start);
            pos += buf_src_size - size_start;
            printf("Copy data done.\n");
            //decoder_jpeg_decompress(jpeg_buf, out_buf, pos);
            //printf("decompress done.\n");
            //free(jpeg_buf);
            return jpeg_buf;
        }
    }


}



