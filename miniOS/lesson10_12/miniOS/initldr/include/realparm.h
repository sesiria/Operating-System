#ifndef REALPARM_H
#define REALPARM_H

#define ETYBAK_ADR 0x2000
#define PM32_EIP_OFF (ETYBAK_ADR)
#define PM32_ESP_OFF (ETYBAK_ADR+4)
#define RM16_EIP_OFF (ETYBAK_ADR+8)
#define RM16_ESP_OFF (ETYBAK_ADR+12)

#define RWHDPACK_ADR (ETYBAK_ADR+32)
#define E80MAP_NR (ETYBAK_ADR+64)           // store the number of the elements of struct e820map_t 
#define E80MAP_ADRADR (ETYBAK_ADR+68)       // store the start address of the struct e820map
#define E80MAP_ADR (0x5000)
#define VBEINFO_ADR (0x6000)
#define VBEMINFO_ADR (0x6400)

#define READHD_BUFADR 0x3000



#endif // REALPARM_H