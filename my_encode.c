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

#define ENCODE_FILE_SIZE              MILLION_TO_BYTE(256)

#define FILE_SLICE_UP_COUNT           4
#define SEGMENT_SLICE_UP_COUNT        4

#define FILE_SIZE_SLICE_UP            (MILLION_TO_BYTE(256)/4)
#define SEGMENT_SIZE_SLICE_UP         ((FILE_SIZE_SLICE_UP)/4)

#define EC_ASSERT(test)               {if (test) assert(0);}
//{if (test) assert(0);}
#define testlog(fmt, ...) {\
    syslog(3, "%s_%d:" fmt ".\n", __FUNCTION__, __LINE__,  ##__VA_ARGS__); \
}

#define SAVE_ENCODE_FILE_PATH         "/tmp/temp_save_encode/"
#define EC_INDEX_LEN                  (SEGMENT_SLICE_UP_COUNT*SEGMENT_SLICE_UP_COUNT)
#define WRITE_FILE_FLAG               (O_RDWR | O_CREAT | O_TRUNC)
#define WRITE_FILE 0
#define TEST_FLAG 1

typedef struct seg_index
{
    ywb_uint32_t index[EC_INDEX_LEN];
    ywb_uint32_t length;
}seg_index_t;

seg_index_t cauchy_table_1[FILE_SLICE_UP_COUNT];
seg_index_t cauchy_table_2[FILE_SLICE_UP_COUNT];

void init_cauchy_table(void)
{
#if 1
    cauchy_table_1[0].index[0] = 0;
    cauchy_table_1[0].index[1] = 4;
    cauchy_table_1[0].index[2] = 8;
    cauchy_table_1[0].index[3] = 12;
    cauchy_table_1[0].length = 4;

    cauchy_table_1[1].index[0] = 1;
    cauchy_table_1[1].index[1] = 5;
    cauchy_table_1[1].index[2] = 9;
    cauchy_table_1[1].index[3] = 13;
    cauchy_table_1[1].length = 4;

    cauchy_table_1[2].index[0] = 2;
    cauchy_table_1[2].index[1] = 6;
    cauchy_table_1[2].index[2] = 10;
    cauchy_table_1[2].index[3] = 14;
    cauchy_table_1[2].length = 4;

    cauchy_table_1[3].index[0] = 3;
    cauchy_table_1[3].index[1] = 7;
    cauchy_table_1[3].index[2] = 11;
    cauchy_table_1[3].index[3] = 15;
    cauchy_table_1[3].length = 4;
#endif    

#if 2
    cauchy_table_2[0].index[0] = 0;
    cauchy_table_2[0].index[1] = 5;
    cauchy_table_2[0].index[2] = 8;
    cauchy_table_2[0].index[3] = 11;    
    cauchy_table_2[0].index[4] = 14;
    cauchy_table_2[0].length = 5;

    cauchy_table_2[1].index[0] = 1;
    cauchy_table_2[1].index[1] = 6;
    cauchy_table_2[1].index[2] = 8;
    cauchy_table_2[1].index[3] = 15;
    cauchy_table_2[1].length = 4;

    cauchy_table_2[2].index[0] = 2;
    cauchy_table_2[2].index[1] = 7;
    cauchy_table_2[2].index[2] = 9;
    cauchy_table_2[2].index[3] = 12;    
    cauchy_table_2[2].index[4] = 13;
    cauchy_table_2[2].length = 5;

    cauchy_table_2[3].index[0] = 3;
    cauchy_table_2[3].index[1] = 4;
    cauchy_table_2[3].index[2] = 5;
    cauchy_table_2[3].index[3] = 10;
    cauchy_table_2[3].index[4] = 13;
    cauchy_table_2[3].index[5] = 14;
    cauchy_table_2[3].length = 6;
#endif

    return;
}

