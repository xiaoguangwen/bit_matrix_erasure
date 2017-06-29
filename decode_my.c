#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*for file operator*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/*for syslog*/
#include <sys/klog.h>

/*for assert*/
#include <assert.h>

/*for dirname*/
#include <libgen.h>

/*for time*/
#include <sys/time.h>

typedef char                        ywb_int8_t;     
typedef unsigned char               ywb_uint8_t;     
typedef int                         ywb_int32_t;     
typedef unsigned int                ywb_uint32_t;    
typedef long long int               ywb_int64_t;    
typedef long long unsigned int      ywb_uint64_t;    


#define S_OK                          0
#define S_ERR                         -1

#define KB_SHIFT                      10
#define MILLION_SHIFT                 20
#define BILLION_SHIFT                 30

#define KILO_TO_BYTE(size)            ((size)<<KB_SHIFT)
#define BYTE_TO_KILO(size)            ((size)>>KB_SHIFT)

#define MILLION_TO_BYTE(size)         ((size)<<MILLION_SHIFT)
#define BYTE_TO_MILLION(size)         ((size)>>MILLION_SHIFT)

#define ENCODE_FILE_SIZE              MILLION_TO_BYTE(16)

#define ORIG_DATA_SEGMENT_NUM          4
#define VERIFY_DATA_SEGMENT_NUM        2

#define ALL_SEGMENT_NUM (ORIG_DATA_SEGMENT_NUM + VERIFY_DATA_SEGMENT_NUM)

#define SEGMENT_SLICE_UP_COUNT         4

#define SEGMENT_SIZE                  MILLION_TO_BYTE(4)
#define SEGMENT_SIZE_SLICE_UP         ((SEGMENT_SIZE)/4)


#define EC_ASSERT(test)               {if (test) assert(0);}

#define testlog(fmt, ...) {\
    syslog(3, "%s_%d:" fmt ".\n", __FUNCTION__, __LINE__,  ##__VA_ARGS__); \
}

#define SAVE_ENCODE_FILE_PATH         "/tmp/temp_save_encode/"
#define EC_INDEX_LEN                  (SEGMENT_SLICE_UP_COUNT*SEGMENT_SLICE_UP_COUNT)
#define WRITE_FILE_FLAG               (O_RDWR | O_CREAT | O_TRUNC)
#define WRITE_FILE                    1
#define TEST_FLAG                     0

typedef struct seg_index
{
    ywb_uint32_t index[EC_INDEX_LEN];
    ywb_uint32_t length;
}seg_index_t;

seg_index_t cauchy_table_1[SEGMENT_SLICE_UP_COUNT];
seg_index_t cauchy_table_2[SEGMENT_SLICE_UP_COUNT];

#define RECOVER_TABLE_SIZE 18
#define RECOVER_MAX_COUNT 2

typedef struct recover_info
{
    ywb_uint32_t lost_count;
    char *recover_name[RECOVER_MAX_COUNT];
    ywb_uint64_t recover_bitmap[RECOVER_MAX_COUNT][SEGMENT_SLICE_UP_COUNT];
}recover_info_t;

static recover_info_t g_recover_info_table[RECOVER_TABLE_SIZE];

