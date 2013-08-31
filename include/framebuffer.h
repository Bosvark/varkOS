#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

typedef struct __attribute__ ((aligned (16))){
    uint32_t physical_width;
    uint32_t physical_height;
    uint32_t virt_width;
    uint32_t virt_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x;
    uint32_t y;
    uint32_t *gpu_ptr;
    uint32_t size;
}FramebufferInfo;

uint32_t FramebufferInit(FramebufferInfo *fb_info);
void console_write(const char *msg);

#endif // _FRAMEBUFFER_H_

