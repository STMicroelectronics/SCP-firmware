/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2022, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2024, STMicroelectronics and the Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Interrupt management.
 */

#include <fwk_arch.h>
#include <fwk_status.h>

#include <stdbool.h>
#include <stdint.h>

#include <arch_interrupt.h>

static int global_enable(void)
{
    return FWK_E_SUPPORT;
}

static int global_disable(void)
{
    return FWK_E_SUPPORT;
}

static int is_enabled(unsigned int interrupt, bool *state)
{
    return FWK_E_SUPPORT;
}

static int enable(unsigned int interrupt)
{
    return FWK_E_SUPPORT;
}

static int disable(unsigned int interrupt)
{
    return FWK_E_SUPPORT;
}

static int is_pending(unsigned int interrupt, bool *state)
{
    return FWK_E_SUPPORT;
}

static int set_pending(unsigned int interrupt)
{
    return FWK_E_SUPPORT;
}

static int clear_pending(unsigned int interrupt)
{
    return FWK_E_SUPPORT;
}

static int set_isr_irq(unsigned int interrupt, void (*isr)(void))
{
    return FWK_E_SUPPORT;
}

static int set_isr_irq_param(
    unsigned int interrupt,
    void (*isr)(uintptr_t param),
    uintptr_t parameter)
{
    return FWK_E_SUPPORT;
}

static int set_isr_nmi(void (*isr)(void))
{
    return FWK_E_SUPPORT;
}

static int set_isr_nmi_param(void (*isr)(uintptr_t param), uintptr_t parameter)
{
    return FWK_E_SUPPORT;
}

static int set_isr_fault(void (*isr)(void))
{
    return FWK_E_SUPPORT;
}

static int get_current(unsigned int *interrupt)
{
    return FWK_E_SUPPORT;
}

static bool is_interrupt_context(void)
{
    return false;
}

static const struct fwk_arch_interrupt_driver driver = {
    .global_enable = global_enable,
    .global_disable = global_disable,
    .is_enabled = is_enabled,
    .enable = enable,
    .disable = disable,
    .is_pending = is_pending,
    .set_pending = set_pending,
    .clear_pending = clear_pending,
    .set_isr_irq = set_isr_irq,
    .set_isr_irq_param = set_isr_irq_param,
    .set_isr_nmi = set_isr_nmi,
    .set_isr_nmi_param = set_isr_nmi_param,
    .set_isr_fault = set_isr_fault,
    .get_current = get_current,
    .is_interrupt_context = is_interrupt_context,
};

int arch_interrupt_init(const struct fwk_arch_interrupt_driver **_driver)
{
    if (_driver == NULL)
        return FWK_E_PARAM;

    *_driver = &driver;
    return FWK_SUCCESS;
}
