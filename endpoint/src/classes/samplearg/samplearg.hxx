#pragma once

#include "../initialization/initialization.hxx"

class c_samplearg;

class c_samplearg : public c_endpoint
{
public:
    explicit c_samplearg() noexcept;

    ~c_samplearg() noexcept override;

    [[nodiscard]] auto status_execute(const char* ptr_cstr_args) noexcept -> status_code;
};