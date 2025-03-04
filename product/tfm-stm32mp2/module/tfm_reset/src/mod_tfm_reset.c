/*
 * Arm SCP/MCP Software
 * Copyright (c) 2022-2023, Linaro Limited and Contributors. All rights
 * reserved.
 * Copyright (c) 2024, STMicroelectronics and the Contributors. All
 * rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Interface SCP-firmware reset module with tfm reset resources.
 */

#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_log.h>
#include <inttypes.h>
#include <mod_tfm_reset.h>
#include <mod_reset_domain.h>

#include <reset.h>

#include <stdbool.h>

#define MOD_NAME "[SCMI RESET] "

#define TIMEOUT_US_1MS 1000

/* Device context */
struct tfm_reset_dev_ctx {
    struct reset_control *rstctrl;
};

/* Module context */
struct tfm_reset_ctx {
    struct tfm_reset_dev_ctx *dev_ctx_table;
    unsigned int dev_count;
};

static struct tfm_reset_ctx module_ctx;

static bool is_exposed(struct tfm_reset_dev_ctx *ctx)
{
    return ctx->rstctrl != NULL;
}

/*
 * Driver API functions
 */
static int reset_set_state(fwk_id_t dev_id, enum mod_reset_domain_mode mode,
                           uint32_t reset_state, uintptr_t cookie)
{
    struct tfm_reset_dev_ctx *ctx = NULL;
    int status = FWK_SUCCESS;

    if (!fwk_module_is_valid_element_id(dev_id)) {
        return FWK_E_PARAM;
    }

    ctx = &module_ctx.dev_ctx_table[fwk_id_get_element_idx(dev_id)];

    if (!is_exposed(ctx)) {
        return FWK_E_ACCESS;
    }

    /* Whatever the reset_state set, we consider a unique context loss mode */
    if (reset_state) {
        FWK_LOG_INFO(MOD_NAME "Override requested SCMI reset state %#"PRIx32,
            reset_state);
    }

    switch (mode) {
    case MOD_RESET_DOMAIN_MODE_EXPLICIT_ASSERT:
        FWK_LOG_DEBUG(
            MOD_NAME "SCMI reset %u: assert(%s%d)",
            fwk_id_get_element_idx(dev_id),
            ctx->rstctrl->dev->name, ctx->rstctrl->id);

        if (reset_control_assert(ctx->rstctrl)) {
            status = FWK_E_DEVICE;
        }
        break;
    case MOD_RESET_DOMAIN_MODE_EXPLICIT_DEASSERT:
        FWK_LOG_DEBUG(
            MOD_NAME "SCMI reset %u: deassert(%s%d)",
            fwk_id_get_element_idx(dev_id),
            ctx->rstctrl->dev->name, ctx->rstctrl->id);

        if (reset_control_deassert(ctx->rstctrl)) {
            status = FWK_E_DEVICE;
        }
        break;
    case MOD_RESET_DOMAIN_AUTO_RESET:
        FWK_LOG_DEBUG(
            MOD_NAME "SCMI reset %u: cycle(%s)",
            fwk_id_get_element_idx(dev_id),
            fwk_id_get_element_idx(dev_id),
            ctx->rstctrl->dev->name, ctx->rstctrl->id);
;

        if (reset_control_reset(ctx->rstctrl)) {
            status = FWK_E_TIMEOUT;
        }
        if (reset_control_reset(ctx->rstctrl)) {
            status = FWK_E_TIMEOUT;
        }
        break;
    default:
        return FWK_E_PARAM;
    }

    return status;
}

static const struct mod_reset_domain_drv_api api_tfm_reset = {
    .set_reset_state = reset_set_state,
};

/*
 * Framework handler functions
 */

static int tfm_reset_init(fwk_id_t module_id, unsigned int element_count,
                            const void *data)
{
    module_ctx.dev_count = element_count;

    if (element_count == 0) {
        return FWK_SUCCESS;
    }

    module_ctx.dev_ctx_table = fwk_mm_calloc(element_count,
                                             sizeof(struct tfm_reset_dev_ctx));

    return FWK_SUCCESS;
}

static int tfm_reset_element_init(fwk_id_t element_id,
                                    unsigned int unused __unused,
                                    const void *data)
{
    struct tfm_reset_dev_ctx *ctx = NULL;
    const struct mod_tfm_reset_dev_config *config = NULL;

    if (!fwk_module_is_valid_element_id(element_id)) {
        return FWK_E_PARAM;
    }

    ctx = &module_ctx.dev_ctx_table[fwk_id_get_element_idx(element_id)];
    config = (const struct mod_tfm_reset_dev_config *)data;

    ctx->rstctrl = (struct reset_control *)config->rstctrl;

    return FWK_SUCCESS;
}

static int tfm_reset_process_bind_request(fwk_id_t requester_id, fwk_id_t id,
                                            fwk_id_t api_type, const void **api)
{
    *api = &api_tfm_reset;

    return FWK_SUCCESS;
}

const struct fwk_module module_tfm_reset = {
    .type = FWK_MODULE_TYPE_DRIVER,
    .api_count = 1,
    .init = tfm_reset_init,
    .element_init = tfm_reset_element_init,
    .process_bind_request = tfm_reset_process_bind_request,
};
