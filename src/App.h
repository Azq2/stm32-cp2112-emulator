#pragma once

#include <cstdio>

#include "UsbHid.h"

class App {
	protected:
		UsbHid m_usb = {};
		
		void initHw();
	public:
		int run();
};
