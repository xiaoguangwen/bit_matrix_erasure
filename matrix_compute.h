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

#define EC_OK            0
#define MAX_DATA_UNIT    12
#define MAX_PARITY_UNIT  4


#define BIT_WIDTH     4
#define GALOIS_SPACE  16


#define GALOIS_SPACE_2_POWER_4_MAX      15
#define GALOIS_SPACE_IDENTITY_ELEMENT   1
#define GALOIS_SPACE_ZERO_ELEMENT       0


#define KB_SHIFT                      10
#define MILLION_SHIFT                 20

#define KILO_TO_BYTE(size)            ((size)<<KB_SHIFT)
#define BYTE_TO_KILO(size)            ((size)>>KB_SHIFT)

#define MILLION_TO_BYTE(size)         ((size)<<MILLION_SHIFT)
#define BYTE_TO_MILLION(size)         ((size)>>MILLION_SHIFT)


typedef enum ec_error_code
{
    EC_ERROR_CODE_CONFIG_ERR = 20120512,
    EC_ERROR_CODE_NO_FAULT_NODE,        
    EC_ERROR_CODE_EXCEED_MAX_FAULT,
    EC_ERROR_CODE_NOT_GET_MATRIX,
    EC_ERROR_CODE_NO_MEMORY,
    EC_ERROR_CODE_FAIL_OPEN_FILE,
    EC_ERROR_CODE_FAIL_READ_FILE,

}ec_error_code_e;

typedef enum ec_code_type
{
    EC_CODE_TYPE_INIT = 0,
    EC_CODE_TYPE_FULL_WRITE,
    EC_CODE_TYPE_RECONSTRUCT_Write,
    EC_CODE_TYPE_READ_MODIFY_Write,    
    EC_CODE_TYPE_END
}ec_code_type_e;

typedef struct ec_config
{
    ywb_uint8_t du_count;                       /**<数据节点个数，2<=dn_count<=12*/
    ywb_uint8_t count_pu;                       /**<校验节点个数, 1<=count_pn<=3*/
    ywb_uint8_t bit_width;                      /**<默认值是4*4阶，仅仅支持dn_count+count<=16*/
    ywb_uint64_t code_size;                     /**<编码最小单元*/
}ec_config_t;

typedef struct ec_status
{
    ec_config_t config;
    ywb_uint8_t dfault_num;                     
    ywb_uint8_t dfault_array[MAX_DATA_UNIT];    /**<数据节点失效index*/
    ywb_uint8_t num_pfault;
    ywb_uint8_t array_pfault[MAX_PARITY_UNIT];  /**<校验节点失效index*/
}ec_status_t;

typedef enum ec_recover_type
{
    EC_RECOVER_TYPE_INIT = 0,
    EC_RECOVER_TYPE_DATA,                   /**<恢复数据类型*/
    EC_RECOVER_TYPE_PARITY,                     /**<恢复校验类型*/
    EC_RECOVER_TYPE_MAX
}ec_recover_type_e;


typedef struct ec_recover_node
{
    ec_recover_type_e reco_type;                /**<恢复单元的类型，数据或者校验*/
    ywb_uint32_t index;                         /**<恢复单元下标*/
    ywb_uint8_t bitmap_len;                     /**<parity_count乘以data_count+parity_count乘以4*/
    /*bitmap拓扑，以下是最大规格12个date_node，4个parity_node*/
    /*  c3       c2      c1        c0         d11 ------------d0*/
    /*63<->60 | 59<->56 | 55<->52 |  51<->48 |||   47<->44 后面依次类推*/
    ywb_uint64_t recover_bitmap[BIT_WIDTH];     /**<需要参与计算的bitmap*/
}ec_recover_node_t;

typedef struct ec_recover_table
{
    ywb_uint32_t recover_num;
    double cost_sec;
    ec_recover_node_t recover_node[MAX_PARITY_UNIT];
}ec_recover_table_t;

typedef struct ec_lost_matrix
{
    ywb_uint8_t *matrix;
    ywb_uint32_t matrix_line;
    ywb_uint32_t matrix_list;
    ywb_uint8_t *unitization;
    ywb_uint32_t unitization_size;
}ec_lost_matrix_t;

int ec_get_recover_table_by_ec_status(ec_status_t *ec_status, 
                                                ec_recover_table_t *recover_table);


