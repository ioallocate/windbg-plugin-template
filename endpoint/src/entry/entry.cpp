#include "../classes/samplearg/samplearg.hxx"


static std::shared_ptr<c_samplearg> g_sptr_sample;

extern "C" __declspec (dllexport) auto CALLBACK DebugExtensionInitialize(
    u32* ptr_version,
    u32* ptr_flags
) noexcept -> HRESULT
{
    if (ptr_version == nullptr || ptr_flags == nullptr)
    {
        return E_INVALIDARG;
    }

    *ptr_version = DEBUG_EXTENSION_VERSION(1, 0);
    *ptr_flags = 0;

    g_sptr_sample = std::make_shared<c_samplearg>();
    if (g_sptr_sample == nullptr)
    {
        return E_OUTOFMEMORY;
    }

    return S_OK;
}

extern "C" __declspec (dllexport) auto CALLBACK DebugExtensionUninitialize() noexcept -> void
{

    if (g_sptr_sample != nullptr)
    {
        g_sptr_sample->v_cleanup();
        g_sptr_sample.reset();
    }
}

extern "C" __declspec (dllexport) auto CALLBACK DebugExtensionNotify(
    u32 u32_notify,
    u64 u64_argument
) noexcept -> void
{
    switch (u32_notify)
    {
    case DEBUG_NOTIFY_SESSION_ACTIVE:
    {
        break;
    }
    case DEBUG_NOTIFY_SESSION_INACTIVE:
    {
        if (g_sptr_sample != nullptr)
        {
            g_sptr_sample->v_cleanup();
        }
        break;
    }
    case DEBUG_NOTIFY_SESSION_ACCESSIBLE:
    {
        break;
    }
    case DEBUG_NOTIFY_SESSION_INACCESSIBLE:
    {
        break;
    }
    default:
    {
        break;
    }
    }
}

/*

        This is an example command, when loaded u index these by their function name
        in this case "hello", the actual way of calling this tho is by using the prefix
		"!" infront. The expected console input would then look like this:
		!hello [args]

*/

/*

    Define it as a export and a callback.
	input the IDebugClient interface pointer
    and the arguments as a c-string.
	Return an HRESULT code

*/

extern "C" __declspec (dllexport) auto CALLBACK hello(  
    IDebugClient* ptr_client,
    const char* ptr_cstr_args
) noexcept -> HRESULT
{
	// Validate the command instance

    if (g_sptr_sample == nullptr)
    {
        return E_FAIL; 
    }

	//  Make sure the command is initialized

    if (!g_sptr_sample->b_is_initialized())
    {
        bool8 b_init_success = g_sptr_sample->b_initialize(ptr_client);
        if (!b_init_success)
        {
            return E_FAIL;
        }
    }

	// Execute the command with the provided arguments

    status_code status_result = g_sptr_sample->status_execute(ptr_cstr_args);
    if (status_result != status_code::success)
    {
        return E_FAIL;
    }

    //  Return the status

    return S_OK;
}
