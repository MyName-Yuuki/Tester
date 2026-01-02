#pragma once

// Stub header for missing i_p2sp_download library
// This is a minimal stub to allow compilation

#include <string>
#include <vector>

namespace PATCH {

// Stub callback type
typedef void (*callback_t)(unsigned int current, unsigned int total);

// Stub DownloadManager
class DownloadManager
{
public:
	static DownloadManager& GetSingleton()
	{
		static DownloadManager instance;
		return instance;
	}

	// Stub function - placeholder only
	void GetByUrl(const char* src, const char* dest, callback_t callback, unsigned int start)
	{
		// TODO: Implement actual download logic
		// This is just a stub to allow compilation
	}

private:
	DownloadManager() {}
	~DownloadManager() {}
};

} // namespace PATCH