void init_recover_info_table(void)
{
    g_recover_info_table[0].lost_count = 1;
    g_recover_info_table[0].recover_name[0] = "D0";
    g_recover_info_table[0].recover_bitmap[0][0] = 0B000000010001000100010000;
    g_recover_info_table[0].recover_bitmap[0][1] = 0B000000100010001000100000;   
    g_recover_info_table[0].recover_bitmap[0][2] = 0B000001000100010001000000;
    g_recover_info_table[0].recover_bitmap[0][3] = 0B000010001000100010000000;
    
    g_recover_info_table[1].lost_count = 1;
    g_recover_info_table[1].recover_name[0] = "D1";
    g_recover_info_table[1].recover_bitmap[0][0] = 0B000000010001000100000001;
    g_recover_info_table[1].recover_bitmap[0][1] = 0B000000100010001000000010;   
    g_recover_info_table[1].recover_bitmap[0][2] = 0B000001000100010000000100;
    g_recover_info_table[1].recover_bitmap[0][3] = 0B000010001000100000001000;
    
    g_recover_info_table[2].lost_count = 1;
    g_recover_info_table[2].recover_name[0] = "D2";
    g_recover_info_table[2].recover_bitmap[0][0] = 0B000000010001000000010001;
    g_recover_info_table[2].recover_bitmap[0][1] = 0B000000100010000000100010;    
    g_recover_info_table[2].recover_bitmap[0][2] = 0B000001000100000001000100;
    g_recover_info_table[2].recover_bitmap[0][3] = 0B000010001000000010001000;
    
    g_recover_info_table[3].lost_count = 1;
    g_recover_info_table[3].recover_name[0] = "D3";
    g_recover_info_table[3].recover_bitmap[0][0] = 0B000000010000000100010001;
    g_recover_info_table[3].recover_bitmap[0][1] = 0B000000100000001000100010;   
    g_recover_info_table[3].recover_bitmap[0][2] = 0B000001000000010001000100;
    g_recover_info_table[3].recover_bitmap[0][3] = 0B000010000000100010001000;
    
    g_recover_info_table[4].lost_count = 2;
    g_recover_info_table[4].recover_name[0] = "D0";       
    g_recover_info_table[4].recover_name[1] = "D1";
    g_recover_info_table[4].recover_bitmap[0][0] = 0B111011110010100000000000;
    g_recover_info_table[4].recover_bitmap[0][1] = 0B111111010100001100000000;   
    g_recover_info_table[4].recover_bitmap[0][2] = 0B110110011000011000000000;
    g_recover_info_table[4].recover_bitmap[0][3] = 0B100100010011110000000000;
    g_recover_info_table[4].recover_bitmap[1][0] = 0B111011100011100100000000;
    g_recover_info_table[4].recover_bitmap[1][1] = 0B111111110110000100000000;   
    g_recover_info_table[4].recover_bitmap[1][2] = 0B110111011100001000000000;
    g_recover_info_table[4].recover_bitmap[1][3] = 0B100110011011010000000000;

    
    g_recover_info_table[5].lost_count = 2;
    g_recover_info_table[5].recover_name[0] = "D0";       
    g_recover_info_table[5].recover_name[1] = "D2";
    g_recover_info_table[5].recover_bitmap[0][0] = 0B111111100111000000110000;
    g_recover_info_table[5].recover_bitmap[0][1] = 0B110111111110000001100000;   
    g_recover_info_table[5].recover_bitmap[0][2] = 0B100111011111000011000000;
    g_recover_info_table[5].recover_bitmap[0][3] = 0B000110011101000010110000;
    g_recover_info_table[5].recover_bitmap[1][0] = 0B111111110110000000100000;
    g_recover_info_table[5].recover_bitmap[1][1] = 0B110111011100000001000000;   
    g_recover_info_table[5].recover_bitmap[1][2] = 0B100110011011000010000000;
    g_recover_info_table[5].recover_bitmap[1][3] = 0B000100010101000000110000;
    
    g_recover_info_table[6].lost_count = 2;
    g_recover_info_table[6].recover_name[0] = "D0";       
    g_recover_info_table[6].recover_name[1] = "D3";
    g_recover_info_table[6].recover_bitmap[0][0] = 0B101110100000011011110000;
    g_recover_info_table[6].recover_bitmap[0][1] = 0B010101110000110011010000;   
    g_recover_info_table[6].recover_bitmap[0][2] = 0B101011100000101110010000;
    g_recover_info_table[6].recover_bitmap[0][3] = 0B011111110000010100010000;
    g_recover_info_table[6].recover_bitmap[1][0] = 0B101110110000011111100000;
    g_recover_info_table[6].recover_bitmap[1][1] = 0B010101010000111011110000;   
    g_recover_info_table[6].recover_bitmap[1][2] = 0B101010100000111111010000;
    g_recover_info_table[6].recover_bitmap[1][3] = 0B011101110000110110010000;

                         
    g_recover_info_table[7].lost_count = 2;
    g_recover_info_table[7].recover_name[0] = "D1";       
    g_recover_info_table[7].recover_name[1] = "D2";
    g_recover_info_table[7].recover_bitmap[0][0] = 0B010110111100000000001110;
    g_recover_info_table[7].recover_bitmap[0][1] = 0B101001011011000000001111;
    g_recover_info_table[7].recover_bitmap[0][2] = 0B011110100101000000001101;
    g_recover_info_table[7].recover_bitmap[0][3] = 0B111001111010000000001001;
    g_recover_info_table[7].recover_bitmap[1][0] = 0B010110101101000000001111;
    g_recover_info_table[7].recover_bitmap[1][1] = 0B101001111001000000001101;   
    g_recover_info_table[7].recover_bitmap[1][2] = 0B011111100001000000001001;
    g_recover_info_table[7].recover_bitmap[1][3] = 0B111011110010000000000001;
                         
    g_recover_info_table[8].lost_count = 2;
    g_recover_info_table[8].recover_name[0] = "D1";       
    g_recover_info_table[8].recover_name[1] = "D3";
    g_recover_info_table[8].recover_bitmap[0][0] = 0B011111110000010100001000;
    g_recover_info_table[8].recover_bitmap[0][1] = 0B111011010000101000000011;   
    g_recover_info_table[8].recover_bitmap[0][2] = 0B111110010000011100000110;
    g_recover_info_table[8].recover_bitmap[0][3] = 0B110100010000111000001100;
    g_recover_info_table[8].recover_bitmap[1][0] = 0B011111100000010000001001;
    g_recover_info_table[8].recover_bitmap[1][1] = 0B111011110000100000000001;   
    g_recover_info_table[8].recover_bitmap[1][2] = 0B111111010000001100000010;
    g_recover_info_table[8].recover_bitmap[1][3] = 0B110110010000011000000100;
                         
    g_recover_info_table[9].lost_count = 2;
    g_recover_info_table[9].recover_name[0] = "D2";        
    g_recover_info_table[9].recover_name[1] = "D3";
    g_recover_info_table[9].recover_bitmap[0][0] = 0B010000110000000010110111;
    g_recover_info_table[9].recover_bitmap[0][1] = 0B100001100000000001011110;   
    g_recover_info_table[9].recover_bitmap[0][2] = 0B001111000000000010101111;
    g_recover_info_table[9].recover_bitmap[0][3] = 0B011010110000000001111101;
    g_recover_info_table[9].recover_bitmap[1][0] = 0B010000100000000010100110;
    g_recover_info_table[9].recover_bitmap[1][1] = 0B100001000000000001111100;   
    g_recover_info_table[9].recover_bitmap[1][2] = 0B001110000000000011101011;
    g_recover_info_table[9].recover_bitmap[1][3] = 0B011000110000000011110101;
    
    g_recover_info_table[10].lost_count = 2;
    g_recover_info_table[10].recover_name[0] = "D0";        
    g_recover_info_table[10].recover_name[1] = "C0";
    g_recover_info_table[10].recover_bitmap[0][0] = 0B000100000100100100100000;
    g_recover_info_table[10].recover_bitmap[0][1] = 0B001000001000000101000000;   
    g_recover_info_table[10].recover_bitmap[0][2] = 0B010000000011001010000000;
    g_recover_info_table[10].recover_bitmap[0][3] = 0B100000000110010000110000;
    g_recover_info_table[10].recover_bitmap[1][0] = 0B000100000101100000110000;
    g_recover_info_table[10].recover_bitmap[1][1] = 0B001000001010001101100000;   
    g_recover_info_table[10].recover_bitmap[1][2] = 0B010000000111011011000000;
    g_recover_info_table[10].recover_bitmap[1][3] = 0B100000001110110010110000;
                         
    g_recover_info_table[11].lost_count = 2;
    g_recover_info_table[11].recover_name[0] = "D1";        
    g_recover_info_table[11].recover_name[1] = "C0";
    g_recover_info_table[11].recover_bitmap[0][0] = 0B100100000010110100001001;
    g_recover_info_table[11].recover_bitmap[0][1] = 0B000100000100100100000001;   
    g_recover_info_table[11].recover_bitmap[0][2] = 0B001000001000000100000010;
    g_recover_info_table[11].recover_bitmap[0][3] = 0B010000000011001000000100;
    g_recover_info_table[11].recover_bitmap[1][0] = 0B100100000011110000001000;
    g_recover_info_table[11].recover_bitmap[1][1] = 0B000100000110101100000011;   
    g_recover_info_table[11].recover_bitmap[1][2] = 0B001000001100010100000110;
    g_recover_info_table[11].recover_bitmap[1][3] = 0B010000001011101000001100;
                         
    g_recover_info_table[12].lost_count = 2;
    g_recover_info_table[12].recover_name[0] = "D2";       
    g_recover_info_table[12].recover_name[1] = "C0";
    g_recover_info_table[12].recover_bitmap[0][0] = 0B001000001000000001000010;
    g_recover_info_table[12].recover_bitmap[0][1] = 0B010000000011000010000100;   
    g_recover_info_table[12].recover_bitmap[0][2] = 0B100000000110000000111000;
    g_recover_info_table[12].recover_bitmap[0][3] = 0B001100001100000001100011;
    g_recover_info_table[12].recover_bitmap[1][0] = 0B001000001001000001010011;
    g_recover_info_table[12].recover_bitmap[1][1] = 0B010000000001000010100110;   
    g_recover_info_table[12].recover_bitmap[1][2] = 0B100000000010000001111100;
    g_recover_info_table[12].recover_bitmap[1][3] = 0B001100000100000011101011;
                         
    g_recover_info_table[13].lost_count = 2;
    g_recover_info_table[13].recover_name[0] = "D3";        
    g_recover_info_table[13].recover_name[1] = "C0";
    g_recover_info_table[13].recover_bitmap[0][0] = 0B110100000000111110011101;
    g_recover_info_table[13].recover_bitmap[0][1] = 0B100100000000110100011001;   
    g_recover_info_table[13].recover_bitmap[0][2] = 0B000100000000100100100001;
    g_recover_info_table[13].recover_bitmap[0][3] = 0B001000000000000101000010;
    g_recover_info_table[13].recover_bitmap[1][0] = 0B110100000000111010001100;
    g_recover_info_table[13].recover_bitmap[1][1] = 0B100100000000111100111011;   
    g_recover_info_table[13].recover_bitmap[1][2] = 0B000100000000110101100101;
    g_recover_info_table[13].recover_bitmap[1][3] = 0B001000000000100111001010;
                         
    g_recover_info_table[14].lost_count = 2;
    g_recover_info_table[14].recover_name[0] = "D0";     
    g_recover_info_table[14].recover_name[1] = "C1";
    g_recover_info_table[14].recover_bitmap[0][0] = 0B000000010001000100010000;
    g_recover_info_table[14].recover_bitmap[0][1] = 0B000000100010001000100000;   
    g_recover_info_table[14].recover_bitmap[0][2] = 0B000001000100010001000000;
    g_recover_info_table[14].recover_bitmap[0][3] = 0B000010001000100010000000;
    g_recover_info_table[14].recover_bitmap[1][0] = 0B000000010101100000110000;
    g_recover_info_table[14].recover_bitmap[1][1] = 0B000000101010001101100000;   
    g_recover_info_table[14].recover_bitmap[1][2] = 0B000001000111011011000000;
    g_recover_info_table[14].recover_bitmap[1][3] = 0B000010001110110010110000;
                         
    g_recover_info_table[15].lost_count = 2;
    g_recover_info_table[15].recover_name[0] = "D1";        
    g_recover_info_table[15].recover_name[1] = "C1";
    g_recover_info_table[15].recover_bitmap[0][0] = 0B000000010001000100000001;
    g_recover_info_table[15].recover_bitmap[0][1] = 0B000000100010001000000010;   
    g_recover_info_table[15].recover_bitmap[0][2] = 0B000001000100010000000100;
    g_recover_info_table[15].recover_bitmap[0][3] = 0B000010001000100000001000;
    g_recover_info_table[15].recover_bitmap[1][0] = 0B000000100110101100000011;
    g_recover_info_table[15].recover_bitmap[1][1] = 0B000001001100010100000110;   
    g_recover_info_table[15].recover_bitmap[1][2] = 0B000010001011101000001100;
    g_recover_info_table[15].recover_bitmap[1][3] = 0B000000110101011100001011;  
                         
    g_recover_info_table[16].lost_count = 2;
    g_recover_info_table[16].recover_name[0] = "D2";        
    g_recover_info_table[16].recover_name[1] = "C1";
    g_recover_info_table[16].recover_bitmap[0][0] = 0B000000010001000000010001;
    g_recover_info_table[16].recover_bitmap[0][1] = 0B000000100010000000100010;   
    g_recover_info_table[16].recover_bitmap[0][2] = 0B000001000100000001000100;
    g_recover_info_table[16].recover_bitmap[0][3] = 0B000010001000000010001000;
    g_recover_info_table[16].recover_bitmap[1][0] = 0B000010011101000010111000;
    g_recover_info_table[16].recover_bitmap[1][1] = 0B000000011001000001010011;   
    g_recover_info_table[16].recover_bitmap[1][2] = 0B000000100001000010100110;
    g_recover_info_table[16].recover_bitmap[1][3] = 0B000001000010000001111100;
                         
    g_recover_info_table[17].lost_count = 2;
    g_recover_info_table[17].recover_name[0] = "D3";       
    g_recover_info_table[17].recover_name[1] = "C1";
    g_recover_info_table[17].recover_bitmap[0][0] = 0B000000010000000100010001;
    g_recover_info_table[17].recover_bitmap[0][1] = 0B000000100000001000100010;   
    g_recover_info_table[17].recover_bitmap[0][2] = 0B000001000000010001000100;
    g_recover_info_table[17].recover_bitmap[0][3] = 0B000010000000100010001000;
    g_recover_info_table[17].recover_bitmap[1][0] = 0B000001000000110101100101;
    g_recover_info_table[17].recover_bitmap[1][1] = 0B000010000000100111001010;   
    g_recover_info_table[17].recover_bitmap[1][2] = 0B000000110000000110110111;
    g_recover_info_table[17].recover_bitmap[1][3] = 0B000001100000001001011110;
}

