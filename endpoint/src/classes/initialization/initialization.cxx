
#include "initialization.hxx"
#include <cstdarg>

c_endpoint::c_endpoint() noexcept
    : ptr_m_debug_client{ nullptr }
    , ptr_m_debug_control{ nullptr }
    , ptr_m_debug_symbols{ nullptr }
    , ptr_m_debug_dataspaces{ nullptr }
    , b_m_initialized{ false }
{
}

c_endpoint::~c_endpoint() noexcept
{
    v_cleanup();
}

[[nodiscard]] auto c_endpoint::b_initialize(IDebugClient* ptr_client) noexcept -> bool8
{
    if (ptr_client == nullptr)
    {
        return false;
    }

    if (b_m_initialized)
    {
        return true;
    }

    ptr_m_debug_client = ptr_client;

    HRESULT hr_result = ptr_client->QueryInterface(
        __uuidof (IDebugControl),
        reinterpret_cast<void**>(&ptr_m_debug_control)
    );

    if (FAILED(hr_result))
    {
        ptr_m_debug_client = nullptr;
        return false;
    }

    hr_result = ptr_client->QueryInterface(
        __uuidof (IDebugSymbols),
        reinterpret_cast<void**>(&ptr_m_debug_symbols)
    );

    if (FAILED(hr_result))
    {
        ptr_m_debug_control->Release();
        ptr_m_debug_control = nullptr;
        ptr_m_debug_client = nullptr;
        return false;
    }

    hr_result = ptr_client->QueryInterface(
        __uuidof (IDebugDataSpaces),
        reinterpret_cast<void**>(&ptr_m_debug_dataspaces)
    );

    if (FAILED(hr_result))
    {
        ptr_m_debug_symbols->Release();
        ptr_m_debug_symbols = nullptr;
        ptr_m_debug_control->Release();
        ptr_m_debug_control = nullptr;
        ptr_m_debug_client = nullptr;
        return false;
    }

    b_m_initialized = true;
    return true;
}

auto c_endpoint::v_cleanup() noexcept -> void
{
    if (ptr_m_debug_dataspaces != nullptr)
    {
        ptr_m_debug_dataspaces->Release();
        ptr_m_debug_dataspaces = nullptr;
    }

    if (ptr_m_debug_symbols != nullptr)
    {
        ptr_m_debug_symbols->Release();
        ptr_m_debug_symbols = nullptr;
    }

    if (ptr_m_debug_control != nullptr)
    {
        ptr_m_debug_control->Release();
        ptr_m_debug_control = nullptr;
    }

    ptr_m_debug_client = nullptr;
    b_m_initialized = false;
}

[[nodiscard]] auto c_endpoint::b_is_initialized() const noexcept -> bool8
{
    return b_m_initialized;
}

[[nodiscard]] auto c_endpoint::b_output_text(const char* ptr_cstr_text) noexcept -> bool8
{
    if (ptr_cstr_text == nullptr)
    {
        return false;
    }

    if (ptr_m_debug_control == nullptr)
    {
        return false;
    }

    HRESULT hr_result = ptr_m_debug_control->Output(
        DEBUG_OUTPUT_NORMAL,
        "%s",
        ptr_cstr_text
    );

    return SUCCEEDED(hr_result);
}

[[nodiscard]] auto c_endpoint::b_output_formatted(const char* ptr_cstr_format, ...) noexcept -> bool8
{
    if (ptr_cstr_format == nullptr)
    {
        return false;
    }

    if (ptr_m_debug_control == nullptr)
    {
        return false;
    }

    char arr_buffer[1024];

    va_list args_varargs;
    va_start(args_varargs, ptr_cstr_format);

    i32 i32_result = _vsnprintf_s(
        arr_buffer,
        sizeof(arr_buffer),
        _TRUNCATE,
        ptr_cstr_format,
        args_varargs
    );

    va_end(args_varargs);

    if (i32_result < 0)
    {
        return false;
    }

    return b_output_text(arr_buffer);
}

[[nodiscard]] auto c_endpoint::b_evaluate_expression(const char* ptr_cstr_expr, u64* ptr_out_value) noexcept -> bool8
{
    if (ptr_cstr_expr == nullptr || ptr_out_value == nullptr)
    {
        return false;
    }

    if (ptr_m_debug_control == nullptr)
    {
        return false;
    }

    DEBUG_VALUE debug_value;
    HRESULT hr_result = ptr_m_debug_control->Evaluate(
        ptr_cstr_expr,
        DEBUG_VALUE_INT64,
        &debug_value,
        nullptr
    );

    if (FAILED(hr_result))
    {
        return false;
    }

    *ptr_out_value = debug_value.I64;
    return true;
}

[[nodiscard]] auto c_endpoint::b_read_virtual(u64 u64_address, void* ptr_buffer, u32 u32_size, u32* ptr_out_bytes_read) noexcept -> bool8
{
    if (ptr_buffer == nullptr)
    {
        return false;
    }

    if (ptr_m_debug_dataspaces == nullptr)
    {
        return false;
    }

    ULONG ulong_bytes_read = 0;
    HRESULT hr_result = ptr_m_debug_dataspaces->ReadVirtual(
        u64_address,
        ptr_buffer,
        u32_size,
        &ulong_bytes_read
    );

    if (ptr_out_bytes_read != nullptr)
    {
        *ptr_out_bytes_read = static_cast<u32>(ulong_bytes_read);
    }

    return SUCCEEDED(hr_result);
}

[[nodiscard]] auto c_endpoint::b_write_virtual(u64 u64_address, const void* ptr_buffer, u32 u32_size, u32* ptr_out_bytes_written) noexcept -> bool8
{
    if (ptr_buffer == nullptr)
    {
        return false;
    }

    if (ptr_m_debug_dataspaces == nullptr)
    {
        return false;
    }

    ULONG ulong_bytes_written = 0;
    HRESULT hr_result = ptr_m_debug_dataspaces->WriteVirtual(
        u64_address,
        const_cast<PVOID>(ptr_buffer),
        u32_size,
        &ulong_bytes_written
    );

    if (ptr_out_bytes_written != nullptr)
    {
        *ptr_out_bytes_written = static_cast<u32>(ulong_bytes_written);
    }

    return SUCCEEDED(hr_result);
}
