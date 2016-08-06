#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

unsigned char* mjpeg2jpeg(unsigned char *in_buf, int buf_src_size);
void decoder_jpeg_decompress(unsigned char *out_buf, unsigned char *jpeg_buf, int jpeg_size);

#ifdef __cplusplus
}
#endif
