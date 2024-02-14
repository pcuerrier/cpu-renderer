#include "gtest/gtest.h"
#include "display.h"

TEST(Display, initialize_window)
{
    SDL_API sdl = initialize_window();

    EXPECT_NE(sdl.window, nullptr);
    EXPECT_NE(sdl.renderer, nullptr);
    EXPECT_FALSE(sdl.fullscreen);
    EXPECT_TRUE(sdl.is_running);

    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

TEST(Display, destroy_window)
{
    SDL_API sdl = {};
    SDL_Init(SDL_INIT_VIDEO);
    sdl.window = SDL_CreateWindow("test-CPU-Renderer", 100, 100, 0x0);
    sdl.renderer = SDL_CreateRenderer(sdl.window, 0, 0);

    destroy_window(sdl);

    EXPECT_NE(sdl.window, nullptr);
    EXPECT_NE(sdl.renderer, nullptr);

    SDL_Quit();
}

TEST(Display, clear_color_buffer)
{
    ColorBuffer color_buffer = {};
    color_buffer.width = 10;
    color_buffer.height = 10;
    uint32_t size = color_buffer.width * color_buffer.height;
    color_buffer.memory = (uint32_t*)malloc(4 * size);
    clear_color_buffer(color_buffer, 0xFF000000); // Pure Black
    for (uint32_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(color_buffer.memory[i], 0xFF000000);
    }
    clear_color_buffer(color_buffer, 0xFFFFFFFF); // Pure White
    for (uint32_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(color_buffer.memory[i], 0xFFFFFFFF);
    }
}

TEST(Display, create_color_buffer)
{
    SDL_API sdl = {};
    SDL_Init(SDL_INIT_VIDEO);
    sdl.window = SDL_CreateWindow("test-CPU-Renderer", 100, 100, 0x0);
    sdl.renderer = SDL_CreateRenderer(sdl.window, 0, 0);

    ColorBuffer color_buffer = {};
    create_color_buffer(sdl, color_buffer);
    EXPECT_EQ(color_buffer.width, 100);
    EXPECT_EQ(color_buffer.height, 100);
    EXPECT_NE(color_buffer.memory, nullptr);
    EXPECT_NE(color_buffer.texture, nullptr);

    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

TEST(Display, resize_color_buffer)
{
    SDL_API sdl = {};
    SDL_Init(SDL_INIT_VIDEO);
    sdl.window = SDL_CreateWindow("test-CPU-Renderer", 100, 100, 0x0);
    sdl.renderer = SDL_CreateRenderer(sdl.window, 0, 0);

    ColorBuffer color_buffer = {};
    color_buffer.width = 10;
    color_buffer.height = 10;
    uint32_t size = color_buffer.width * color_buffer.height;
    color_buffer.memory = (uint32_t*)malloc(4 * size);
    resize_color_buffer(sdl, color_buffer, 100, 100);

    EXPECT_EQ(color_buffer.width, 100);
    EXPECT_EQ(color_buffer.height, 100);
    EXPECT_NE(color_buffer.memory, nullptr);
    EXPECT_NE(color_buffer.texture, nullptr);

    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

TEST(Display, destroy_color_buffer)
{
    SDL_API sdl = {};
    SDL_Init(SDL_INIT_VIDEO);
    sdl.window = SDL_CreateWindow("test-CPU-Renderer", 100, 100, 0x0);
    sdl.renderer = SDL_CreateRenderer(sdl.window, 0, 0);

    ColorBuffer color_buffer = {};
    color_buffer.width = 10;
    color_buffer.height = 10;
    uint32_t size = color_buffer.width * color_buffer.height;
    color_buffer.memory = (uint32_t*)malloc(4 * size);
    color_buffer.texture = SDL_CreateTexture(sdl.renderer, 0, 0, 10, 10);
    destroy_color_buffer(color_buffer);

    EXPECT_EQ(color_buffer.width, 0);
    EXPECT_EQ(color_buffer.height, 0);
    EXPECT_EQ(color_buffer.memory, nullptr);
    //EXPECT_EQ(color_buffer.texture, nullptr);

    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}

TEST(Display, draw_pixel)
{
    ColorBuffer color_buffer = {};
    color_buffer.width = 10;
    color_buffer.height = 10;
    uint32_t size = color_buffer.width * color_buffer.height;

    // Allocate buffer and sets memory to 0
    color_buffer.memory = (uint32_t*)calloc(4 * size, sizeof(uint32_t));

    draw_pixel(color_buffer, 0, 0, 0xFFFFFFFF);
    draw_pixel(color_buffer, 5, 8, 0xFFFFFFFF);

    for (uint32_t i = 0; i < size; ++i)
    {
        if (i == 0) // First test
        {
            EXPECT_EQ(color_buffer.memory[i], 0xFFFFFFFF);
        }
        else if (i == 85) // Second test
        {
            EXPECT_EQ(color_buffer.memory[i], 0xFFFFFFFF);
        }
        else
        {
            EXPECT_EQ(color_buffer.memory[i], 0x00000000);
        }
    }
    draw_pixel(color_buffer, 10, 10, 0xFFFFFFFF); // Out-of-bound
    EXPECT_TRUE(true); // Should not have crashed before

}

TEST(Display, draw_grid)
{
    ColorBuffer color_buffer = {};
    uint32_t pitch = 10;
    color_buffer.width = pitch;
    color_buffer.height = pitch;
    uint32_t size = color_buffer.width * color_buffer.height;

    // Allocate buffer and sets memory to 0
    color_buffer.memory = (uint32_t*)calloc(4 * size, sizeof(uint32_t));

    // Shouldn't infinite loop or crash
    draw_grid(color_buffer, 0, 0xFFFFFFFF);

    for (uint32_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(color_buffer.memory[i], 0x00000000);
    }
    
    draw_grid(color_buffer, 5, 0xFFFFFFFF);
    for (uint32_t i = 0; i < pitch; i += 5)
    {
        for (uint32_t j = 0; j < pitch; j += 5)
        {
            ASSERT_EQ(color_buffer.memory[i * pitch + j], 0xFFFFFFFF);
        }
    }
}

TEST(Display, draw_rect)
{
    ColorBuffer color_buffer = {};
    uint32_t pitch = 10;
    color_buffer.width = pitch;
    color_buffer.height = pitch;
    uint32_t size = color_buffer.width * color_buffer.height;

    // Allocate buffer and sets memory to 0
    color_buffer.memory = (uint32_t*)calloc(4 * size, sizeof(uint32_t));

    draw_rect(color_buffer, 10, 10, 10, 10, 0xFFFFFFFF); // Out-of-bounds
    for (uint32_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(color_buffer.memory[i], 0x00000000);
    }

    draw_rect(color_buffer, 9, 9, 10, 10, 0xFFFFFFFF); // Out-of-bounds but fill single pixel
    for (uint32_t i = 0; i < size; ++i)
    {
        if (i == 9 * 10 + 9)
        {
            ASSERT_EQ(color_buffer.memory[i], 0xFFFFFFFF);    
        }
        else
        {
            ASSERT_EQ(color_buffer.memory[i], 0x00000000);
        }
    }

    // Reset buffer
    for (uint32_t i = 0; i < size; ++i)
    {
        color_buffer.memory[i] = 0x00000000;
    }
    draw_rect(color_buffer, 0, 0, 11, 11, 0xFFFFFFFF); // Out-of-bounds but fill buffer
    for (uint32_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(color_buffer.memory[i], 0xFFFFFFFF);
    }
    
    // Reset buffer
    for (uint32_t i = 0; i < size; ++i)
    {
        color_buffer.memory[i] = 0x00000000;
    }
    draw_rect(color_buffer, 2, 2, 4, 4, 0xFFFFFFFF);
    for (uint32_t i = 0; i < pitch; i += 5)
    {
        for (uint32_t j = 0; j < pitch; j += 5)
        {
            if (i >= 2 && i < 6 && j >= 2 && j < 6)
            {
                EXPECT_EQ(color_buffer.memory[i * pitch + j], 0xFFFFFFFF);
            }
            else
            {
                EXPECT_EQ(color_buffer.memory[i * pitch + j], 0x00000000)
                    << "I: " << i << ", J: " << j;
            }
        }
    }
}