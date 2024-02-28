/* EEPROM for SN and MAC */

#define CONFIG_SYS_EEPROM_BUS_NUM		0
#define FORMAT_VERSION				0x1
#define PCB_VERSION				0x1
#define BOM_VERSION				'A'
/*
 * BYTES_PER_EEPROM_PAGE: the 24FC04H datasheet says that data can
 * only be written in page mode, which means 16 bytes at a time:
 * 16-Byte Page Write Buffer
 */
#define BYTES_PER_EEPROM_PAGE			16
/*
 * EEPROM_WRITE_DELAY_MS: the 24FC04H datasheet says it takes up to
 * 5ms to complete a given write:
 * Write Cycle Time (byte or page) ro Page Write Time 5 ms, Maximum
 */
#define EEPROM_WRITE_DELAY_MS			5000
/*
 * StarFive OUI. Registration Date is 20xx-xx-xx
 */
#define OUI_PREFIX			"6C:CF:39:"
#define DEFAULT_MAC			{0x6C, 0xCF, 0x39, 0x08, 0x08, 0x08}
/* Magic number at the first four bytes of EEPROM HATs */
#define EEPROM_HATS_SIG	"ROCK" /* Radxa ROCK */
#define CONFIG_EEPROM_HATS_SIZE_MAX	128 /* Header + Atom1&4(v1) */
#define CONFIG_EEPROM_WP_OFFSET	256 /* Read only field */
#define CONFIG_EEPROM_ATOM1_PSTR	"VF7100A1-2201-D008E000-00000001\0"
#define CONFIG_EEPROM_ATOM1_PSTR_SIZE	32
#define CONFIG_EEPROM_ATOM1_SN_OFFSET	23
#define CONFIG_EEPROM_ATOM1_VSTR	"Radxa Technology Co., Ltd.\0\0\0\0\0\0"
#define CONFIG_EEPROM_ATOM1_VSTR_SIZE	32
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1

int mac_read_from_eeprom(void);
