#include "cgb.h"
#include <stdio.h>


/*

*/
cgb_result cgb_find_head(cgb_buffer *src)
{
    while(src->head < src->capacity)
    {
        cgb_data_byte * db = (cgb_data_byte*)(src->buffer + src->head);
        if(db->ctrl)
        {
            return cgb_result_ok;
        }
        src->head ++;
    }
    return cgb_result_frame_not_found;
}

/*
    7 Bit to 8 Bit decoding.
*/
cgb_result cgb_decoding(
    const cgb_byte * pSrc, 
    cgb_byte * pDest,
    cgb_size n,
    cgb_size * len)
{
    const cgb_byte data_bits = 7;
    cgb_byte shl = 1;
    *len = 0;

    while(n-- > 0)
    {
        cgb_byte * pD = pDest + (*len)++; 
        *pD = (*pSrc) << shl;
        *pD |= *(++pSrc) >> (data_bits - shl);
       
        if(shl == 1) n--;
 
        if(shl < data_bits)
        {
            shl++;
        }
        else
        {
            shl = 1;
            pSrc++;
        }
    }

    return cgb_result_ok;
}

/*
    8 Bit to 7 Bit encoding.
*/
cgb_result cgb_encoding(
    const cgb_byte * pSrc, 
    cgb_byte * pDest,
    cgb_size n,
    cgb_size * len)
{
    const cgb_byte data_bits = 7;
    const cgb_byte ctrl_mask = (cgb_byte) ~0x80;

    cgb_byte shr = 1;

    *pDest = 0;
    *len = 0;

    while(n-- > 0)
    {
        *pDest++ |= ((*pSrc) >> shr);               
        *pDest = ((*pSrc) << (data_bits - shr)) & ctrl_mask;
        
        pSrc++;
        *len += (shr == 1) ? 2 : 1;
    
        if(shr < data_bits)
        {
            shr++;
        }
        else
        {
            shr = 1;
            pDest++; *pDest = 0;
        }
    }

    return cgb_result_ok;
}

cgb_result cgb_check_crc(const cgb_buffer * src, cgb_frame * dest)
{
    return cgb_result_ok;
}

