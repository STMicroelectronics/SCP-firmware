/*
 * Arm SCP/MCP Software
 * Copyright (c) 2024, STMicroelectronics and the Contributors. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Interface SCP-firmware console module with tfm console resources.
 */

#include <fwk_assert.h>
#include <fwk_attributes.h>
#include <fwk_event.h>
#include <fwk_log.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_status.h>

#include <stdbool.h>
#include <stddef.h>

#include <uart_stdout.h>
static int mod_console_init(
    fwk_id_t module_id,
    unsigned int element_count,
    const void *data)
{
    return FWK_SUCCESS;
}

static int mod_console_element_init(
    fwk_id_t element_id,
    unsigned int unused,
    const void *data)
{
    return FWK_SUCCESS;
}


static int mod_console_io_open(const struct fwk_io_stream *stream)
{
    return FWK_SUCCESS;
}

static int mod_console_io_getch(
    const struct fwk_io_stream *restrict stream,
    char *restrict ch)
{
    *ch =  'A';

    return FWK_SUCCESS;
}

static int mod_console_io_putch(const struct fwk_io_stream *stream, char ch)
{
    unsigned char c[2];
    c[0] = (char)ch;
    c[1] = 0;
    stdio_output_string(c, 1);
    return FWK_SUCCESS;
}

static int mod_console_close(const struct fwk_io_stream *stream)
{
    return FWK_SUCCESS;
}

const struct fwk_module module_tfm_console = {
    .type = FWK_MODULE_TYPE_DRIVER,

    .init = mod_console_init,
    .element_init = mod_console_element_init,

    .adapter =
        (struct fwk_io_adapter){
            .open = mod_console_io_open,
            .getch = mod_console_io_getch,
            .putch = mod_console_io_putch,
            .close = mod_console_close,
        },
};

struct fwk_module_config config_tfm_console = {
    .elements = FWK_MODULE_STATIC_ELEMENTS({
        [0] = {
            .name = "printf",
            .data = (void *)1
        },
        [1] = { 0 },
    }),
};
