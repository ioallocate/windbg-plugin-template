#include "samplearg.hxx"

c_samplearg::c_samplearg() noexcept
    : c_endpoint()
{
}

c_samplearg::~c_samplearg() noexcept
{
}

[[nodiscard]] auto c_samplearg::status_execute(const char* ptr_cstr_args) noexcept -> status_code
{
    if (!b_m_initialized)
    {
        return status_code::failure_initialization;
    }

    bool8 b_output_success = b_output_text("Sample Arg.!\n");
    if (!b_output_success)
    {
        return status_code::failure_execution;
    }

    if (ptr_cstr_args != nullptr && ptr_cstr_args[0] != '\0')
    {
        b_output_success = b_output_formatted("Formatted Args: %s\n", ptr_cstr_args);
        if (!b_output_success)
        {
            return status_code::failure_execution;
        }
    }

    return status_code::success;
}