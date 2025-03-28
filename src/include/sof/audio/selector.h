/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2019 Intel Corporation. All rights reserved.
 *
 * Author: Lech Betlej <lech.betlej@linux.intel.com>
 */

/**
 * \file audio/selector.h
 * \brief Channel selector component header file
 * \authors Lech Betlej <lech.betlej@linux.intel.com>
 */

#ifndef __SOF_AUDIO_SELECTOR_H__
#define __SOF_AUDIO_SELECTOR_H__

#include <sof/audio/module_adapter/module/generic.h>
#include <sof/trace/trace.h>
#include <ipc/stream.h>
#if CONFIG_IPC_MAJOR_4
#include <ipc4/base-config.h>
#endif
#include <user/selector.h>
#include <user/trace.h>
#include <stdint.h>

struct comp_buffer;
struct comp_dev;

/** \brief Supported channel count on input. */
#define SEL_SOURCE_2CH 2
#define SEL_SOURCE_4CH 4

/** \brief Supported channel count on output. */
#define SEL_SINK_1CH 1
#define SEL_SINK_2CH 2
#define SEL_SINK_4CH 4

#if CONFIG_IPC_MAJOR_4
/** \brief selector processing function interface */
typedef void (*sel_func)(struct processing_module *mod, struct input_stream_buffer *bsource,
		       struct output_stream_buffer *bsink, uint32_t frames);

struct micsel_data {
	struct ipc4_base_module_cfg base_cfg;
	struct ipc4_audio_format output_format;
};
#else
typedef void (*sel_func)(struct comp_dev *dev, struct audio_stream __sparse_cache *sink,
			 const struct audio_stream __sparse_cache *source, uint32_t frames);
#endif

/** \brief Selector component private data. */
struct comp_data {
#if CONFIG_IPC_MAJOR_4
		struct micsel_data md;
#endif

	uint32_t source_period_bytes;	/**< source number of period bytes */
	uint32_t sink_period_bytes;	/**< sink number of period bytes */
	enum sof_ipc_frame source_format;	/**< source frame format */
	enum sof_ipc_frame sink_format;		/**< sink frame format */
	struct sof_sel_config config;	/**< component configuration data */
	sel_func sel_func;	/**< channel selector processing function */
};

/** \brief Selector processing functions map. */
struct comp_func_map {
	uint16_t source;	/**< source frame format */
	uint32_t out_channels;	/**< number of output stream channels */
	sel_func sel_func;	/**< selector processing function */
};

/** \brief Map of formats with dedicated processing functions. */
extern const struct comp_func_map func_map[];

#if CONFIG_IPC_MAJOR_4
/**
 * \brief Retrieves selector processing function.
 * \param[in,out] mod Selector module adapter.
 */
sel_func sel_get_processing_function(struct processing_module *mod);

#ifdef UNIT_TEST
void sys_comp_module_selector_interface_init(void);
#endif
#else
/**
 * \brief Retrieves selector processing function.
 * \param[in,out] dev Selector base component device.
 */
sel_func sel_get_processing_function(struct comp_dev *dev);

#ifdef UNIT_TEST
void sys_comp_selector_init(void);
#endif
#endif

#endif /* __SOF_AUDIO_SELECTOR_H__ */
