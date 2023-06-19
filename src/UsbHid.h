#pragma once

#include <cstdio>

#include <libopencm3/usb/usbd.h>

class UsbHid {
	protected:
		UsbHid(const UsbHid &);
		void operator=(const UsbHid &);
		
		static UsbHid *m_instance;
		
		bool m_inited = false;
		bool m_test = false;
		usbd_device *m_usbd = nullptr;
		
		static const struct usb_device_descriptor m_dev;
		static const struct usb_config_descriptor m_config;
		
		static char m_serial_number[25];
		static const char *m_usb_strings[];
		uint8_t m_usbd_control_buffer[128];
		
		void triggerRescanUsb();
		
		enum usbd_request_return_codes handleGetReport(uint8_t report_type, uint8_t **buf, uint16_t *len, uint16_t w_length);
		enum usbd_request_return_codes handleSetReport(uint8_t report_type, uint8_t **buf, uint16_t *len, uint16_t w_length);
		
		static void usbSetConfigCallback(usbd_device *usbd_dev, uint16_t wValue);
		static enum usbd_request_return_codes usbHidControlRequest(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
			void (**complete)(usbd_device *, struct usb_setup_data *));
		static enum usbd_request_return_codes usbHidControlRequest2(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
			void (**complete)(usbd_device *, struct usb_setup_data *));
		
		static void usbDataRxCallback(usbd_device *usbd_dev, uint8_t ep);
		static void usbDataReportCallback(usbd_device *usbd_dev, uint8_t ep);
		
		static const uint8_t *getReportDescriptors(uint16_t *length);
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
