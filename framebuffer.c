#include <stdint.h>
#include <string.h>
#include "framebuffer.h"
#include "mailbox.h"
#include "uart.h"
#include "klog.h"

// These external names are relevant to the path of the font files
extern uint8_t _binary_fonts_font_mono_8x16_bin_start;
extern uint8_t _binary_fonts_font_mono_8x16_bin_end;
extern uint32_t _binary_fonts_font_mono_8x16_bin_size;

extern uint8_t _binary_icons_Logo_24b_260x194_bin_start;
extern uint8_t _binary_icons_Logo_24b_260x194_bin_end;
extern uint32_t _binary_icons_Logo_24b_260x194_bin_size;

#define FNT_WIDTH	8
#define FNT_HEIGHT	16

// Framebuffer parameters
static uint32_t fb_address;
static uint32_t fb_size_bytes;
static uint32_t fb_x,fb_y,pitch,depth;

// Console parameters
static uint32_t cons_x=1;
static uint32_t cons_y=1;
static uint32_t console_init=0;
//static uint32_t cons_color=(uint32_t)0x00ffffff;


void put_pixel_RGB565(uint32_t x, uint32_t y, uint16_t color);
void put_pixel_RGB24(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);
void disp_char(int8_t cval);
void display_logo(void);

uint32_t FramebufferInit(void)
{
    uint32_t retval=0;
    volatile unsigned int mb[100] __attribute__ ((aligned(16)));
(void)fb_info;

	depth = 24;

/*
    MailboxWrite((uint32_t)(fb_info+0x40000000), 1);

	if((retval = MailboxRead(1)) == 0){
		klogStr("Read FramebufferInfo failed");
		return 0;
	}

//	klogBin("FramebufferInfo - Response:", (uint8_t*)fb_info, sizeof(FramebufferInfo));
	klogInt("FramebufferInfo.physical_width :", fb_info->physical_width);
	klogInt("FramebufferInfo.physical_height:", fb_info->physical_height);
	klogInt("FramebufferInfo.virt_width     :", fb_info->virt_width);
	klogInt("FramebufferInfo.virt_height    :", fb_info->virt_height);
	klogInt("FramebufferInfo.size           :", fb_info->size);
	klogInt("FramebufferInfo.depth          :", fb_info->depth);
	klogInt("FramebufferInfo.pitch          :", fb_info->pitch);
	klogInt("FramebufferInfo.x              :", fb_info->x);
	klogInt("FramebufferInfo.y              :", fb_info->y);
	klogBin("FramebufferInfo.gpu_ptr        :", (uint8_t*)fb_info->gpu_ptr, 4);
*/
//	klogBin("Font0 start:",(uint8_t*)&_binary_fonts_font_mono_8x16_bin_start, _binary_fonts_font_mono_8x16_bin_size);
    /*
     * Buffer contents:
		u32: buffer size in bytes (including the header values, the end tag and padding)
		u32: buffer request/response code
				Request codes:
					0x00000000: process request
				All other values reserved
				Response codes:
					0x80000000: request successful
					0x80000001: error parsing request buffer (partial response)
				All other values reserved
		u8...: sequence of concatenated tags
		u32: 0x0 (end tag)
		u8...: padding

		Tag format:
		u32: tag identifier
		u32: value buffer size in bytes
		u32: 1 bit (MSB) request/response indicator (0=request, 1=response), 31 bits (LSB) value length in bytes
		u8...: value buffer
*/
	//
	// Get physical size
	//
    mb[0] = 8 * 4;		// Buffer size
    mb[1] = 0;			// Request
    mb[2] = 0x00040003;	// Tag id - Get physical width/height
    mb[3] = 2*4;		// Buffer size
    mb[4] = 0;			// Request/Response indicator (MSB 0 for request)
    mb[5] = 0;			// Width
    mb[6] = 0;			// Height
    mb[7] = 0;			// End tag

    klogBin("Get physical size - Request:", (uint8_t*)mb, mb[0]+4);

    MailboxWrite((uint32_t)(mb+0x40000000), 8);

    if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){
		klogStr("Get physical tag failed");
		return 0;
	}
    
    klogBin("Get physical size - Response:", (uint8_t*)mb, mb[0]+4);

    fb_x = mb[5];
    fb_y = mb[6];
    klogInt("Screen width:", fb_x);
    klogInt("Screen height:", fb_y);

    uint32_t mb_pos=1;

    mb[mb_pos++] = 0;			// Request indicator
    mb[mb_pos++] = 0x00048003;	// Tag - Set physical width/height
    mb[mb_pos++] = 2*4;			// Value buffer length - 2 x sizeof(u32)
    mb[mb_pos++] = 2*4;			// Request indicator + value buffer length
    mb[mb_pos++] = fb_x;		// Width
    mb[mb_pos++] = fb_y;		// Height
    mb[mb_pos++] = 0x00048004;	// Tag - Set virtual width/height
	mb[mb_pos++] = 2*4;			// Value buffer length - 2 x sizeof(u32)
	mb[mb_pos++] = 2*4;			// Request indicator + value buffer length
	mb[mb_pos++] = fb_x;		// Width
	mb[mb_pos++] = fb_y;		// Height
	mb[mb_pos++] = 0x00048005;	// Tag - Set depth
	mb[mb_pos++] = 1*4;			// Value buffer length - 2 x sizeof(u32)
	mb[mb_pos++] = 1*4;			// Request indicator + value buffer length
	mb[mb_pos++] = depth;		// Depth
	mb[mb_pos++] = 0x00040001;	// Tag - Allocate buffer
	mb[mb_pos++] = 2*4;			// Value buffer length - 2 x sizeof(u32)
	mb[mb_pos++] = 2*4;			// Request indicator + value buffer length
	mb[mb_pos++] = 16;			// Alignment
	mb[mb_pos++] = 0;			// Place holder for response value
	mb[mb_pos++] = 0;			// End tag
	mb[0] = mb_pos*4;			// Size of the whole buffer

	klogBin("Setup - Request:", (uint8_t*)mb, mb[0]+4);

	MailboxWrite((uint32_t)(mb+0x40000000), 8);

	if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){
		klogStr("Set physical tag failed");
		return 0;
	}

	klogBin("Setup - Response:", (uint8_t*)mb, mb[0]+4);

	mb_pos = 2;
	unsigned int val_buff_len=0;
	while(mb[mb_pos] != 0){
		switch(mb[mb_pos++])
		{
			case 0x00048003:
				val_buff_len = mb[mb_pos++];
				mb_pos+= (val_buff_len/4)+1;
				break;
			case 0x00048004:
				val_buff_len = mb[mb_pos++];
				mb_pos+= (val_buff_len/4)+1;
				break;
			case 0x00048005:
				val_buff_len = mb[mb_pos++];
				mb_pos+= (val_buff_len/4)+1;
				break;
			case 0x00040001:
				val_buff_len = mb[mb_pos++];
				mb_pos++;
				fb_address = mb[mb_pos++];
				fb_size_bytes = mb[mb_pos++];
				break;
		}
	}

	//
	// Get pitch
	//
	mb[0] = 8 * 4;		// Buffer size
	mb[1] = 0;			// Request
	mb[2] = 0x00040008;	// Tag id - Get pitch
	mb[3] = 4;			// Buffer size
	mb[4] = 0;			// Request length
	mb[5] = 0;			// Pitch
	mb[6] = 0;			// End tag
	mb[7] = 0;

	klogBin("Get pitch - Request:", (uint8_t*)mb, mb[0]+4);

	MailboxWrite((uint32_t)(mb+0x40000000), 8);

	if(((retval = MailboxRead(8)) == 0) || (mb[1] != 0x80000000)){
		klogStr("Get pitch tag failed");
		return 0;
	}

	klogBin("Get pitch - Response:", (uint8_t*)mb, mb[0]+4);

	console_init = 1;	// Flag to indicate that console is initialised
	pitch = mb[5];

	klogBin("Framebuffer address: 0x", (uint8_t*)&fb_address, sizeof(fb_address));
	klogInt("Framebuffer size:", fb_size_bytes);
	klogInt("Pitch:", pitch);

	fb_x--;
	fb_y--;
