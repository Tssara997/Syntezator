#pragma once
class SoundNotLoadingExepction: public std::exception
{
public:
	SoundNotLoadingExepction() noexcept = default;
	~SoundNotLoadingExepction() = default;
	virtual const char* what() const noexcept override {
		return "Sound not loading";
	}
};

