//
//  OLEDBuffer.c
//
//  Created by Dan LaBove on 4/22/16.
//

#include "OLEDBuffer.h"

unsigned char DISPLAY_BUFFER[BUFFER_COLUMN * BUFFER_LINE] = {ALL_BLACK};

int update_OLED(unsigned char *buffer)
{
    unsigned int i = 0;
    unsigned char *char_pointer;
    
    char_pointer = buffer;
    
    for (i = 0; i < BUFFER_COLUMN*BUFFER_LINE; i++)
    {
        Xil_Out32(OLED_BASE+(i), *char_pointer);
        char_pointer++;
    }

    for (i = 0; i <= DELAY; i++)
    {
        Xil_Out32(OLED_BASE+(64), 1);
    }

    for (i = 0; i <= DELAY; i++)
    {
        Xil_Out32(OLED_BASE+(64), 0);
    }

    return (1);
}


void OLEDClearBuffer(void)
{
    int i;
    for (i = 0; i < BUFFER_COLUMN*BUFFER_LINE; i++)
    {
//        DISPLAY_BUFFER[i] = ALL_BLACK;
    	DISPLAY_BUFFER[i] = ALL_WHITE;
    }
    update_OLED(DISPLAY_BUFFER);
}

void OLEDClearSelector(void)
{
    int i;
    for (i = 0; i < BUFFER_COLUMN; i++)
    {
        DISPLAY_BUFFER[i] &= 0x0F;
    }
    update_OLED(DISPLAY_BUFFER);
    
}

void OLEDUpdateSelectedFrequencyBand(column)
{
    //First, clear selector without writing to buffer
    int i;
    for (i = 0; i < BUFFER_COLUMN; i++)
    {
        DISPLAY_BUFFER[i] &= 0x0F;
    }
    
    int freq_column = NUM_BANDS - (column - 1); //Inverses column to match how the bytes are written to screen
    
    int col_start = ((freq_column - 1)*(OLED_LENGTH/NUM_BANDS));
    
    if (freq_column == 1)
    {
        col_start = 1;
    }
    
    int col_stop = (freq_column*OLED_LENGTH/NUM_BANDS) - 2;
    int col;
    for (col = col_start; col <= col_stop; col++)
    {
        DISPLAY_BUFFER[col] |= FREQ_COLUMN_BYTE; //doesn't touch bottom 5 bits in order to preserve any data for frequency band height
    }
    
    update_OLED(DISPLAY_BUFFER);

}

void OLEDUpdateFrequencyBandHeight(column, height)
{
    //First, clear selector without writing to buffer
    int i;
    for (i = 0; i < BUFFER_COLUMN; i++)
    {
        DISPLAY_BUFFER[i] &= 0x0F;
    }
    
    int freq_column = NUM_BANDS - (column - 1);  //Inverses column to match how the bytes are written to screen
    
    int col_start = ((freq_column - 1)*(OLED_LENGTH/NUM_BANDS));
    
    if (freq_column == 1)
    {
        col_start = 1;
    }
    
    int col_stop = (freq_column*OLED_LENGTH/NUM_BANDS) - 2;
    int col;
    for (col = col_start; col <= col_stop; col++)
    {
        DISPLAY_BUFFER[col] |= FREQ_COLUMN_BYTE; //doesn't touch bottom 5 bits in order to preserve any data for frequency band height
    }
    
    if (height > COLUMN_HEIGHT)
    {
        height = COLUMN_HEIGHT;
    }
    
    int line_stop = height/8; //find how many buffer lines will be written to
    
    if (height % 8 != 0)
    {
        line_stop++;
    }
    
    int line;
    for (line = BUFFER_LINE; line > (BUFFER_LINE - line_stop); line--) //start at buffer line 4 because it corresponds to lowest part of screen where frequency band begins
    {
        unsigned char data_byte = 0x01;
        
        int boundary_check = (height - BYTE_LENGTH*(BUFFER_LINE - (line - 1))); //check if height to write to is higher than what the present buffer line can write to
        
        if (boundary_check >= 0) //if so, all bits in this section are going to be HIGH
        {
            data_byte = 0xFF;
        }
        else //take the remainder heights and set those bits HIGH while leaving those above height low
        {
            if (line == 1)
            {
                data_byte = FREQ_COLUMN_BYTE + ((data_byte << (height % BYTE_LENGTH)) - 1);
            }
            else
            {
                data_byte = (data_byte << (height % BYTE_LENGTH)) - 1;
            }
        }
        for (col = col_start; col <= col_stop; col++) //all bits in this section of frequency band will be uniform
        {
            DISPLAY_BUFFER[col + (BUFFER_COLUMN*(line - 1))] = data_byte;
        }
    }
    
    update_OLED(DISPLAY_BUFFER); 
}

void OLEDUpdateFrequencyBands(int heights[NUM_BANDS])
{
    /* Written for display mode one, max height is size of screen (32), column selector is disabled */
    
    //First, clear buffer but without writing to OLED
    int i;
    for (i = 0; i < BUFFER_COLUMN*BUFFER_LINE; i++)
    {
        DISPLAY_BUFFER[i] = ALL_BLACK;
    }
    
    for (i = 1; i <= NUM_BANDS; i++)
    {
        int col_start = ((i - 1)*(OLED_LENGTH/NUM_BANDS));
        if (i == 1)
        {
            col_start = 1;
        }
        int col_stop = (i*OLED_LENGTH/NUM_BANDS) - 2;
        
        int height = heights[NUM_BANDS - i]; //reverse the order
        if (height > OLED_HEIGHT)
        {
            height = OLED_HEIGHT;
        }
        int line_stop = height/8; //find how many buffer lines will be written to
        if (height % 8 != 0)
        {
            line_stop++;
        }
        int line;
        int col;
        
        for (line = BUFFER_LINE; line > (BUFFER_LINE - line_stop); line--)
        {
            /* Determine how many bits to set high for each line in each frequency band */
            unsigned char data_byte = 0x01; //initialize data to set the first pixel high (1)
            
            int boundary_check = (height - BYTE_LENGTH*(BUFFER_LINE - (line - 1))); //check if height to write to is higher than what the present buffer line can write to
            
            if (boundary_check >= 0) //if so, all bits in this section are going to be HIGH
            {
                data_byte = 0xFF;
            }
            else //take the remainder of height divided by 8 and set those bits HIGH while leaving those above height low
            {
                data_byte = (data_byte << (height % BYTE_LENGTH)) - 1;
            }
    
            for (col = col_start; col <= col_stop; col++) //all bits in this section of frequency band will be uniform
            {
                DISPLAY_BUFFER[col + (BUFFER_COLUMN*(line - 1))] = data_byte;
            }
        }
    }
    
    update_OLED(DISPLAY_BUFFER);
}
