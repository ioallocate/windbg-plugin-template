#pragma once

#include <windows.h>
#include <dbgeng.h>
#include <cstdint>
#include <memory>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using bool8 = bool;

enum class status_code : u32
{
    success = 0,
    failure_initialization,
    failure_invalid_argument,
    failure_debugger_interface,
    failure_execution
};

class c_endpoint;

class c_endpoint
{
public:

    explicit c_endpoint() noexcept;

    virtual ~c_endpoint() noexcept;

    c_endpoint(const c_endpoint&) = delete;
    auto operator= (const c_endpoint&) -> c_endpoint & = delete;

    c_endpoint(c_endpoint&&) = delete;
    auto operator= (c_endpoint&&) -> c_endpoint & = delete;

    [[nodiscard]] auto b_initialize(IDebugClient* ptr_client) noexcept -> bool8;

    auto v_cleanup() noexcept -> void;

    [[nodiscard]] auto b_is_initialized() const noexcept -> bool8;

protected:

    [[nodiscard]] auto b_output_text(const char* ptr_cstr_text) noexcept -> bool8;

    [[nodiscard]] auto b_output_formatted(const char* ptr_cstr_format, ...) noexcept -> bool8;

    [[nodiscard]] auto b_evaluate_expression(const char* ptr_cstr_expr, u64* ptr_out_value) noexcept -> bool8;

    [[nodiscard]] auto b_read_virtual(u64 u64_address, void* ptr_buffer, u32 u32_size, u32* ptr_out_bytes_read) noexcept -> bool8;

    [[nodiscard]] auto b_write_virtual(u64 u64_address, const void* ptr_buffer, u32 u32_size, u32* ptr_out_bytes_written) noexcept -> bool8;

    IDebugClient* ptr_m_debug_client;

    IDebugControl* ptr_m_debug_control;

    IDebugSymbols* ptr_m_debug_symbols;

    IDebugDataSpaces* ptr_m_debug_dataspaces;

    bool8 b_m_initialized;
};