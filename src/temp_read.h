#define COLD_NTC_PIN                    PA0
#define HOT_NTC_PIN                     PA1
#define NTC3_PIN                        PC1
#define ICE_PIN                         PC2 
#define HOT_LEVEL_PIN                   PC3

void temp_read(void);
unsigned int ADC_RD(unsigned char u8_channel);
unsigned int maxminaverage(unsigned int *p,unsigned char len,unsigned char maxmin_len);
int temp_change_cold(unsigned int val);
int temp_change_hot(unsigned int val);