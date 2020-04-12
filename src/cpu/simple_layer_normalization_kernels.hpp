/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CPU_SIMPLE_LAYER_NORMALIZATION_KERNELS_HPP
#define CPU_SIMPLE_LAYER_NORMALIZATION_KERNELS_HPP

#include "layer_normalization_pd.hpp"

namespace dnnl {
namespace impl {
namespace cpu {
namespace lnorm_utils {

struct statistics_kernel_t {
    static statistics_kernel_t *create(const layer_normalization_pd_t *pd);
    virtual ~statistics_kernel_t() = default;

    virtual void operator()(const float *src, float *mean, float *var) const;

protected:
    statistics_kernel_t(const layer_normalization_pd_t *pd)
        : C_(pd->norm_axis()) {}

    int C_;
};

struct data_kernel_t {
    static data_kernel_t *create(const layer_normalization_pd_t *pd);
    virtual ~data_kernel_t() = default;

    virtual void operator()(const float *src, float *dst, const float *ss,
            const float *mean, const float *var) const;

protected:
    data_kernel_t(const layer_normalization_pd_t *pd)
        : C_(pd->norm_axis())
        , use_scaleshift_(pd->use_scaleshift())
        , eps_(pd->desc()->layer_norm_epsilon) {}

    int C_;
    bool use_scaleshift_;
    const float eps_;
};

struct diff_ss_kernel_t {
    static diff_ss_kernel_t *create(const layer_normalization_pd_t *pd);
    virtual ~diff_ss_kernel_t() = default;

    virtual void operator()(const float *src, const float *diff_dst,
            float *diff_gamma, float *diff_beta, const float *mean,
            const float *var) const;

protected:
    diff_ss_kernel_t(const layer_normalization_pd_t *pd)
        : C_(pd->norm_axis()), eps_(pd->desc()->layer_norm_epsilon) {}

    int C_;
    const float eps_;
};

struct diff_data_kernel_t {
    static diff_data_kernel_t *create(const layer_normalization_pd_t *pd);
    virtual ~diff_data_kernel_t() = default;

    virtual void operator()(const float *src, const float *diff_dst,
            float *diff_src, const float *ss, const float *mean,
            const float *var) const;

protected:
    diff_data_kernel_t(const layer_normalization_pd_t *pd)
        : C_(pd->norm_axis())
        , eps_(pd->desc()->layer_norm_epsilon)
        , calculate_diff_stats_(!pd->use_global_stats())
        , use_scaleshift_(pd->use_scaleshift()) {}

    int C_;
    const float eps_;
    bool calculate_diff_stats_;
    bool use_scaleshift_;
};

} // namespace lnorm_utils
} // namespace cpu
} // namespace impl
} // namespace dnnl

#endif
