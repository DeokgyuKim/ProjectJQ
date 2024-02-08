#pragma once

template<typename T>
class FIdGenerator
{
private:
	T Min;
	T Max;
	T CurrentID = Min;
public:
	void Initialize(T InMin, T InMax)
	{
		Min = InMin;
		Max = InMax;
		CurrentID = Min;
	}
	
	static int32 GenerateID()
	{
		int32 out = CurrentID++;
		if(CurrentID > Max)
			CurrentID = Min;
		return out;
	}
};