void encode_save_file_data_into_path(char *file_data, 
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
    

void encode_file_data_slice_up(char *file_data, ywb_int64_t data_size,
                                      char **segment_data)
{
    ywb_int32_t i = 0;

    EC_ASSERT(NULL == file_data);
    EC_ASSERT(ENCODE_FILE_SIZE != data_size);
    EC_ASSERT(segment_data == NULL);

    for (i = 0; i < FILE_SLICE_UP_COUNT; i++)
    {
        segment_data[i] = file_data + i*(FILE_SIZE_SLICE_UP);
    }
    
    return;
}

void encode_segment_data_slice_up(char *segment_data, ywb_int64_t data_size,
                                  char **sub_segment_data)
{
    ywb_int32_t i = 0;

    EC_ASSERT(NULL == segment_data);
    EC_ASSERT(FILE_SIZE_SLICE_UP != data_size);
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
#if 0
#define EC_XOR_TEMPLETE(A, B, RESULT) ({ __asm__ __volatile__ (\
                            "xor    %1,    %2\n\t"  \
                            "movl   %2,    %0\n\t"  \
                            : "=r"(RESULT)          \
                            : "r"(A), "r"(B) ); })  
#endif
#define EC_XOR_TEMPLETE(A, B, RESULT) ({ __asm__ __volatile__ (\
                                    "xor    %2,    %1\n\t"  \
                                    : "=r"(RESULT)          \
                                    : "r"(A), "r"(B) ); })  


#else
#define EC_XOR_TEMPLETE(A, B, RESULT) {RESULT = (A) ^ (B);}  
#endif


void encode_compuate_ec_by_map_table(char *data_src,
                                              char *dst_data,
                                              ywb_uint64_t data_offset,
                                              ywb_uint64_t data_len)
{
    XOR_UNIT_TYPE *p_data_src = (XOR_UNIT_TYPE *)(data_src + data_offset);
    XOR_UNIT_TYPE *p_dst_data = (XOR_UNIT_TYPE *)(dst_data + data_offset);
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
    seg_index_t *cauchy_table;
    char *ec_segmet_prt;
    ywb_uint64_t ec_len;
    ywb_uint64_t ec_offset;
};

static char *g_sub_segment[FILE_SLICE_UP_COUNT][SEGMENT_SLICE_UP_COUNT];

void *pthread_func(void *argv)
{
    struct pthread_var *pthread_var = argv;
    ywb_uint32_t i = 0, j;
    ywb_uint32_t sub_seg_i = 0;    
    ywb_uint32_t sub_seg_j = 0;
    char *src = NULL;
    ywb_uint64_t offset = 0;
    ywb_uint64_t len = 0;    

    len = pthread_var->ec_len;
    offset = pthread_var->ec_offset;
    src = pthread_var->ec_segmet_prt;

    for (j = 0; j < pthread_var->cauchy_table->length; j++)
    {
        
        sub_seg_i = pthread_var->cauchy_table->index[j] / SEGMENT_SLICE_UP_COUNT;
        sub_seg_j = pthread_var->cauchy_table->index[j] % SEGMENT_SLICE_UP_COUNT;
        if (0 == j) 
        { 
            memcpy(src+offset, g_sub_segment[sub_seg_i][sub_seg_j]+offset, len);
            continue;
        }    
        encode_compuate_ec_by_map_table(src, g_sub_segment[sub_seg_i][sub_seg_j],offset, len);
    }   

    pthread_exit(0); 
    
    return;
}

