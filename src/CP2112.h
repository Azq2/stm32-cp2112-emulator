#pragma once

#define USB_VENDOR_ID_CYGNAL		0x10c4
#define USB_DEVICE_ID_CYGNAL_CP2112	0xea90

enum {
	CP2112_GPIO_CONFIG				= 0x02,
	CP2112_GPIO_GET					= 0x03,
	CP2112_GPIO_SET					= 0x04,
	CP2112_GET_VERSION_INFO			= 0x05,
	CP2112_SMBUS_CONFIG				= 0x06,
	CP2112_DATA_READ_REQUEST		= 0x10,
	CP2112_DATA_WRITE_READ_REQUEST	= 0x11,
	CP2112_DATA_READ_FORCE_SEND		= 0x12,
	CP2112_DATA_READ_RESPONSE		= 0x13,
	CP2112_DATA_WRITE_REQUEST		= 0x14,
	CP2112_TRANSFER_STATUS_REQUEST	= 0x15,
	CP2112_TRANSFER_STATUS_RESPONSE	= 0x16,
	CP2112_CANCEL_TRANSFER			= 0x17,
	CP2112_LOCK_BYTE				= 0x20,
	CP2112_USB_CONFIG				= 0x21,
	CP2112_MANUFACTURER_STRING		= 0x22,
	CP2112_PRODUCT_STRING			= 0x23,
	CP2112_SERIAL_STRING			= 0x24,
};

enum {
	STATUS0_IDLE		= 0x00,
	STATUS0_BUSY		= 0x01,
	STATUS0_COMPLETE	= 0x02,
	STATUS0_ERROR		= 0x03,
};

enum {
	STATUS1_TIMEOUT_NACK		= 0x00,
	STATUS1_TIMEOUT_BUS			= 0x01,
	STATUS1_ARBITRATION_LOST	= 0x02,
	STATUS1_READ_INCOMPLETE		= 0x03,
	STATUS1_WRITE_INCOMPLETE	= 0x04,
	STATUS1_SUCCESS				= 0x05,
};

struct cp2112_smbus_config_report {
	uint8_t report;		/* CP2112_SMBUS_CONFIG */
	uint32_t clock_speed;	/* Hz */
	uint8_t device_address;	/* Stored in the upper 7 bits */
	uint8_t auto_send_read;	/* 1 = enabled, 0 = disabled */
	uint16_t write_timeout;	/* ms, 0 = no timeout */
	uint16_t read_timeout;	/* ms, 0 = no timeout */
	uint8_t scl_low_timeout;	/* 1 = enabled, 0 = disabled */
	uint16_t retry_time;	/* # of retries, 0 = no limit */
} __packed;

struct cp2112_usb_config_report {
	uint8_t report;	/* CP2112_USB_CONFIG */
	uint16_t vid;	/* Vendor ID */
	uint16_t pid;	/* Product ID */
	uint8_t max_power;	/* Power requested in 2mA units */
	uint8_t power_mode;	/* 0x00 = bus powered
			   0x01 = self powered & regulator off
			   0x02 = self powered & regulator on */
	uint8_t release_major;
	uint8_t release_minor;
	uint8_t mask;	/* What fields to program */
} __packed;

struct cp2112_read_req_report {
	uint8_t report;	/* CP2112_DATA_READ_REQUEST */
	uint8_t slave_address;
	uint16_t length;
} __packed;

struct cp2112_write_read_req_report {
	uint8_t report;	/* CP2112_DATA_WRITE_READ_REQUEST */
	uint8_t slave_address;
	uint16_t length;
	uint8_t target_address_length;
	uint8_t target_address[16];
} __packed;

struct cp2112_write_req_report {
	uint8_t report;	/* CP2112_DATA_WRITE_REQUEST */
	uint8_t slave_address;
	uint8_t length;
	uint8_t data[61];
} __packed;

struct cp2112_force_read_report {
	uint8_t report;	/* CP2112_DATA_READ_FORCE_SEND */
	uint16_t length;
} __packed;

struct cp2112_xfer_status_report {
	uint8_t report;	/* CP2112_TRANSFER_STATUS_RESPONSE */
	uint8_t status0;	/* STATUS0_* */
	uint8_t status1;	/* STATUS1_* */
	uint16_t retries;
	uint16_t length;
} __packed;