#if 0
void init_cauchy_table(void)
{
#if 1
    cauchy_table_1[0].index[0] = 11;
    cauchy_table_1[0].index[1] = 13;
    cauchy_table_1[0].index[2] = 0;
    cauchy_table_1[0].index[3] = 1;
    cauchy_table_1[0].index[4] = 2;
    cauchy_table_1[0].index[5] = 3;
    cauchy_table_1[0].index[6] = 5;
    cauchy_table_1[0].index[7] = 6;
    cauchy_table_1[0].index[8] = 7;
    cauchy_table_1[0].length = 9;

    cauchy_table_1[1].index[0] = 8;
    cauchy_table_1[1].index[1] = 9;
    cauchy_table_1[1].index[2] = 14;
    cauchy_table_1[1].index[3] = 0;
    cauchy_table_1[1].index[4] = 2;
    cauchy_table_1[1].index[5] = 3;
    cauchy_table_1[1].index[6] = 4;
    cauchy_table_1[1].index[7] = 5;
    cauchy_table_1[1].index[8] = 6;
    cauchy_table_1[1].index[9] = 7;    
    cauchy_table_1[1].length = 10;

    cauchy_table_1[2].index[0] = 9;
    cauchy_table_1[2].index[1] = 10;
    cauchy_table_1[2].index[2] = 15;
    cauchy_table_1[2].index[3] = 0;
    cauchy_table_1[2].index[4] = 3;
    cauchy_table_1[2].index[5] = 4;
    cauchy_table_1[2].index[6] = 6;
    cauchy_table_1[2].index[7] = 7;
    cauchy_table_1[2].length = 8;

    cauchy_table_1[3].index[0] = 10;
    cauchy_table_1[3].index[1] = 11;
    cauchy_table_1[3].index[2] = 12;
    cauchy_table_1[3].index[3] = 13;
    cauchy_table_1[3].index[4] = 0;
    cauchy_table_1[3].index[5] = 4;
    cauchy_table_1[3].index[6] = 7;
    cauchy_table_1[3].length = 7;

    cauchy_table_2[0].index[0] = 8;
    cauchy_table_2[0].index[1] = 11;
    cauchy_table_2[0].index[2] = 12;
    cauchy_table_2[0].index[3] = 13;
    cauchy_table_2[0].index[4] = 1;
    cauchy_table_2[0].index[5] = 2;
    cauchy_table_2[0].index[6] = 3;
    cauchy_table_2[0].index[7] = 5;
    cauchy_table_2[0].index[8] = 6;
    cauchy_table_2[0].index[9] = 7;
    cauchy_table_2[0].length = 10;

    cauchy_table_2[1].index[0] = 8;
    cauchy_table_2[1].index[1] = 13;
    cauchy_table_2[1].index[2] = 14;
    cauchy_table_2[1].index[3] = 0;
    cauchy_table_2[1].index[4] = 1;
    cauchy_table_2[1].index[5] = 2;
    cauchy_table_2[1].index[6] = 3;
    cauchy_table_2[1].index[7] = 4;
    cauchy_table_2[1].index[8] = 5;
    cauchy_table_2[1].index[9] = 6;    
    cauchy_table_2[1].index[10] = 7;    
    cauchy_table_2[1].length = 11;

    cauchy_table_2[2].index[0] = 9;
    cauchy_table_2[2].index[1] = 14;
    cauchy_table_2[2].index[2] = 15;
    cauchy_table_2[2].index[3] = 0;
    cauchy_table_2[2].index[4] = 2;
    cauchy_table_2[2].index[5] = 3;
    cauchy_table_2[2].index[6] = 4;
    cauchy_table_2[2].index[7] = 6;    
    cauchy_table_2[2].index[8] = 7;
    cauchy_table_2[2].length = 9;

    cauchy_table_2[3].index[0] = 10;
    cauchy_table_2[3].index[1] = 12;
    cauchy_table_2[3].index[2] = 13;
    cauchy_table_2[3].index[3] = 15;
    cauchy_table_2[3].index[4] = 0;
    cauchy_table_2[3].index[5] = 3;
    cauchy_table_2[3].index[6] = 4;    
    cauchy_table_2[3].index[7] = 7;
    cauchy_table_2[3].length = 8;
#endif

    return;
}
#endif

