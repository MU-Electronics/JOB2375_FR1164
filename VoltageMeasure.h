#include <vector>
#include <map>

#pragma once
class VoltageMeasure
{
	protected:
		int SEL_PIN;
		int DATAOUT_PIN;
		int DATAIN_PIN;
		int SPICLOCK_PIN;
	private:
		int external(int channel);
		int internal(int channel);
		float average(int channel, int type, int value);
		float digitalToVoltage(int value);
		int acquire(int channel, int type);
		std::vector< std::map< int, std::vector<float> > >* channel_container;
		std::vector<int>::iterator Iter;
	public:
		VoltageMeasure(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt);
		~VoltageMeasure(void);
		int get(int channel, int type);
			
};

