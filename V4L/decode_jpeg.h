#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

void decoder_mjpeg_decode(unsigned char *in_buf, unsigned char *out_buf, int buf_size);
void decoder_jpeg_decompress(unsigned char *out_buf, unsigned char *jpeg_buf, int jpeg_size);

#ifdef __cplusplus
}
#endif