int decode_get_file_data(char *file_data, ywb_uint64_t file_size, char *file_name)
{
    int ret = S_OK;
    ywb_uint64_t read_size = 0;
    FILE *fp = NULL;
    
    EC_ASSERT(NULL == file_data);
    EC_ASSERT(NULL == file_name);    
    EC_ASSERT(SEGMENT_SIZE != file_size);

 	fp = fopen(file_name, "rb");
    if (NULL == fp) 
    {
        testlog("Failed open file:%s", file_name);

        ret = S_ERR;
    
        return ret;
    }
       
    read_size = fread(file_data, sizeof(char), file_size, fp);
    if (file_size != read_size)
    {
        testlog("Failed get file[%s] data due to get_size[%llu] is not equal file_size[%llu].",
                file_name, read_size, file_size);

        exit(0);     
    }
    
    fclose(fp);
    
    return ret;
}


int decode_check_file_vaild(char *file_name, ywb_uint64_t *file_size)
{
    int ret = S_OK;
    struct stat file_stat;

    EC_ASSERT(NULL == file_size);
    EC_ASSERT(NULL == file_name);

    if (NULL == file_name)
    {
        testlog("Filed to check file due to filename is NULL");

        EC_ASSERT(1);

        //ret = S_ERR;

        //return ret;
    }

    memset(&file_stat, 0x0, sizeof(struct stat));
    ret = stat(file_name, &file_stat);
    if (S_OK != ret)
    {
        testlog("Failed to decode due to file:%s is notexist.");

        ret = S_ERR;

        return ret;

        //EC_ASSERT(1);
    }
    
    if (!S_ISREG(file_stat.st_mode))
    {
        testlog("Failed to decode due to %s is file.");

        //ret = S_ERR;

        //return ret;

        EC_ASSERT(1);
    }

    if(SEGMENT_SIZE != file_stat.st_size)
    {
        testlog("Failed to decode due to file[%s] size[%llu] is not equal 4M.",
                file_name, file_stat.st_size);

        //ret = S_ERR;

        //return ret;           
        
        EC_ASSERT(1);
    }

    *file_size = file_stat.st_size;
    
    return ret;
}