/*
	put_pixel_RGB565(1, 1, 0xf800);				// Top left
	put_pixel_RGB565(fb_x, 1, 0xf800);			// Top right
	put_pixel_RGB565(1, fb_y, 0xf800);			// Bottom left
	put_pixel_RGB565(fb_x, fb_y, 0xf800);		// Bottom right
	put_pixel_RGB565(fb_x/2, fb_y/2, 0xf800);	// Centre
*/

/* Frame
	for(uint32_t j=0;j<16; j++){
		for(uint32_t i=0;i<fb_y; i++){
			switch(depth)
			{
				case 16:put_pixel_RGB565(j, i, 0xf800);break;
				case 24:put_pixel_RGB24(j, i, 0, 0xff, 0);break;
			}
		}
	}

	for(uint32_t j=0;j<16; j++){
		for(uint32_t i=0;i<fb_x; i++){
			switch(depth)
			{
				case 16:put_pixel_RGB565(i, j, 0xf800);break;
				case 24:put_pixel_RGB24(i, j, 0, 0xff, 0);break;
			}
		}
	}

	for(uint32_t j=fb_y-16;j<fb_y; j++){
		for(uint32_t i=fb_x;i; i--){
			switch(depth)
			{
				case 16:put_pixel_RGB565(i, j, 0xf800);break;
				case 24:put_pixel_RGB24(i, j, 0, 0xff, 0);break;
			}
		}
	}

	for(uint32_t j=fb_x-16;j<fb_x; j++){
		for(uint32_t i=0;i<fb_y; i++){
			switch(depth)
			{
				case 16:put_pixel_RGB565(j, i, 0xf800);break;
				case 24:put_pixel_RGB24(j, i, 0, 0xff, 0);break;
			}
		}
	}
*/

	display_logo();

	console_write("Framebuffer initialised!\n");
	console_write("Well done\r\n");
	console_write("This deserves a beer");

	return 1;
}

