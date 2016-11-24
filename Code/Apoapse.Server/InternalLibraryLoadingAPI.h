#pragma once

struct InternalLibraryLoadingAPI
{
	virtual void Start(std::vector<std::string>& params) = 0;
};