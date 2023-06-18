#include "UsbHid.h"

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
	.bcdUSB = 0x0200,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x10C4,
	.idProduct = 0xEA90,
	.bcdDevice = 0x0200,
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
}

void UsbHid::usbSetConfigCallback(usbd_device *usbd_dev, uint16_t wValue) {
	(void) wValue;
//	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, usbDataRxCallback);
//	usbd_ep_setup(usbd_dev, 0x81, USB_ENDPOINT_ATTR_BULK, 64, NULL);
}

void UsbHid::usbDataRxCallback(usbd_device *usbd_dev, uint8_t ep) {
	(void) ep;
	
//	char buf[64];
//	int len = usbd_ep_read_packet(usbd_dev, 0x01, buf, 64);
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