/*
 * Notes on color formats:
 *
 * Monochrome (2 colors) - 1 bit per pixel: 1 = white, 0 = black
 * Greyscale (256 colors) - 1 byte per pixel: 255 = white, 0 = black
 * 8 Color   (8 colors) - 3 bits per pixel: 1st bit (LSB) red, 2nd bit green, 3rd bit (MSB) blue
 * Low Color (256 colors) - 8 bits per pixel: 1st 3 bits (LSB) red intensity, 2nd 3 bits green intensity, 3rd 2 bits (MSB) blue intensity
 * High color (65 536 colors) - 16 bits per pixel: 1st 5 bits (LSB) red intensity, 2nd 6 bits green intensity, 3rd 5 bits (MSB) blue intensity
 * True Color (16 777 216 colors) - 24 bits per pixel: 1st 8 bits (LSB) red intensity, 2nd 8 bits green intensity, 3rd 8 bits (MSB) blue intensity
 * RGBA32 (16 777 216 colors with 256 levels of transparency) - 32 bits per pixel: 1st 8 bits (LSB) red intensity, 2nd 8 bits green intensity,
 *                                                              3rd 8 bits blue intensity, 4th 8 bits (MSB) transparency level
 */
void put_pixel_RGB565(uint32_t x, uint32_t y, uint16_t color)
{
	volatile uint32_t *ptr=0;
	uint32_t offset=0;

	offset = (y * pitch) + (x*2);
	ptr = (uint32_t*)(fb_address + offset);
	*ptr = (uint32_t)color;
}

void put_pixel_RGB24(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	volatile uint32_t *ptr=0;
	uint32_t offset=0;

	offset = (y * pitch) + (x * 3);
	ptr = (uint32_t*)(fb_address + offset);
	*((uint8_t*)ptr) = red;
	*((uint8_t*)(ptr+1)) = green;
	*((uint8_t*)(ptr+2)) = blue;
}

void disp_char(int8_t cval)
{
	uint8_t *ptr=&_binary_fonts_font_mono_8x16_bin_start;
	uint32_t font_offset=cval*16,row=0,col=0;
	uint8_t r,g,b;
	uint32_t x,y;

	for(y=cons_y;y<cons_y+FNT_HEIGHT;y++){
		col=0;
		for(x=cons_x;x<cons_x+FNT_WIDTH;x++){
			if(*(ptr+font_offset+row)&(1<<col++)){
				r=0xff;
				g=0xff;
				b=0xff;
			}else{
				r=0;
				g=0;
				b=0;
			}

			put_pixel_RGB24(x, y, r, g, b);
		}
		row++;
	}

	cons_x += FNT_WIDTH;
}

void console_write(const char *msg)
{
	if(console_init == 0)
		return;

	for(;*msg;msg++){
		if(*msg == '\r')
			cons_x = 1;
		else if(*msg == '\n')
			cons_y+=FNT_HEIGHT;
		else
			disp_char(*msg);
	}
}

void display_logo(void)
{
	uint8_t *ptr=&_binary_icons_Logo_24b_260x194_bin_start;
	uint8_t r,g,b;
	uint32_t x,y;

	for(y=1;y<=194;y++){
		for(x=fb_x-260;x<fb_x;x++){
			r=*ptr++;
			g=*ptr++;
			b=*ptr++;
			put_pixel_RGB24(x, y, r, g, b);
		}
	}
}
