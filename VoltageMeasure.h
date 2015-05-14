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
		bool average(int input, int value);
		float digitalToVoltage(int value);
		int acquire(int input, int type);
	public:
		VoltageMeasure(void);
		~VoltageMeasure(void);
		int get(int input, int type);
		
		
	
};

