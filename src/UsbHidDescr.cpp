#include "UsbHid.h"

#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>

static const uint8_t m_hid_report_descriptor[] = {
	// https://eleccelerator.com/usbdescreqparser/
	0x06, 0x00, 0xFF,	// Usage Page (Vendor Defined 0xFF00)
	0x09, 0x01,			// Usage (0x01)
	0xA1, 0x01,			// Collection (Application)
	0x85, 0x01,			//		Report ID (1)
	0x95, 0x01,			//		Report Count (1)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x02,			//		Report ID (2)
	0x95, 0x04,			//		Report Count (4)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x03,			//		Report ID (3)
	0x95, 0x01,			//		Report Count (1)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x04,			//		Report ID (4)
	0x95, 0x02,			//		Report Count (2)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x05,			//		Report ID (5)
	0x95, 0x02,			//		Report Count (2)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x06,			//		Report ID (6)
	0x95, 0x0D,			//		Report Count (13)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x10,			//		Report ID (16)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x11,			//		Report ID (17)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x12,			//		Report ID (18)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x13,			//		Report ID (19)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x85, 0x14,			//		Report ID (20)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x15,			//		Report ID (21)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x16,			//		Report ID (22)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x85, 0x17,			//		Report ID (23)
	0x95, 0x3F,			//		Report Count (63)
	0x75, 0x08,			//		Report Size (8)
	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x15, 0x00,			//		Logical Minimum (0)
	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x20,			//		Report ID (32)
	0x95, 0x01,			//		Report Count (1)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x21,			//		Report ID (33)
	0x95, 0x09,			//		Report Count (9)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x22,			//		Report ID (34)
	0x95, 0x3E,			//		Report Count (62)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x23,			//		Report ID (35)
	0x95, 0x3E,			//		Report Count (62)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x24,			//		Report ID (36)
	0x95, 0x3E,			//		Report Count (62)
	0x09, 0x01,			//		Usage (0x01)
	0xB1, 0x02,			//		Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,				// End Collection
};

static const struct {
	struct usb_hid_descriptor hid_descriptor;
	struct {
		uint8_t bReportDescriptorType;
		uint16_t wDescriptorLength;
	} __attribute__((packed)) hid_report;
} __attribute__((packed)) hid_function = {
	.hid_descriptor = {
		.bLength = sizeof(hid_function),
		.bDescriptorType = USB_DT_HID,
		.bcdHID = 0x0101,
		.bCountryCode = 0,
		.bNumDescriptors = 1,
	},
	.hid_report = {
		.bReportDescriptorType = USB_DT_REPORT,
		.wDescriptorLength = sizeof(m_hid_report_descriptor),
	}
};

const struct usb_endpoint_descriptor hid_endpoints[] = {
	{
		.bLength 			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType 	= USB_DT_ENDPOINT,
		.bEndpointAddress 	= 0x81,
		.bmAttributes 		= USB_ENDPOINT_ATTR_INTERRUPT,
		.wMaxPacketSize 	= 64,
		.bInterval 			= 0x01,
		.extra				= nullptr,
		.extralen			= 0,
	},
	{
		.bLength 			= USB_DT_ENDPOINT_SIZE,
		.bDescriptorType 	= USB_DT_ENDPOINT,
		.bEndpointAddress 	= 0x01,
		.bmAttributes 		= USB_ENDPOINT_ATTR_INTERRUPT,
		.wMaxPacketSize 	= 64,
		.bInterval 			= 0x01,
		.extra				= nullptr,
		.extralen			= 0,
	}
};

const struct usb_interface_descriptor hid_iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_HID,
	.bInterfaceSubClass = 0,
	.bInterfaceProtocol = 0,
	.iInterface = 0,

	.endpoint = hid_endpoints,

	.extra = &hid_function,
	.extralen = sizeof(hid_function),
};

const struct usb_interface ifaces[] = {{
	.cur_altsetting = nullptr,
	.num_altsetting = 1,
	.iface_assoc = nullptr,
	.altsetting = &hid_iface,
}};

const struct usb_config_descriptor UsbHid::m_config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0xC0,
	.bMaxPower = 0x32,
	.interface = ifaces,
};

const uint8_t *UsbHid::getReportDescriptors(uint16_t *length) {
	*length = static_cast<uint16_t>(sizeof(m_hid_report_descriptor));
	return m_hid_report_descriptor;
}
