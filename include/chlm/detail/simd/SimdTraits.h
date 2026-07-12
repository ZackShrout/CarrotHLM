//
// Created by Zack Shrout on 7/11/26.
// Copyright (c) 2026 BunnySoft. All rights reserved.
//

#pragma once

namespace chlm::detail {
    enum class simd_backend_kind
    {
        scalar,
        sse2,
        neon
    };

    [[nodiscard]] constexpr const char* to_string(const simd_backend_kind kind) noexcept
    {
        switch (kind)
        {
            case simd_backend_kind::scalar: return "scalar";
            case simd_backend_kind::sse2: return "sse2";
            case simd_backend_kind::neon: return "neon";
        }

        return "unknown";
    }
} // namespace chlm::detail
