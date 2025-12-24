# windbg-plugin-template
A small Template for easily creating WinDbg extensions.

I wrote this in a very short time, so dont expect it to be perfect.

this is the template for the commands :


	Define it as a export and a callback.
    args: 
        input the IDebugClient interface pointer
		and the arguments as a c-string.
		-> Return an HRESULT code
	Validate the shared Pointer to the command instance
	Check if the command instance is initialized
    If not, initialize it with the provided IDebugClient pointer
			If initialization fails, return an error code
	Execute the command with the provided arguments
	If execution fails, return an error code
	Return success code

I am currently making a ai based plugin, and decided to release this on the way.