void encode_make_ecdate_by_cauchy_table(char *ec_data, ywb_uint64_t ec_data_size,
                                                 seg_index_t *cauchy_table,
                                                 char *sub_segment[FILE_SLICE_UP_COUNT][SEGMENT_SLICE_UP_COUNT])
{
    char *ec_segmet_prt[FILE_SLICE_UP_COUNT] = {NULL};
    ywb_uint32_t i = 0, j;
    ywb_uint32_t sub_seg_i = 0;    
    ywb_uint32_t sub_seg_j = 0;
    ywb_uint64_t data_offset = 0;
    ywb_uint64_t data_len = 0;
    ywb_uint64_t temp_ec_offset = 0;
    pthread_t thread_tid[SEGMENT_SLICE_UP_COUNT] = {0};
    struct pthread_var pthread_var[SEGMENT_SLICE_UP_COUNT];

    EC_ASSERT(NULL == ec_data);
    EC_ASSERT(FILE_SIZE_SLICE_UP != ec_data_size);
    EC_ASSERT(cauchy_table == NULL);
    EC_ASSERT(NULL == sub_segment);

    memset(ec_data, 0x0, ec_data_size);
    encode_segment_data_slice_up(ec_data, FILE_SIZE_SLICE_UP, ec_segmet_prt);

    for (temp_ec_offset = 0; temp_ec_offset < SEGMENT_SIZE_SLICE_UP; temp_ec_offset += g_ec_unit_size)
    {
#if 1    

        for (i = 0; i < SEGMENT_SLICE_UP_COUNT; i++)
        {
            pthread_var[i].cauchy_table =  &cauchy_table[i];
            pthread_var[i].ec_segmet_prt = ec_segmet_prt[i];
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
            for (j = 0; j < cauchy_table[i].length; j++)
            {
                
                sub_seg_i = cauchy_table[i].index[j] / SEGMENT_SLICE_UP_COUNT;
                sub_seg_j = cauchy_table[i].index[j] % SEGMENT_SLICE_UP_COUNT;
                if (0 == j) 
                { 
                    memcpy(ec_segmet_prt[i]+temp_ec_offset, sub_segment[sub_seg_i][sub_seg_j]+temp_ec_offset,
                           g_ec_unit_size);
                    continue;
                }    
                encode_compuate_ec_by_map_table(ec_segmet_prt[i], 
                                                sub_segment[sub_seg_i][sub_seg_j],
                                                temp_ec_offset, g_ec_unit_size);
            }
        }
#endif

    }
    
    return;
}

int encode_api_for_file_data(char *file_data, ywb_int64_t data_size, char *file_name)
{
    int ret = S_OK;
    ywb_uint32_t i = 0;    
    ywb_uint32_t j = 0;
    char file_path[256] = {0};
    char *segment[FILE_SLICE_UP_COUNT] = {NULL};
    //char *sub_segment[FILE_SLICE_UP_COUNT][SEGMENT_SLICE_UP_COUNT] = {NULL};
    char *ec_data = NULL;

#if TEST_FLAG
    struct timeval time_start, time_end;
    struct timezone time_zone;
    double cost_tsec_once = 0;
    double cost_total_sec = 0;
    int default_loop = 100;
#endif

    EC_ASSERT(NULL == file_data);
    EC_ASSERT(ENCODE_FILE_SIZE != data_size);
    EC_ASSERT(file_name == NULL);
    
    encode_file_data_slice_up(file_data, data_size, segment);

    for (i = 0; i < FILE_SLICE_UP_COUNT; i++)
    {
        encode_segment_data_slice_up(segment[i], FILE_SIZE_SLICE_UP, (char **)&(g_sub_segment[i]));
    }

    ec_data = malloc(FILE_SIZE_SLICE_UP);
    if (NULL == ec_data)
    {
        ret = S_ERR;

        testlog("Failed to alloc mem for ec_data");

        goto abort;
    }


#if TEST_FLAG
    for (i = 0; i < EC_UNIT_TYPE; i++)
    {
    g_ec_unit_size = g_ec_unit_type[i];
    gettimeofday(&time_start, &time_zone);

    for (j = 0; j < default_loop; j++)
    {
#endif  

    encode_make_ecdate_by_cauchy_table(ec_data, FILE_SIZE_SLICE_UP, cauchy_table_1, g_sub_segment);

    sprintf(file_path, "%s%s%s%d", SAVE_ENCODE_FILE_PATH, file_name, "_ec_data_C", 0);
    encode_save_file_data_into_path(ec_data, FILE_SIZE_SLICE_UP, file_path);

    encode_make_ecdate_by_cauchy_table(ec_data, FILE_SIZE_SLICE_UP, cauchy_table_2, g_sub_segment);

    sprintf(file_path, "%s%s%s%d", SAVE_ENCODE_FILE_PATH, file_name, "_ec_data_C", 1);
    encode_save_file_data_into_path(ec_data, FILE_SIZE_SLICE_UP, file_path);
    
#if TEST_FLAG
    }
    gettimeofday(&time_end, &time_zone);
    cost_tsec_once = 0.0;
    cost_tsec_once += time_end.tv_usec;
    cost_tsec_once -= time_start.tv_usec;
    cost_tsec_once /= 1000000.0;
    cost_tsec_once += time_end.tv_sec;
    cost_tsec_once -= time_start.tv_sec;

    printf("encode loop_count:%d encode_size:%dM, ec_unit:%dK, encode_count:%d, cost_time:%0.10f, arvage_time:%0.10f.\n",
           default_loop, FILE_SIZE_SLICE_UP/1024/1024, g_ec_unit_size/1024,
           FILE_SLICE_UP_COUNT, cost_tsec_once, cost_tsec_once/default_loop);
    }
#endif 

    for (i = 0; i < FILE_SLICE_UP_COUNT; i++)
    {
        sprintf(file_path, "%s%s%sD%u", SAVE_ENCODE_FILE_PATH, file_name, "_segment_data_", i);
        encode_save_file_data_into_path(segment[i], FILE_SIZE_SLICE_UP, file_path);     
    }

    free(ec_data);
    
abort:
    
    return ret;
}


