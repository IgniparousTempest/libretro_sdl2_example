#include "libretro.hpp"
#include "game.hpp"
#include <cstring>
#include <memory>
#include <iostream>

static const unsigned FRAMEBUFFER_WIDTH = 1920;
static const unsigned FRAMEBUFFER_HEIGHT = 1080;

std::unique_ptr<Game> game;

// Callbacks
static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static struct retro_hw_render_callback hw_render;

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_cheat_reset(void)
{

}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{

}

static void audio_callback(void)
{
}

static void audio_set_state(bool enable)
{
}

static void context_reset(void)
{
    fprintf(stderr, "Context reset!\n");
    //TODO: rglgen_resolve_symbols(hw_render.get_proc_address);
}

static void context_destroy(void)
{
    fprintf(stderr, "Context destroy!\n");
}

bool retro_load_game(const struct retro_game_info *info)
{
    if (USE_HARDWARE) {
        hw_render.context_type = RETRO_HW_CONTEXT_OPENGL;
        hw_render.context_reset = context_reset;
        hw_render.context_destroy = context_destroy;
        hw_render.depth = true;
        hw_render.bottom_left_origin = true;
        if (!environ_cb(RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render)) {
            fprintf(stderr, "HW Context could not be initialized, exiting...\n");
            return false;
        }
    }
    game = std::make_unique<Game>(FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    return false;
}

void retro_unload_game(void)
{

}

unsigned retro_get_region(void)
{
    return RETRO_REGION_NTSC;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{

}

void *retro_get_memory_data(unsigned id)
{
    return nullptr;
}

size_t retro_get_memory_size(unsigned id)
{
    return 0;
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    return false;
}

void retro_deinit(void)
{
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    // Start without rom
    bool no_rom = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }



void retro_init(void)
{
}

void retro_get_system_info(struct retro_system_info *info)
{
    memset(info, 0, sizeof(*info));
    info->library_name = "Game";
    info->library_version = "0.9.0";
    info->need_fullpath = false;
    info->valid_extensions = "zip";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{

    int pixel_format = RETRO_PIXEL_FORMAT_XRGB8888;

    memset(info, 0, sizeof(*info));
    info->timing.fps            = 60;
    info->timing.sample_rate    = 44100;
    info->geometry.base_width   = FRAMEBUFFER_WIDTH;
    info->geometry.base_height  = FRAMEBUFFER_HEIGHT;
    info->geometry.max_width    = FRAMEBUFFER_WIDTH;
    info->geometry.max_height   = FRAMEBUFFER_HEIGHT;
    info->geometry.aspect_ratio = (float) FRAMEBUFFER_WIDTH / (float) FRAMEBUFFER_HEIGHT;

    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &pixel_format);
}

void retro_reset(void) { }

void retro_run(void)
{
    if (USE_HARDWARE) {
        hw_render.get_current_framebuffer = reinterpret_cast<retro_hw_get_current_framebuffer_t>(game->GetFrameBuffer());
        video_cb(RETRO_HW_FRAME_BUFFER_VALID, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, sizeof(uint32_t) * FRAMEBUFFER_WIDTH);
    } else {
        video_cb(game->GetFrameBuffer(), FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, sizeof(uint32_t) * FRAMEBUFFER_WIDTH);
    }
}