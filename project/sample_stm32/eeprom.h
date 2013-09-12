#ifndef __PROJECT_EEPROM_H__
#define __PROJECT_EEPROM_H__

struct eeprom_priv_s {
	int bus_id;

	/* may not needed */
	i2c_dev *bus_dev;

	int dev_addr;
	int regaddr_size;
	int page_size;
	int pages;
};

int eeprom_read(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset,
		uint32 nb);
int eeprom_write(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset,
		 uint32 nb);
int eeprom_erase(struct eeprom_priv_s *ee, uint32 offset, uint32 nb);
int eeprom_verify(struct eeprom_priv_s *ee, uint8 * buf, uint32 offset,
		  uint32 nb);

#endif
