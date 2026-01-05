/* fast_mem.c */
#include <stdint.h>
#include <stddef.h>

/* 针对 Cortex-M7 的优化拷贝，假设地址 32 位对齐（LVGL 通常满足） */
void fast_memcpy_m7(void *dest, const void *src, size_t n) {
    uint32_t *d32 = (uint32_t *)dest;
    const uint32_t *s32 = (const uint32_t *)src;

    // 检查对齐
    if ((((uint32_t)dest | (uint32_t)src) & 0x03) == 0) {
        // 展开循环，每次拷贝 32 字节 (8个字)
        while (n >= 32) {
            d32 = s32; d32 = s32; d32 = s32; d32 = s32;
            d32 = s32; d32 = s32; d32 = s32; d32 = s32;
            d32 += 8; s32 += 8;
            n -= 32;
        }
        // 处理剩余的字
        while (n >= 4) {
            *d32++ = *s32++;
            n -= 4;
        }
    }
    
    // 处理剩余的字节（或处理不对齐情况）
    uint8_t *d8 = (uint8_t *)d32;
    const uint8_t *s8 = (const uint8_t *)s32;
    while (n--) {
        *d8++ = *s8++;
    }
}