int decode_get_file_data_by_name(char *file_name, char **file_data, ywb_uint64_t *file_size)
{
    int ret = S_OK;
    ywb_uint64_t temp_file_size = 0;
    char *temp_data = NULL;

    EC_ASSERT(NULL == file_name);
    EC_ASSERT(NULL == file_data);
    EC_ASSERT(NULL == file_size);

    ret = decode_check_file_vaild(file_name, &temp_file_size);
    if (S_OK != ret)
    {
        testlog("Failed to read file data.");

        return ret;
    }
    
    temp_data = malloc(temp_file_size);
    if (NULL == temp_data)
    {
        testlog("Failed to malloc mem for read file data.");

        return ret;
    }

    ret = decode_get_file_data(temp_data, temp_file_size, file_name);
    if (S_OK != ret)
    {
        testlog("Failed to get file[%s] data.");

        free(temp_data);

        return ret;
    }

    *file_data = temp_data;
    *file_size = temp_file_size;

    return ret;
}

void decode_segment_data_slice_up(char *segment_data, ywb_int64_t data_size,
                                              char **sub_segment_data)
{
    ywb_int32_t i = 0;

    EC_ASSERT(NULL == segment_data);
    EC_ASSERT(SEGMENT_SIZE != data_size);
    EC_ASSERT(sub_segment_data == NULL);

    for (i = 0; i < SEGMENT_SLICE_UP_COUNT; i++)
    {
        sub_segment_data[i] = segment_data + i*(SEGMENT_SIZE_SLICE_UP);
    }
    
    return;    
}

#define XOR_SWITCH 8
typedef long XOR_UNIT_TYPE;

#define XOR_TYPE_ASM 0
#if XOR_TYPE_ASM
#define EC_XOR_TEMPLETE(A, B, RESULT) ({ __asm__ __volatile__ (\
                            "xor    %1,    %2\n\t"  \
                            "movl   %2,    %0\n\t"  \
                            : "=r"(RESULT)          \
                            : "r"(A), "r"(B) ); })  
#else
#define EC_XOR_TEMPLETE(A, B, RESULT) {RESULT =(A) ^ (B);}   
#endif


