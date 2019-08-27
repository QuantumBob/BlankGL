#include "rwkError.h"

#include "dbghelp.h"

#include <string>


///////////////////////////////////////////////////////////

rwkError::rwkError() 
{
}

///////////////////////////////////////////////////////////

rwkError::~rwkError()
{
}

///////////////////////////////////////////////////////////

void APIENTRY rwkError::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)//33356)
	{
		std::string stack_walk;
		HANDLE process = GetCurrentProcess();

		// Set up the symbol options so that we can gather information from the current
		// executable's PDB files, as well as the Microsoft symbol servers.  We also want
		// to undecorate the symbol names we're returned.  If you want, you can add other
		// symbol servers or paths via a semi-colon separated list in SymInitialized.
		SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_UNDNAME);
	if (!SymInitialize(process, "http://msdl.microsoft.com/download/symbols", TRUE)) return;

		// Capture up to 25 stack frames from the current call stack.  We're going to
		// skip the first stack frame returned because that's the GetStackWalk function
		// itself, which we don't care about.
		PVOID addrs[25] = { 0 };
		USHORT frames = CaptureStackBackTrace(1, 25, addrs, NULL);

		for (USHORT i = 0; i < frames; i++) {
			// Allocate a buffer large enough to hold the symbol information on the stack and get 
			// a pointer to the buffer.  We also have to set the size of the symbol structure itself
			// and the number of bytes reserved for the name.
			ULONG64 buffer[(sizeof(SYMBOL_INFO) + 1024 + sizeof(ULONG64) - 1) / sizeof(ULONG64)] = { 0 };
			SYMBOL_INFO* info = (SYMBOL_INFO*)buffer;
			info->SizeOfStruct = sizeof(SYMBOL_INFO);
			info->MaxNameLen = 1024;

			// Attempt to get information about the symbol and add it to our output parameter.
			DWORD64 displacement = 0;
			if (SymFromAddr(process, (DWORD64)addrs[i], &displacement, info)) {

				IMAGEHLP_LINE64 line;// = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

				DWORD disp = 0;
				if (SymGetLineFromAddr64(process, (DWORD64)addrs[i], &disp, &line))
				{
					stack_walk.append("In function ");
					stack_walk.append(info->Name, info->NameLen);
					stack_walk.append("->");

					int j = i - 1;
					while (j >= 0)
					{
						SymFromAddr(process, (DWORD64)addrs[j], &displacement, info);
						if (std::string(info->Name).find("gl", 0, 2) != std::string::npos) {
							stack_walk.append(info->Name, info->NameLen);
							break;
						}
						j--;
					}
					stack_walk.append(" at line : ");
					stack_walk.append(std::to_string(line.LineNumber));
					stack_walk.append("\n");
					stack_walk.append("In file : ");
					stack_walk.append(line.FileName);
					stack_walk.append("\n");
					break;
				}
			}
		}

		SymCleanup(process);

		std::string msg = "[OpenGL Error] ("
			+ std::string(message) + ")\n"
			+ stack_walk;
		OutputDebugString(msg.c_str());
		__debugbreak();
	}
}
