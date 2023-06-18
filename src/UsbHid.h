#pragma once

#include <cstdio>

#include <libopencm3/usb/usbd.h>

class UsbHid {
	protected:
		UsbHid(const UsbHid &);
		void operator=(const UsbHid &);
		
		static UsbHid *m_instance;
		
		usbd_device *m_usbd = nullptr;
		
		static const struct usb_device_descriptor m_dev;
		static const struct usb_config_descriptor m_config;
		static const uint8_t m_hid_report_descriptor[];
		
		static char m_serial_number[25];
		static const char *m_usb_strings[];
		uint8_t m_usbd_control_buffer[128];
		
		void triggerRescanUsb();
		
		static void usbSetConfigCallback(usbd_device *usbd_dev, uint16_t wValue);
		static void usbDataRxCallback(usbd_device *usbd_dev, uint8_t ep);
	public:
		UsbHid() {
			m_instance = this;
		}
		
		~UsbHid() {
			m_instance = nullptr;
		}
		
		void init();
		void poll();
};