void decode_compuate_ec_by_map_table(char *data_src,
                                   char *dst_data,
                                   ywb_uint64_t data_offset,
                                   ywb_uint64_t data_len)
{
    XOR_UNIT_TYPE *p_data_src = (XOR_UNIT_TYPE *)(data_src+data_offset);
    XOR_UNIT_TYPE *p_dst_data = (XOR_UNIT_TYPE *)(dst_data+data_offset);
    ywb_uint64_t change_length = data_len / sizeof(XOR_UNIT_TYPE);
    ywb_uint64_t i = 0;

    EC_ASSERT(NULL == data_src);
    //EC_ASSERT(SEGMENT_SIZE_SLICE_UP != data_len);
    EC_ASSERT(dst_data == NULL);    

#if (1 == XOR_SWITCH)
    for (i = 0; i < change_length ; i+=1)
    {
        EC_XOR_TEMPLETE(p_data_src[i], p_dst_data[i], p_data_src[i]);
    }
#elif (4 == XOR_SWITCH)
    for (i = 0; i < change_length ; i+=4)
    {    
        EC_XOR_TEMPLETE(p_data_src[i], p_dst_data[i], p_data_src[i]);
        EC_XOR_TEMPLETE(p_data_src[i+1], p_dst_data[i+1], p_data_src[i+1]);
        EC_XOR_TEMPLETE(p_data_src[i+2], p_dst_data[i+2], p_data_src[i+2]);
        EC_XOR_TEMPLETE(p_data_src[i+3], p_dst_data[i+3], p_data_src[i+3]);
    }
#elif (8 == XOR_SWITCH)
    for (i = 0; i < change_length ; i+=8)
    {    
        EC_XOR_TEMPLETE(p_data_src[i], p_dst_data[i], p_data_src[i]);
        EC_XOR_TEMPLETE(p_data_src[i+1], p_dst_data[i+1], p_data_src[i+1]);
        EC_XOR_TEMPLETE(p_data_src[i+2], p_dst_data[i+2], p_data_src[i+2]);
        EC_XOR_TEMPLETE(p_data_src[i+3], p_dst_data[i+3], p_data_src[i+3]);
        EC_XOR_TEMPLETE(p_data_src[i+4], p_dst_data[i+4], p_data_src[i+4]);
        EC_XOR_TEMPLETE(p_data_src[i+5], p_dst_data[i+5], p_data_src[i+5]);
        EC_XOR_TEMPLETE(p_data_src[i+6], p_dst_data[i+6], p_data_src[i+6]);
        EC_XOR_TEMPLETE(p_data_src[i+7], p_dst_data[i+7], p_data_src[i+7]);
    }
    
#elif (16 == XOR_SWITCH)
    for (i = 0; i < change_length ; i+=16)
    {
        EC_XOR_TEMPLETE(p_data_src[i], p_dst_data[i], p_data_src[i]);
        EC_XOR_TEMPLETE(p_data_src[i+1], p_dst_data[i+1], p_data_src[i+1]);
        EC_XOR_TEMPLETE(p_data_src[i+2], p_dst_data[i+2], p_data_src[i+2]);
        EC_XOR_TEMPLETE(p_data_src[i+3], p_dst_data[i+3], p_data_src[i+3]);
        EC_XOR_TEMPLETE(p_data_src[i+4], p_dst_data[i+4], p_data_src[i+4]);
        EC_XOR_TEMPLETE(p_data_src[i+5], p_dst_data[i+5], p_data_src[i+5]);
        EC_XOR_TEMPLETE(p_data_src[i+6], p_dst_data[i+6], p_data_src[i+6]);
        EC_XOR_TEMPLETE(p_data_src[i+7], p_dst_data[i+7], p_data_src[i+7]);

        EC_XOR_TEMPLETE(p_data_src[i+8], p_dst_data[i+8], p_data_src[i+8]);
        EC_XOR_TEMPLETE(p_data_src[i+9], p_dst_data[i+9], p_data_src[i+9]);
        EC_XOR_TEMPLETE(p_data_src[i+10], p_dst_data[i+10], p_data_src[i+10]);
        EC_XOR_TEMPLETE(p_data_src[i+11], p_dst_data[i+11], p_data_src[i+11]);
        EC_XOR_TEMPLETE(p_data_src[i+12], p_dst_data[i+12], p_data_src[i+12]);
        EC_XOR_TEMPLETE(p_data_src[i+13], p_dst_data[i+13], p_data_src[i+13]);
        EC_XOR_TEMPLETE(p_data_src[i+14], p_dst_data[i+14], p_data_src[i+14]);        
        EC_XOR_TEMPLETE(p_data_src[i+15], p_dst_data[i+15], p_data_src[i+15]);
    }
#elif (32 == XOR_SWITCH)
    for (i = 0; i < change_length ; i+=32)
    {
        EC_XOR_TEMPLETE(p_data_src[i], p_dst_data[i], p_data_src[i]);
        EC_XOR_TEMPLETE(p_data_src[i+1], p_dst_data[i+1], p_data_src[i+1]);
        EC_XOR_TEMPLETE(p_data_src[i+2], p_dst_data[i+2], p_data_src[i+2]);
        EC_XOR_TEMPLETE(p_data_src[i+3], p_dst_data[i+3], p_data_src[i+3]);
        EC_XOR_TEMPLETE(p_data_src[i+4], p_dst_data[i+4], p_data_src[i+4]);
        EC_XOR_TEMPLETE(p_data_src[i+5], p_dst_data[i+5], p_data_src[i+5]);
        EC_XOR_TEMPLETE(p_data_src[i+6], p_dst_data[i+6], p_data_src[i+6]);
        EC_XOR_TEMPLETE(p_data_src[i+7], p_dst_data[i+7], p_data_src[i+7]);

        EC_XOR_TEMPLETE(p_data_src[i+8], p_dst_data[i+8], p_data_src[i+8]);
        EC_XOR_TEMPLETE(p_data_src[i+9], p_dst_data[i+9], p_data_src[i+9]);
        EC_XOR_TEMPLETE(p_data_src[i+10], p_dst_data[i+10], p_data_src[i+10]);
        EC_XOR_TEMPLETE(p_data_src[i+11], p_dst_data[i+11], p_data_src[i+11]);
        EC_XOR_TEMPLETE(p_data_src[i+12], p_dst_data[i+12], p_data_src[i+12]);
        EC_XOR_TEMPLETE(p_data_src[i+13], p_dst_data[i+13], p_data_src[i+13]);
        EC_XOR_TEMPLETE(p_data_src[i+14], p_dst_data[i+14], p_data_src[i+14]);
        EC_XOR_TEMPLETE(p_data_src[i+15], p_dst_data[i+15], p_data_src[i+15]);

        EC_XOR_TEMPLETE(p_data_src[i+16], p_dst_data[i+16], p_data_src[i+16]);
        EC_XOR_TEMPLETE(p_data_src[i+17], p_dst_data[i+17], p_data_src[i+17]);
        EC_XOR_TEMPLETE(p_data_src[i+18], p_dst_data[i+18], p_data_src[i+18]);
        EC_XOR_TEMPLETE(p_data_src[i+19], p_dst_data[i+19], p_data_src[i+19]);
        EC_XOR_TEMPLETE(p_data_src[i+20], p_dst_data[i+20], p_data_src[i+20]);
        EC_XOR_TEMPLETE(p_data_src[i+21], p_dst_data[i+21], p_data_src[i+21]);
        EC_XOR_TEMPLETE(p_data_src[i+22], p_dst_data[i+22], p_data_src[i+22]);
        EC_XOR_TEMPLETE(p_data_src[i+23], p_dst_data[i+23], p_data_src[i+23]);
        
        EC_XOR_TEMPLETE(p_data_src[i+24], p_dst_data[i+24], p_data_src[i+24]);
        EC_XOR_TEMPLETE(p_data_src[i+25], p_dst_data[i+25], p_data_src[i+25]);
        EC_XOR_TEMPLETE(p_data_src[i+26], p_dst_data[i+26], p_data_src[i+26]);
        EC_XOR_TEMPLETE(p_data_src[i+27], p_dst_data[i+27], p_data_src[i+27]);
        EC_XOR_TEMPLETE(p_data_src[i+28], p_dst_data[i+28], p_data_src[i+28]);
        EC_XOR_TEMPLETE(p_data_src[i+29], p_dst_data[i+29], p_data_src[i+29]);
        EC_XOR_TEMPLETE(p_data_src[i+30], p_dst_data[i+30], p_data_src[i+30]);
        EC_XOR_TEMPLETE(p_data_src[i+31], p_dst_data[i+31], p_data_src[i+31]);            
    }
#else

    EC_ASSERT(1);
    
#endif


  return;
}

