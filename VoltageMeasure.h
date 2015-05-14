

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
		bool average(int channel, int value);
		float digitalToVoltage(int value);
		int acquire(int channel, int type);
	public:
		VoltageMeasure();
		~VoltageMeasure(void);
		int get(int channel, int type);
		
		
	
};

