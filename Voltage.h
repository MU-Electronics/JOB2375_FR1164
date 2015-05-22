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
		std::map<int, float> voltages_internal; 
		std::map<int, float> voltages_external;
	public:
		VoltageMeasure();
		~VoltageMeasure(void);
		float get(int channel, int type);
		bool update(std::map<int, float>& averageChannelsInt, std::map<int, float>& averageChannelsExt);
			
};