#define EC_UNIT_TYPE 6
static ywb_uint64_t g_ec_unit_size = KILO_TO_BYTE(64);
static ywb_uint64_t g_ec_unit_type[EC_UNIT_TYPE] = {MILLION_TO_BYTE(1), KILO_TO_BYTE(512), KILO_TO_BYTE(256), KILO_TO_BYTE(128),
                                       KILO_TO_BYTE(64), KILO_TO_BYTE(32)};

struct pthread_var
{
   ywb_uint64_t recover_table;
   char *ec_segmet_prt;
   ywb_uint64_t ec_len;
   ywb_uint64_t ec_offset;
};

static char *g_sub_segment[ORIG_DATA_SEGMENT_NUM][SEGMENT_SLICE_UP_COUNT];

void *pthread_func(void *argv)
{
    struct pthread_var *pthread_var = argv;
    ywb_uint32_t i = 0, j;
    ywb_uint32_t sub_seg_i = 0;    
    ywb_uint32_t sub_seg_j = 0;
    char *src = NULL;
    ywb_uint64_t offset = 0;
    ywb_uint64_t len = 0;    
    ywb_uint64_t temp_bitmap = 0;
    ywb_uint64_t bitmap_mask = 0x0000000000000001;

    len = pthread_var->ec_len;
    offset = pthread_var->ec_offset;
    src = pthread_var->ec_segmet_prt;

    temp_bitmap = pthread_var->recover_table;
    for (j = 0; (temp_bitmap != 0) ; j++, temp_bitmap = temp_bitmap >> 1)
    {
        if (!(temp_bitmap & bitmap_mask)) continue;
        
        sub_seg_i = j / SEGMENT_SLICE_UP_COUNT;
        sub_seg_j = j % SEGMENT_SLICE_UP_COUNT;
            
        decode_compuate_ec_by_map_table(src, 
                                        g_sub_segment[sub_seg_i][sub_seg_j],
                                        offset, len);
    }

    pthread_exit(0); 
    
    return;
}

void decode_make_recover_data_by_cauchy_table(char *recover_data,ywb_uint64_t recover_data_size,
                                                         ywb_uint64_t recover_bitmap[SEGMENT_SLICE_UP_COUNT],
                                                         char *sub_segment[ALL_SEGMENT_NUM][SEGMENT_SLICE_UP_COUNT])
{
    char *recover_segmet_prt[SEGMENT_SLICE_UP_COUNT] = {NULL};
    ywb_uint32_t i = 0, j;
    ywb_uint32_t sub_seg_i = 0;
    ywb_uint32_t sub_seg_j = 0;
    ywb_uint64_t temp_bitmap = 0;
    ywb_uint64_t data_offset = 0;
    ywb_uint64_t data_len = 0;
    ywb_uint64_t temp_ec_offset = 0;
    ywb_uint64_t bitmap_mask = 0x0000000000000001;
    
    pthread_t thread_tid[SEGMENT_SLICE_UP_COUNT] = {0};
    struct pthread_var pthread_var[SEGMENT_SLICE_UP_COUNT];

    EC_ASSERT(NULL == recover_data);
    EC_ASSERT(SEGMENT_SIZE != recover_data_size);
    EC_ASSERT(NULL == sub_segment);

    memset(recover_data, 0x0, recover_data_size);
    decode_segment_data_slice_up(recover_data, SEGMENT_SIZE, recover_segmet_prt);

    for (temp_ec_offset = 0; temp_ec_offset < SEGMENT_SIZE_SLICE_UP; temp_ec_offset += g_ec_unit_size)
    {
#if 1    
        
        for (i = 0; i < SEGMENT_SLICE_UP_COUNT; i++)
        {
            pthread_var[i].recover_table =  recover_bitmap[i];
            pthread_var[i].ec_segmet_prt = recover_segmet_prt[i];
            pthread_var[i].ec_len = g_ec_unit_size;
            pthread_var[i].ec_offset = temp_ec_offset;

            pthread_create(&thread_tid[i], NULL, pthread_func, &pthread_var[i]);
        }

        for (i = 0; i < SEGMENT_SLICE_UP_COUNT; i++)
        {
            pthread_join(thread_tid[i], NULL);
        }
#else

        for (i = 0; i < SEGMENT_SLICE_UP_COUNT; i++)
        {
            temp_bitmap = recover_bitmap[i];
            for (j = 0; (temp_bitmap != 0) ; j++, temp_bitmap = temp_bitmap >> 1)
            {
                if (!(temp_bitmap & bitmap_mask)) continue;
                
                sub_seg_i = j / SEGMENT_SLICE_UP_COUNT;
                sub_seg_j = j % SEGMENT_SLICE_UP_COUNT;
                    
                decode_compuate_ec_by_map_table(recover_segmet_prt[i], 
                                                sub_segment[sub_seg_i][sub_seg_j],
                                                temp_ec_offset, g_ec_unit_size);
            }
        } 
#endif
    }

    return;
}

void decode_save_file_data_into_path(char *file_data, 
                                                 ywb_int64_t data_size,
                                                 char *file_name)
{
#if WRITE_FILE

   int fd = 0;
   ywb_uint64_t write_size = 0;

   EC_ASSERT(NULL == file_data);    
   EC_ASSERT(NULL == file_name);    
   EC_ASSERT(0 == data_size);

   fd = open((const char *)file_name, WRITE_FILE_FLAG, 0777);
   if (0 == fd)
   {
       testlog("Failed to open file:%s.", file_name);

       EC_ASSERT(1);        
   }

   write_size = write(fd, file_data, data_size);
   EC_ASSERT(write_size != data_size);        

   close(fd);
   
#endif
//abort:

   return;
}

#if TEST_FLAG
    struct timeval time_start, time_end;
    struct timezone time_zone;
    double cost_tsec_once = 0;
    double cost_total_sec = 0;
    int need_recover_count = 0;
    int default_loop = 100;
