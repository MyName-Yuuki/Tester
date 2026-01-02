#pragma once

// Stub header for missing EC_HttpGet
// Minimal stub to allow compilation

class CECHttpGet
{
public:
	CECHttpGet() {}
	virtual ~CECHttpGet() {}

	// Stub functions
	bool Get(const char* url, const char* savePath) { return false; }
	void SetCallback(void* cb) {}
};