int encode_get_file_data(char **file_data, char *file_name)
{
    int ret = S_OK;
    ywb_uint64_t data_size = 0;
    char *temp_data = NULL;   
    FILE *fp = NULL;

    if ((NULL == file_data) || (NULL == file_name))
    {
        testlog("error input.");
    }

	fp = fopen(file_name, "rb");
    if (NULL == fp) 
    {
        testlog("Failed open file:%s", file_name);

        ret = S_ERR;
    
        return ret;
    }

    temp_data = malloc(ENCODE_FILE_SIZE);
    if (NULL == temp_data)
    {
        testlog("Failed alloc memory[size:%llu] to save file[%s] data.",
                ENCODE_FILE_SIZE, file_name);

        ret = S_ERR;

        return ret;       
    }
       
    data_size = fread(temp_data, sizeof(char), ENCODE_FILE_SIZE, fp);
    if (ENCODE_FILE_SIZE != data_size)
    {
        testlog("Failed get file[%s] data due to get_size[%llu] is not equal 16MB.",
                file_name, ENCODE_FILE_SIZE, data_size);

        exit(0);     
    }

    fclose(fp);
    
    *file_data = temp_data;

    return ret;
}

int encode_makedir_save_encode_data(char *dir_path)
{
    int ret = S_OK;

    if (NULL == dir_path)
    {
        testlog("Failed to makdir save encode data");

        exit(0);
    }

    ret = mkdir(dir_path, 0777);

    return ret;
}

int encode_check_file_vaild(char *file_name)
{
    int ret = S_OK;
    struct stat file_stat;

    if (NULL == file_name)
    {
        testlog("Filed to check file due to filename is NULL");

        ret = S_ERR;

        return ret;
    }

    memset(&file_stat, 0x0, sizeof(struct stat));
    ret = stat(file_name, &file_stat);
    if (S_OK != ret)
    {
        testlog("Failed to encode due to file:%s is notexist.");

        ret = S_ERR;

        return ret;
    }
    
    if (!S_ISREG(file_stat.st_mode))
    {
        testlog("Failed to encode due to %s is file.");

        ret = S_ERR;

        return ret;        
    }

    if(ENCODE_FILE_SIZE != file_stat.st_size)
    {
        testlog("Failed to encode due to file[%s] size[%llu] is not equal 16M.",
                file_name, file_stat.st_size);

        ret = S_ERR;

        return ret;           
    }
    
    return ret;
}

int main(int argc, char **argv)
{
    int ret = S_OK;
    char *file_name = NULL;
    char *base_name = NULL;
    char *file_data = NULL;    

    if (argc != 2) {
		fprintf(stderr, "usage: my inputfile\n");
		exit(0);
	}
    
    file_name = argv[1];
    //testlog("input filename:%s\n", file_name);

    /*check file*/
    ret = encode_check_file_vaild(file_name);
    if (S_OK != ret)
    {
 		fprintf(stderr, "failed to check inputfile:%s\n", file_name);
		exit(0);   
    }

    /*get file data*/
    ret = encode_get_file_data(&file_data, file_name);
    if (S_OK != ret)
    {
 		fprintf(stderr, "failed to get file_data inputfile:%s\n", file_name);
		exit(0);   
    }

    base_name = basename(file_name);

    /*mkdir to save encode file data*/
    (void)encode_makedir_save_encode_data(SAVE_ENCODE_FILE_PATH);

    init_cauchy_table();
    encode_api_for_file_data(file_data, ENCODE_FILE_SIZE, base_name);
    
    //free(base_name);

    return ret;
}
