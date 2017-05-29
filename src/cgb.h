#ifndef __CGB_H__
#define __CGB_H__

// ALLES GROß schreiben.
enum cgb_result
{
    cgb_result_frame_incomplete     = 1,
    cgb_result_ok                   = 0,
    cgb_result_frame_not_found      = -1,
    cgb_result_frame_length_invalid = -2,
    cgb_result_invalid_crc          = -3,
};

typedef unsigned char   cgb_byte;
typedef cgb_byte        cgb_size;

// Maximum 8Bit Datasize to transform
const cgb_size MAX_8BIT_DATA_SIZE = 111; // 128 * 7 / 8;


typedef struct
{
    cgb_byte data   : 7;
    cgb_byte ctrl   : 1;
} cgb_data_byte;

typedef struct 
{
    cgb_size capacity;
    cgb_size size;
    cgb_size head;
    cgb_byte * buffer;
} cgb_buffer;

typedef struct
{
    cgb_byte address;
    cgb_byte size;
    cgb_byte capacity;
    cgb_byte * buffer;    
} cgb_frame;

/**
* @brief 
*
* @param src            byte - buffer
* @param size           Anzahl byte in 'src'
* @param head_index     Index gefundener header
*
* @return 
*/
cgb_result cgb_find_head(cgb_byte * src, cgb_size size,  cgb_size &head_index);

/**
* @brief 
*
* @param src            byte - buffer
* @param size           Anzahl bytes in 'src'
*
* @return 
*/
cgb_result cgb_check_crc(cgb_byte * src, cgb_size size);
cgb_result cgb_encoding( const cgb_byte* pSrc, cgb_byte* pDest, cgb_size n, cgb_size * len);
cgb_result cgb_decoding( const cgb_byte* pSrc, cgb_byte* pDest, cgb_size n, cgb_size * len);

#endif // __CGB_H__
