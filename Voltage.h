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
		int MOVING_AVERAGE_SAMPLES;
		int INTERNAL_ADC_BIT_SIZE;
		int EXTERNAL_ADC_BIT_SIZE;
	private:
		int external(int channel);
		int internal(int channel);
		float average(int channel, int type, int value);
		float digitalToVoltage(int channel, int type, int value);
		float acquire(int channel, int type);
		std::map< int, std::map< int, std::map<int, float> > > channel_container;
		std::vector<float> voltages_internal; 
		std::vector<float> voltages_external;
	public:
		VoltageMeasure(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt, std::vector<float>& DigitalVoltagesInternal, std::vector<float>& DigitalVoltagesExternal);
		~VoltageMeasure(void);
		float get(int channel, int type);
		bool update(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt);
			
};

