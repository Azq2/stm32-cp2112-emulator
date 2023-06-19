#include "UsbHid.h"
#include "CP2112.h"

#include <cstring>

#include "utils.h"

#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

UsbHid *UsbHid::m_instance = nullptr;

const struct usb_device_descriptor UsbHid::m_dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0101,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = USB_VENDOR_ID_CYGNAL,
	.idProduct = USB_DEVICE_ID_CYGNAL_CP2112,
	.bcdDevice = 0x0000,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

char UsbHid::m_serial_number[25] = {};
const char *UsbHid::m_usb_strings[] = { "Silicon Laboratories", "CP2112 HID USB-to-SMBus Bridge", m_serial_number };

void UsbHid::init() {
	desig_get_unique_id_as_string(m_serial_number, sizeof(m_serial_number));
	
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO12 | GPIO11);
	
	triggerRescanUsb();
	
	m_usbd = usbd_init(&st_usbfs_v1_usb_driver, &m_dev, &m_config, m_usb_strings, 3, m_usbd_control_buffer, sizeof(m_usbd_control_buffer));
	usbd_register_set_config_callback(m_usbd, usbSetConfigCallback);
	
	m_inited = false;
}

void UsbHid::usbSetConfigCallback(usbd_device *usbd_dev, uint16_t wValue) {
	(void) wValue;
	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_INTERRUPT, 64, &usbDataRxCallback);
	usbd_ep_setup(usbd_dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 64, NULL);
	usbd_register_control_callback(usbd_dev, USB_REQ_TYPE_STANDARD  | USB_REQ_TYPE_INTERFACE, USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, usbHidControlRequest);
	usbd_register_control_callback(usbd_dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE, USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, usbHidControlRequest);
	
	m_instance->m_inited = true;
}

enum usbd_request_return_codes UsbHid::usbHidControlRequest2(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
	void (**complete)(usbd_device *, struct usb_setup_data *))
{
	(void) complete;
	(void) dev;
	printf("usbHidControlRequest2!!!!!!!!!!!\r\n");
}

enum usbd_request_return_codes UsbHid::usbHidControlRequest(usbd_device *dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
	void (**complete)(usbd_device *, struct usb_setup_data *))
{
	(void) complete;
	(void) dev;
	
	if (req->bmRequestType == (USB_REQ_TYPE_IN | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE)) {
		if (req->bRequest == USB_REQ_GET_DESCRIPTOR) {
			if (req->wValue != 0x2200) // HID Report
				return USBD_REQ_NOTSUPP;
			*buf = const_cast<uint8_t *>(getReportDescriptors(len));
			return USBD_REQ_HANDLED;
		}
	} else if (req->bmRequestType == (USB_REQ_TYPE_IN | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)) {
		if (req->bRequest == USB_HID_REQ_TYPE_GET_REPORT) {
			return m_instance->handleGetReport(req->wValue & 0xFF, buf, len, req->wLength);
		}
	} else if (req->bmRequestType == (USB_REQ_TYPE_OUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)) {
		if (req->bRequest == USB_HID_REQ_TYPE_SET_REPORT) {
			return m_instance->handleSetReport(req->wValue & 0xFF, buf, len, req->wLength);
		}
	}
	
	//printf("[IGNORED] bmRequestType=%02X, bRequest=%02X, wValue=%04X\r\n", req->bmRequestType, req->bRequest, req->wValue);
	
	return USBD_REQ_NOTSUPP;
}

enum usbd_request_return_codes UsbHid::handleGetReport(uint8_t report_type, uint8_t **buf, uint16_t *len, uint16_t w_length) {
	if (report_type == CP2112_GET_VERSION_INFO && w_length == 3) {
		uint8_t version[] = {0, 0x0C, 0x02};
		*len = sizeof(version);
		memcpy(*buf, version, sizeof(version));
		return USBD_REQ_HANDLED;
	} else if (report_type == CP2112_LOCK_BYTE && w_length == 2) {
		uint8_t lock[] = {0, 0};
		*len = sizeof(lock);
		memcpy(*buf, lock, sizeof(lock));
		return USBD_REQ_HANDLED;
	} else if (report_type == CP2112_SMBUS_CONFIG && w_length == sizeof(cp2112_smbus_config_report)) {
		cp2112_smbus_config_report report = {
			.report				= 0,
			.clock_speed		= 0,
			.device_address		= 0,
			.auto_send_read		= 0,
			.write_timeout		= 0,
			.read_timeout		= 0,
			.scl_low_timeout	= 0,
			.retry_time			= 0,
		};
		*len = sizeof(report);
		memcpy(*buf, reinterpret_cast<void *>(&report), sizeof(report));
		return USBD_REQ_HANDLED;
	}
	
	printf("Unknown GET report %02X\r\n", report_type);
	
	return USBD_REQ_NOTSUPP;
}


enum usbd_request_return_codes UsbHid::handleSetReport(uint8_t report_type, uint8_t **buf, uint16_t *len, uint16_t w_length) {
	printf("Unknown SET report %02X\r\n", report_type);
	return USBD_REQ_HANDLED;
}

void UsbHid::usbDataRxCallback(usbd_device *usbd_dev, uint8_t ep) {
	(void) ep;
	
	printf("usbDataRxCallback!!!!!!!!!! %02X\r\n", ep);
	
	char buf[64];
	int len = usbd_ep_read_packet(usbd_dev, 0x01, buf, 64);
	
	printf("usbDataRxCallback len=%d\r\n", len);
	
//	if (len) {
//		while (usbd_ep_write_packet(usbd_dev, 0x81, m_sysex_identity, sizeof(m_sysex_identity)) == 0);
//	}
}

void UsbHid::poll() {
	usbd_poll(m_usbd);
}

void UsbHid::triggerRescanUsb() {
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
	gpio_clear(GPIOA, GPIO12);
	delay_ms(2);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO12);
}