#endif

void decode_api_for_file_data(char *segment_data[], ywb_uint32_t segment_count,
                                        ywb_uint64_t file_size, char *base_name)
{
    ywb_uint32_t i = 0;
    ywb_uint32_t j = 0;
    ywb_uint32_t k = 0;    
    ywb_uint32_t l = 0;
    ywb_uint32_t index = 0;
    ywb_uint32_t failure_count = RECOVER_TABLE_SIZE;
    //char *sub_segment[ALL_SEGMENT_NUM][SEGMENT_SLICE_UP_COUNT] = {NULL};
    char *recover_data = NULL;
    char file_path[256] = {0};

    EC_ASSERT(NULL == segment_data);
    EC_ASSERT(ALL_SEGMENT_NUM != segment_count);
    EC_ASSERT(SEGMENT_SIZE != file_size);
    EC_ASSERT(NULL == base_name);
    
    for (i = 0; i < segment_count; i++)
    {
        decode_segment_data_slice_up(segment_data[i], SEGMENT_SIZE, (char **)&(g_sub_segment[i]));   
    }

    recover_data = malloc(SEGMENT_SIZE);
    if (NULL == recover_data)
    {
        testlog("Failed to alloc mem for recover_data");

        goto abort;
    }


    #if TEST_FLAG
    for (l = 0; l < EC_UNIT_TYPE; l++)
    {
    g_ec_unit_size = g_ec_unit_type[l];

    gettimeofday(&time_start, &time_zone);
    for (k = 0; k < default_loop; k++)
    {
        for (index = 4, failure_count = 1, i = 0; i < failure_count; i++, index++)
    #else
    
    for (index = 0, i = 0; i < failure_count; i++, index++)

    #endif
    {        
        for (j = 0; j < g_recover_info_table[index].lost_count; j++)
        {           
            decode_make_recover_data_by_cauchy_table(recover_data, SEGMENT_SIZE,
                                                     g_recover_info_table[index].recover_bitmap[j],
                                                     g_sub_segment);

            sprintf(file_path, "%s%s%stype[%d]_%s", SAVE_ENCODE_FILE_PATH, 
                    base_name, "_recover_data_", i, g_recover_info_table[index].recover_name[j]);
            
            decode_save_file_data_into_path(recover_data, SEGMENT_SIZE, file_path);
        }
    }
    
#if TEST_FLAG
        }
    gettimeofday(&time_end, &time_zone);
    need_recover_count = g_recover_info_table[index].lost_count;
    cost_tsec_once = 0.0;
    cost_tsec_once += time_end.tv_usec;
    cost_tsec_once -= time_start.tv_usec;
    cost_tsec_once /= 1000000.0;
    cost_tsec_once += time_end.tv_sec;
    cost_tsec_once -= time_start.tv_sec;

    printf("decode loop_count:%d recover_size:%dM,  ec_unit:%dK, recover_count:%d, cost_time:%0.10f, arvage_time:%0.10f.\n",
           default_loop, file_size/1024/1024, g_ec_unit_size/1024,
           need_recover_count, cost_tsec_once, cost_tsec_once/default_loop); 
    }
#endif 

    free(recover_data);
    
abort:

    return;
}
                                        

int main(int argc, char **argv)
{
    int ret = S_OK;
    ywb_uint32_t i = 0;    
    ywb_uint32_t data_segment_count = ALL_SEGMENT_NUM;
    ywb_uint64_t file_size = 0;
    char *base_name = NULL;
    char combine_name[256] = {0};
    char *data_file_tail[ORIG_DATA_SEGMENT_NUM] = {"_segment_data_D0", "_segment_data_D1", "_segment_data_D2", "_segment_data_D3"}; 
    char *verify_file_tail[VERIFY_DATA_SEGMENT_NUM] = {"_ec_data_C0", "_ec_data_C1"};
    char *data_file_data[ORIG_DATA_SEGMENT_NUM] = {NULL};
    char *verify_file_data[VERIFY_DATA_SEGMENT_NUM] = {NULL};
    char *segment[ALL_SEGMENT_NUM] = {NULL};
    
#if TEST_FLAG
	if (argc > 3) {
		fprintf(stderr, "usage: inputfile\n");
		exit(0);
	}

    if (3 == argc)
    {
        default_loop = atoi(argv[2]);
        if (default_loop < 1)
        {
    		fprintf(stderr, "usage: input loop_count\n");
    		exit(0);            
        }
        printf("TEST---------default_count:%d.\n", default_loop);
    }    
#else    
    if (argc != 2) {
		fprintf(stderr, "usage: my inputfile");
		exit(0);
	}
#endif
    
    base_name = argv[1];

    for (i = 0; i < ORIG_DATA_SEGMENT_NUM; i++)
    {        
        sprintf(combine_name, "%s%s%s", SAVE_ENCODE_FILE_PATH, base_name, data_file_tail[i]);
        decode_get_file_data_by_name(combine_name, &data_file_data[i], &file_size);

        EC_ASSERT(NULL == data_file_data[i]);
        EC_ASSERT(SEGMENT_SIZE != file_size);
        segment[i] = data_file_data[i];
    }

    for (i = 0; i < VERIFY_DATA_SEGMENT_NUM; i++)
    {        
        sprintf(combine_name, "%s%s%s", SAVE_ENCODE_FILE_PATH, base_name, verify_file_tail[i]);
        decode_get_file_data_by_name(combine_name, &verify_file_data[i], &file_size);
        segment[i+ORIG_DATA_SEGMENT_NUM] = verify_file_data[i];
        
        EC_ASSERT(NULL == segment[i+ORIG_DATA_SEGMENT_NUM]);
        EC_ASSERT(SEGMENT_SIZE != file_size);
    }

    init_recover_info_table();
    decode_api_for_file_data(segment, ALL_SEGMENT_NUM, SEGMENT_SIZE, base_name);
    
    return ret;
}
