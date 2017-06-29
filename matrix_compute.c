#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_compute.h"


//{if (test) assert(0);}
#define ec_log(fmt, ...) {\
    syslog(3, "%s_%d:" fmt ".\n", __FUNCTION__, __LINE__,  ##__VA_ARGS__); \
}

#define ec_malloc(size)                 malloc(size)
#define ec_free(prt)                    free(prt)
#define EC_ASSERT(test)                 {if (test) assert(0);}

//#define EC_PRE_SUB_UNIT                 MILLION_TO_BYTE(1)
#define EC_PRE_SUB_UNIT                 32
#define EC_XOR_TEMPLETE(A, B, RESULT) {RESULT = (A) ^ (B);}  
    

static const ywb_int8_t g_2_n_1_m[]  = {1,1};
static const ywb_int8_t g_3_n_1_m[]  = {1,1,1};
static const ywb_int8_t g_4_n_1_m[]  = {1,1,1,1};
static const ywb_int8_t g_5_n_1_m[]  = {1,1,1,1,1};
static const ywb_int8_t g_6_n_1_m[]  = {1,1,1,1,1,1};
static const ywb_int8_t g_7_n_1_m[]  = {1,1,1,1,1,1,1};
static const ywb_int8_t g_8_n_1_m[]  = {1,1,1,1,1,1,1,1};
static const ywb_int8_t g_9_n_1_m[]  = {1,1,1,1,1,1,1,1,1};
static const ywb_int8_t g_10_n_1_m[] = {1,1,1,1,1,1,1,1,1,1};
static const ywb_int8_t g_11_n_1_m[] = {1,1,1,1,1,1,1,1,1,1,1};
static const ywb_int8_t g_12_n_1_m[] = {1,1,1,1,1,1,1,1,1,1,1,1};


static const ywb_int8_t g_3_n_2_m[]  = {1,1,1,   					1,2,9};
static const ywb_int8_t g_4_n_2_m[]  = {1,1,1,1, 					1,2,9,4};
static const ywb_int8_t g_5_n_2_m[]  = {1,1,1,1,1,				    1,2,9,4,8};
static const ywb_int8_t g_6_n_2_m[]  = {1,1,1,1,1,1,				1,2,9,4,8,13,};
static const ywb_int8_t g_7_n_2_m[]  = {1,1,1,1,1,1,1,			    1,2,9,4,8,13,3,};
static const ywb_int8_t g_8_n_2_m[]  = {1,1,1,1,1,1,1,1,			1,2,9,4,8,13,3,6};
static const ywb_int8_t g_9_n_2_m[]  = {1,1,1,1,1,1,1,1,1,		    1,2,9,4,8,13,3,6,12,};
static const ywb_int8_t g_10_n_2_m[] = {1,1,1,1,1,1,1,1,1,1,		1,2,9,4,8,13,3,6,12,5};
static const ywb_int8_t g_11_n_2_m[] = {1,1,1,1,1,1,1,1,1,1,1,	    1,2,9,4,8,13,3,6,12,5,11};
static const ywb_int8_t g_12_n_2_m[] = {1,1,1,1,1,1,1,1,1,1,1,1,	1,2,9,4,8,13,3,6,12,5,11,15};


static const ywb_int8_t g_4_n_3_m[]  = {1,1,1,1,					1,12,8,11,						12,9,1,6};
static const ywb_int8_t g_5_n_3_m[]  = {1,1,1,1,1,				    13,3,2,6,1,						12,9,1,6,3};
static const ywb_int8_t g_6_n_3_m[]  = {1,1,1,1,1,1,				9,6,4,12,2,1,					1,5,10,9,13,6};
static const ywb_int8_t g_7_n_3_m[]  = {1,1,1,1,1,1,1,			    13,3,2,6,1,9,12,				1,5,10,9,13,6,8};
static const ywb_int8_t g_8_n_3_m[]  = {1,1,1,1,1,1,1,1,			9,6,4,12,2,1,11,13,				1,5,10,9,13,6,8,4};	
static const ywb_int8_t g_9_n_3_m[]  = {1,1,1,1,1,1,1,1,1,		    9,6,4,12,2,1,11,13,10,			1,5,10,9,13,6,8,4,3};
static const ywb_int8_t g_10_n_3_m[] = {1,1,1,1,1,1,1,1,1,1,		13,3,2,6,1,9,12,15,5,8,			1,5,10,9,13,6,8,4,3,12};
static const ywb_int8_t g_11_n_3_m[] = {1,1,1,1,1,1,1,1,1,1,1,	    13,3,2,6,1,9,12,15,5,8,4,		1,5,10,9,13,6,8,4,3,12,15};
static const ywb_int8_t g_12_n_3_m[] = {1,1,1,1,1,1,1,1,1,1,1,1,	13,3,2,6,1,9,12,15,5,8,4,10,	1,5,10,9,13,6,8,4,3,12,15,2};


static const ywb_int8_t g_5_n_4_m[]  = {1,1,1,1,1,					8,1,3,9,13,						1,2,12,6,15,					9,7,3,1,8};
static const ywb_int8_t g_6_n_4_m[]  = {1,1,1,1,1,1,				8,1,3,9,13,6,					8,3,10,5,1,13,					4,13,11,8,12,1};
static const ywb_int8_t g_7_n_4_m[]  = {1,1,1,1,1,1,1,				6,4,12,2,1,11,13,				12,11,15,14,8,2,1,				4,13,11,8,12,1,6};
static const ywb_int8_t g_8_n_4_m[]  = {1,1,1,1,1,1,1,1,			3,2,6,1,9,12,15,5,				6,12,14,7,4,1,9,2,				4,13,11,8,12,1,6,9};
static const ywb_int8_t g_9_n_4_m[]  = {1,1,1,1,1,1,1,1,1,			3,2,6,1,9,12,15,5,8,			8,3,10,5,1,13,15,9,2,			4,13,11,8,12,1,6,9,3};
static const ywb_int8_t g_10_n_4_m[] = {1,1,1,1,1,1,1,1,1,1,		8,1,3,9,13,6,14,11,4,2,			8,3,10,5,1,13,15,9,2,11,		8,9,5,3,11,2,12,1,6,13};
static const ywb_int8_t g_11_n_4_m[] = {1,1,1,1,1,1,1,1,1,1,1,		8,1,3,9,13,6,14,11,4,2,5,		6,12,14,7,4,1,9,2,8,10,13,		4,13,11,8,12,1,6,9,3,15,5};
static const ywb_int8_t g_12_n_4_m[] = {1,1,1,1,1,1,1,1,1,1,1,1,	2,13,4,15,14,8,10,6,1,9,12,5,	6,12,14,7,4,1,9,2,8,10,13,11,	4,13,11,8,12,1,6,9,3,15,5,2};	

#if 0
static const ywb_int8_t g_6_n_5_m[]  = {1,1,1,1,1,1,				4,12,2,1,11,13,					7,1,9,12,3,8,					5,4,13,2,7,1,					7,12,5,9,8,1};
static const ywb_int8_t g_7_n_5_m[]  = {1,1,1,1,1,1,1,				2,6,1,9,12,15,5,				12,14,7,4,1,9,2,				13,11,8,12,1,6,9,				1,14,13,3,5,6,4};
static const ywb_int8_t g_8_n_5_m[]  = {1,1,1,1,1,1,1,1,			2,6,1,9,12,15,5,8,				12,14,7,4,1,9,2,8,				13,11,8,12,1,6,9,3,				3,1,4,5,15,10,12,9};
static const ywb_int8_t g_9_n_5_m[]  = {1,1,1,1,1,1,1,1,1,			2,6,1,9,12,15,5,8,4,			6,7,10,2,9,13,1,4,5,			9,5,3,11,2,12,1,6,13,			7,12,5,9,8,1,15,6,2};
static const ywb_int8_t g_10_n_5_m[] = {1,1,1,1,1,1,1,1,1,1,		1,3,9,13,6,14,11,4,2,5,			7,1,9,12,3,8,6,11,13,4,			11,2,15,1,10,9,5,13,12,4,		5,3,12,15,2,13,7,8,9,4};
static const ywb_int8_t g_11_n_5_m[] = {1,1,1,1,1,1,1,1,1,1,1,		2,6,1,9,12,15,5,8,4,10,14,		14,2,1,11,6,3,12,5,9,8,15,		13,11,8,12,1,6,9,3,15,5,2,		5,3,12,15,2,13,7,8,9,4,6};
#endif

static const ywb_uint8_t galois_log_table_2_power_4[GALOIS_SPACE] = {1, 2, 4, 8, 3, 6, 12, 11, 5, 10, 7, 14, 15, 13, 9, 1};
    
static const ywb_uint8_t ilog_galois_table_2_power_4[GALOIS_SPACE] = {0, 0, 1, 4, 2, 8, 5, 10, 3, 14, 9, 7, 6, 13, 11, 12};

static const ywb_uint8_t *matrix_table[MAX_PARITY_UNIT+1][MAX_DATA_UNIT+1] = 
{{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
 {NULL, NULL, g_2_n_1_m, g_3_n_1_m, g_4_n_1_m, g_5_n_1_m, g_6_n_1_m, g_7_n_1_m, g_8_n_1_m, g_9_n_1_m, g_10_n_1_m, g_11_n_1_m, g_12_n_1_m},
 {NULL, NULL, NULL, g_3_n_2_m, g_4_n_2_m, g_5_n_2_m, g_6_n_2_m, g_7_n_2_m, g_8_n_2_m, g_9_n_2_m, g_10_n_2_m, g_11_n_2_m, g_12_n_2_m},
 {NULL, NULL, NULL, NULL, g_4_n_3_m, g_5_n_3_m, g_6_n_3_m, g_7_n_3_m, g_8_n_3_m, g_9_n_3_m, g_10_n_3_m, g_11_n_3_m, g_12_n_3_m},
 {NULL, NULL, NULL, NULL, NULL, g_5_n_4_m, g_6_n_4_m, g_7_n_4_m, g_8_n_4_m, g_9_n_4_m, g_10_n_4_m, g_11_n_4_m, g_12_n_4_m}};

void ec_static_matrix_table_check(void)
{
    ywb_uint32_t line = 0;
    ywb_uint32_t list = 0;
    ywb_uint32_t i = 0;
    ywb_uint32_t matrix_len = 0;
    ywb_uint8_t *temp_matrix = NULL;

    for (line = 0; line < MAX_PARITY_UNIT+1; line++)
    {
        for (list = 0; list < MAX_DATA_UNIT+1; list++)
        {
            if ((0 == line) || (line <= list))
            {
                EC_ASSERT(NULL != matrix_table[line][list]);   
            }
            else
            {
                temp_matrix = (ywb_uint8_t *)matrix_table[line][list];
                EC_ASSERT(NULL == temp_matrix);
                matrix_len = line*list;               
                EC_ASSERT(matrix_len != sizeof(matrix_table[line][list])/sizeof(ywb_uint8_t)); 
                for (i = 0; i < matrix_len; i++)
                {   
                    if (i < list) EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != temp_matrix[i]);
                    EC_ASSERT(GALOIS_SPACE_ZERO_ELEMENT == temp_matrix[i]);
                    EC_ASSERT(temp_matrix[i] > GALOIS_SPACE_2_POWER_4_MAX);
                }
            }
        }
    }

    return;
}

int ec_data_parity_config_check(  ec_config_t *config)
{
    int ret = EC_OK;
    ywb_uint8_t data_unit = 0;
    ywb_uint8_t parity_unit = 0;

    EC_ASSERT(NULL == config);

    data_unit = config->du_count;
    parity_unit = config->count_pu;
    
    if ((1 >= data_unit) || (0 == parity_unit))
    {
        ret = EC_ERROR_CODE_CONFIG_ERR;
    
        ec_log("data unit count[%u] or parity unit count[%u] is zero",
                data_unit, parity_unit);        

        goto abort;        
    }
    
    if (data_unit <= parity_unit)
    {
        ret = EC_ERROR_CODE_CONFIG_ERR;
    
        ec_log("data unit count[%u] less than parity unit count[%u]",
                data_unit, parity_unit);        

        goto abort;
    }

    if ((data_unit > MAX_DATA_UNIT) || (parity_unit > MAX_PARITY_UNIT))
    {
        ret = EC_ERROR_CODE_CONFIG_ERR;
    
        ec_log("data unit count[%u] or parity unit count[%u] is out of specification",
                data_unit, parity_unit);        

        goto abort;        
    }

    EC_ASSERT(BIT_WIDTH != config->bit_width);
    
abort:

    return ret;
}

int ec_status_table_check(ec_status_t *ec_status)
{
    int ret = EC_OK;
    ywb_uint32_t i = 0;
    ywb_uint32_t last_index = 0;
    ywb_uint32_t lost_count = 0;

    EC_ASSERT(NULL == ec_status);

    ret = ec_data_parity_config_check(&ec_status->config);
    if (EC_OK != ret)
    {
        ec_log("Failed to check config table");

        goto abort;
    }

    if ((ec_status->dfault_num > ec_status->config.du_count)
        || (ec_status->num_pfault > ec_status->config.count_pu))
    {
        ret = EC_ERROR_CODE_CONFIG_ERR;

        ec_log("Failed to check status table due to have conflict.\n"
               "\t dfault_num[%u], du_count[%u], num_pfault[%u], count_pu[%u]",
               ec_status->dfault_num, ec_status->config.du_count,
               ec_status->num_pfault, ec_status->config.count_pu);

        goto abort;
    }
    
    if ((ec_status->dfault_num + ec_status->num_pfault) > ec_status->config.count_pu)
    {
        ret = EC_ERROR_CODE_EXCEED_MAX_FAULT;
        
        ec_log("Failed to check status table due to exceed max fault.\n"
               "\t dfault_num[%u], du_count[%u], num_pfault[%u], count_pu[%u]",
               ec_status->dfault_num, ec_status->config.du_count,
               ec_status->num_pfault, ec_status->config.count_pu);

        goto abort;        
    }

    for (i = 0; i < ec_status->dfault_num; i++)
    {
        if (i > 0) EC_ASSERT(last_index >= ec_status->dfault_array[i]);
        EC_ASSERT(ec_status->dfault_array[i] > ec_status->config.du_count);
        last_index = ec_status->dfault_array[i];
        lost_count++;
    }

    for (i = 0; i < ec_status->num_pfault; i++)
    {
        if (i > 0) EC_ASSERT(last_index >= ec_status->array_pfault[i]);
        EC_ASSERT(ec_status->array_pfault[i] > ec_status->config.count_pu);
        last_index = ec_status->array_pfault[i];
        lost_count++;
    }

    if (0 == lost_count)
    {
        ret = EC_ERROR_CODE_NO_FAULT_NODE;
        
        ec_log("Failed to check status table due to no fault node.\n"
               "\t dfault_num[%u], du_count[%u], num_pfault[%u], count_pu[%u]",
               ec_status->dfault_num, ec_status->config.du_count,
               ec_status->num_pfault, ec_status->config.count_pu);

        goto abort;   
    }
       
abort:

    return ret;
}

ywb_uint8_t *ec_get_encode_matrix_table(ywb_uint8_t data_num, ywb_uint8_t parity_num)
{
    EC_ASSERT(data_num > MAX_DATA_UNIT);
    EC_ASSERT(parity_num > MAX_PARITY_UNIT);
    
    return (ywb_uint8_t *)matrix_table[parity_num][data_num];
}

void ec_create_recover_matrix_reset_lost_list(ec_lost_matrix_t *lost_matrix)
{
    ywb_int32_t i = 0;
    ywb_int32_t j = 0;
    ywb_uint32_t matrix_line = lost_matrix->matrix_line;
    ywb_uint32_t matrix_list = lost_matrix->matrix_list;
    ywb_uint8_t *unitization = lost_matrix->unitization;
    ywb_uint32_t unitization_size = lost_matrix->unitization_size;
    ywb_uint8_t *change_matrix = lost_matrix->matrix;
    ywb_uint8_t matrix_vaule = 0;
    
    EC_ASSERT(matrix_line >= matrix_list);
    
    /*check，检查失效节点矩阵是否被初单位化单位矩阵*/
    for (i = 0; i < unitization_size; i++)
    {
        for (j = 0; j < unitization_size; j++)
        {
            matrix_vaule = change_matrix[i*matrix_list + unitization[j]];

            if (i == j) 
            {
                EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);
                change_matrix[i*matrix_list + unitization[j]] = 0;
            }
            else
            {
                EC_ASSERT(GALOIS_SPACE_ZERO_ELEMENT != matrix_vaule); 
            }
        }
    }
    
    return;
}


void ec_create_recover_matrix_down_triangulation(ec_lost_matrix_t *lost_matrix)
{
    ywb_int32_t i = 0;
    ywb_int32_t j = 0;
    ywb_int32_t k = 0;
    ywb_uint32_t matrix_line = lost_matrix->matrix_line;
    ywb_uint32_t matrix_list = lost_matrix->matrix_list;
    ywb_uint32_t unitization_size = lost_matrix->unitization_size;
    ywb_uint8_t *unitization = lost_matrix->unitization;
    ywb_uint8_t *change_matrix = lost_matrix->matrix;
    ywb_uint8_t matrix_vaule = 0;
    ywb_uint8_t galois_log = 0;
    ywb_uint8_t inv_galois_log = 0;

    EC_ASSERT(NULL == change_matrix);
    EC_ASSERT(NULL == unitization);
    EC_ASSERT(matrix_line >= matrix_list);
    EC_ASSERT(unitization_size > matrix_line);

    for (i = 0; i < unitization_size; i++)
    {
        /*第j个失效列单位化,，同时完成行的初等变换*/
        for (j = 0; j < unitization_size - i; j++)
        {
            matrix_vaule = change_matrix[(j+i)*matrix_list + unitization[i]];
            EC_ASSERT((0 == j) && (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule));
            if (GALOIS_SPACE_IDENTITY_ELEMENT == matrix_vaule) continue;            
            if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;

            EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < matrix_vaule);
            galois_log = ilog_galois_table_2_power_4[matrix_vaule];
            inv_galois_log = GALOIS_SPACE_2_POWER_4_MAX - galois_log;

            for (k = 0; k < matrix_list; k++)
            {
                matrix_vaule = change_matrix[(j+i)*matrix_list + k];
                if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;

                galois_log = ilog_galois_table_2_power_4[matrix_vaule];
                galois_log = (galois_log + inv_galois_log) % GALOIS_SPACE_2_POWER_4_MAX;
                change_matrix[(j+i)*matrix_list + k] = galois_log_table_2_power_4[galois_log];
            }
            matrix_vaule = change_matrix[(j+i)*matrix_list + unitization[i]];
            EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);
        }    

        /*第j个失效列消元，同时完成行的初等变换*/
        for (j = 1; j < unitization_size - i; j++)
        {
            matrix_vaule = change_matrix[(j+i)*matrix_list + unitization[i]];
            if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;
            EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);

            for (k = 0; k < matrix_list; k++)
            {
                EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < change_matrix[(i+j)*matrix_list + k]);
                EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < change_matrix[i*matrix_list + k]);

                change_matrix[(i+j)*matrix_list + k] ^= change_matrix[i*matrix_list + k];

                EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < change_matrix[(i+j)*matrix_list + k]);
            }

            matrix_vaule = change_matrix[(i+j)*matrix_list + unitization[i]];
            EC_ASSERT(GALOIS_SPACE_ZERO_ELEMENT != matrix_vaule);
        }
    }

    return;
}

void ec_create_recover_matrix_up_triangulation(ec_lost_matrix_t *lost_matrix)
{
    ywb_int32_t i = 0;
    ywb_int32_t j = 0;
    ywb_int32_t k = 0;
    ywb_uint32_t matrix_line = lost_matrix->matrix_line;
    ywb_uint32_t matrix_list = lost_matrix->matrix_list;
    ywb_uint8_t *unitization = lost_matrix->unitization;
    ywb_uint32_t unitization_size = lost_matrix->unitization_size;
    ywb_uint8_t *change_matrix = lost_matrix->matrix;
    ywb_uint8_t matrix_vaule = 0;
    ywb_uint8_t galois_log = 0;
    ywb_uint8_t inv_galois_log = 0;

    EC_ASSERT(NULL == lost_matrix);
    EC_ASSERT(NULL == change_matrix);
    EC_ASSERT(NULL == unitization);
    EC_ASSERT(unitization_size > matrix_line);
    EC_ASSERT(matrix_line >= matrix_list);

    /*check，失效节点子矩阵是否被下三角化*/
    for (i = 0; i < unitization_size; i++)
    {
        matrix_vaule = change_matrix[i*matrix_list + unitization[i]];
        EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);
        for (j = 1; j < unitization_size - i; j++)
        {
            matrix_vaule = change_matrix[(j+i)*matrix_list + unitization[i]];
            EC_ASSERT(GALOIS_SPACE_ZERO_ELEMENT != matrix_vaule);
        }
    }

    /*失效矩阵对角化,同时行完成伽罗瓦域初等变换*/
    for (i = unitization_size - 1; i >= 0; i--)
    {
        /*单位化*/
        for (j = i; j >= 0; j--)
        {
            matrix_vaule = change_matrix[j*matrix_list + unitization[i]];
            EC_ASSERT((i == j) && (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule));
            if (GALOIS_SPACE_IDENTITY_ELEMENT == matrix_vaule) continue;            
            if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;

            EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < matrix_vaule);
            galois_log = ilog_galois_table_2_power_4[matrix_vaule];
            inv_galois_log = GALOIS_SPACE_2_POWER_4_MAX - galois_log;

            for (k = 0; k < matrix_list; k++)
            {
                matrix_vaule = change_matrix[j*matrix_list + k];
                if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;

                galois_log = ilog_galois_table_2_power_4[matrix_vaule];
                galois_log = (galois_log + inv_galois_log) % GALOIS_SPACE_2_POWER_4_MAX;
                change_matrix[j*matrix_list + k] = galois_log_table_2_power_4[galois_log];
            }
            matrix_vaule = change_matrix[j*matrix_list + unitization[i]];
            EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);
        }    

        /*消原*/
        for (j = 0; j < i; j++)
        {
            matrix_vaule = change_matrix[j*matrix_list + unitization[i]];
            if (GALOIS_SPACE_ZERO_ELEMENT == matrix_vaule) continue;
            EC_ASSERT(GALOIS_SPACE_IDENTITY_ELEMENT != matrix_vaule);

            for (k = 0; k < matrix_list; k++)
            {
                EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < change_matrix[j*matrix_list + k]);
                EC_ASSERT(GALOIS_SPACE_2_POWER_4_MAX < change_matrix[i*matrix_list + k]);

                change_matrix[j*matrix_list + k] ^= change_matrix[i*matrix_list + k] ;
            }
            
            matrix_vaule = change_matrix[j*matrix_list + unitization[i]];
            EC_ASSERT(GALOIS_SPACE_ZERO_ELEMENT != matrix_vaule);
        }
    }

    return;    
}

void ec_create_recover_matrix_by_unitization_list(ec_lost_matrix_t *lost_matrix)
{
    /*失效节点上三角化，在伽罗瓦域运算*/
    ec_create_recover_matrix_down_triangulation(lost_matrix);

    /*失效节点下三角归零*/
    ec_create_recover_matrix_up_triangulation(lost_matrix);

    /*失效节点列和行清0*/
    ec_create_recover_matrix_reset_lost_list(lost_matrix);
    
    return;
}
                                                             
int ec_prepare_get_recover_table(ec_status_t *ec_status,
                                             ec_lost_matrix_t *lost_matrix)
{
    int ret = EC_OK;    
    ywb_uint32_t line_matrix = 0;    
    ywb_uint32_t list_matrix = 0;
    ywb_uint8_t *encode_matrix = NULL;
    ywb_uint8_t *recover_matrix = NULL;
    ywb_uint32_t i = 0;
    ywb_uint8_t *unitization_list = NULL;
    ywb_uint8_t unitization_len = 0;

    EC_ASSERT(NULL == lost_matrix);
    
    ret = ec_status_table_check(ec_status);
    if (EC_OK != ret)
    {
        ec_log("Failed to check ec status table");

        goto abort;
    }

    encode_matrix = ec_get_encode_matrix_table(ec_status->config.du_count, ec_status->config.count_pu);
    if (NULL == encode_matrix)
    {
        ret = EC_ERROR_CODE_NOT_GET_MATRIX;

        ec_log("Failed to get recover table due to fail to get encode matrix, du_count[%u], pu_count[%u]",
            ec_status->config.du_count, ec_status->config.count_pu);

        goto abort;
    }
     
    line_matrix = ec_status->config.count_pu;
    list_matrix = ec_status->config.du_count + ec_status->config.count_pu;
    recover_matrix = (ywb_uint8_t *)ec_malloc(line_matrix*list_matrix*sizeof(ywb_uint8_t));
    if (NULL == recover_matrix)
    {
        ret = EC_ERROR_CODE_NO_MEMORY;

        ec_log("Failed to get recover table due to fail to get memory, du_count[%u], pu_count[%u]",
            ec_status->config.du_count, ec_status->config.count_pu);

        goto abort;
    }
    memset(recover_matrix, 0x0, line_matrix*list_matrix*sizeof(ywb_uint8_t));
    
    for (i = 0; i < line_matrix; i++)
    {
        memcpy(recover_matrix + (i*list_matrix), 
        encode_matrix + (i*ec_status->config.du_count),
        ec_status->config.du_count*sizeof(ywb_uint8_t));
        recover_matrix[(i+1)*list_matrix - ec_status->config.count_pu + i] = 1;  
    }

    /*参数检查*/
    for (i= 0; i < ec_status->dfault_num; i++)
    {
        EC_ASSERT(1 !=recover_matrix[ec_status->dfault_array[i]]);
    }

    unitization_list = ec_malloc(ec_status->config.count_pu*sizeof(ywb_uint8_t));
    if (NULL == unitization_list)
    {
        ret = EC_ERROR_CODE_NO_MEMORY; 
            
        ec_log("Failed to prepare to get recover table due to memory[%u].\n", 
                ec_status->config.count_pu);

        goto abort;
    }
    memset(unitization_list, 0x0, ec_status->config.count_pu*sizeof(ywb_uint8_t));
    
    /*标记需要对角化的列下标*/
    for (i = 0; i < ec_status->dfault_num; i++)
    {
        EC_ASSERT(unitization_len > ec_status->config.count_pu);
        unitization_list[unitization_len] = ec_status->dfault_array[i];
        unitization_len++;
    }

    for (i = 0; i < ec_status->num_pfault; i++)
    {
        EC_ASSERT(unitization_len > ec_status->config.count_pu);
        unitization_list[unitization_len] = ec_status->array_pfault[i] + ec_status->config.du_count;
        unitization_len++;
    }   
    
    lost_matrix->matrix = recover_matrix;
    lost_matrix->matrix_line = line_matrix;
    lost_matrix->matrix_list = list_matrix;
    lost_matrix->unitization = unitization_list;
    lost_matrix->unitization_size = unitization_len;

    return ret;
    
abort:

    if (NULL != recover_matrix)
    {
        ec_free(recover_matrix);
        recover_matrix = NULL;
    }

    if (NULL != unitization_list)
    {
        ec_free(unitization_list);
        unitization_list = NULL;
    }

    return ret;
}


void ec_lost_matrix_free(ec_lost_matrix_t *lost_matrix)
{
    if (NULL != lost_matrix)
    {
        if (NULL != lost_matrix->matrix)
        {
            ec_free(lost_matrix->matrix);
            lost_matrix->matrix = NULL;
        }

        if (NULL != lost_matrix->unitization)
        {
            ec_free(lost_matrix->unitization);
            lost_matrix->unitization = NULL;
        }
    }

    return;
}

void ec_create_recover_table_by_lost_matrix(ec_lost_matrix_t *lost_matrix,
                                                            ec_recover_table_t *recover_table)
{
    ywb_int32_t i = 0;
    ywb_int32_t j = 0;    
    ywb_int32_t k = 0;
    ywb_uint32_t parity_start = 0;
    ywb_uint8_t matrix_vaule = 0;
    ywb_uint8_t galois_log = 0;
    ywb_uint8_t inv_galois_log = 0;
    ywb_uint8_t *matrix = NULL;
    
    EC_ASSERT(NULL == lost_matrix);
    EC_ASSERT(NULL == recover_table);    
    EC_ASSERT(NULL == lost_matrix->matrix);
    EC_ASSERT(NULL == lost_matrix->unitization);
    EC_ASSERT(0 == lost_matrix->unitization_size);
    EC_ASSERT(lost_matrix->unitization_size > MAX_PARITY_UNIT);
    EC_ASSERT(lost_matrix->matrix_list > (MAX_DATA_UNIT + MAX_PARITY_UNIT));

    recover_table->recover_num = lost_matrix->unitization_size;    
    parity_start = lost_matrix->matrix_list - lost_matrix->matrix_line;
    matrix = lost_matrix->matrix;

    for (i = 0; i < recover_table->recover_num; i++)
    {
        if (lost_matrix->unitization[i] < parity_start)
        {
            recover_table->recover_node[i].reco_type = EC_RECOVER_TYPE_DATA;
            recover_table->recover_node[i].index = lost_matrix->unitization[i];
            recover_table->recover_node[i].bitmap_len = lost_matrix->matrix_list*BIT_WIDTH;
        }
        else
        {
            recover_table->recover_node[i].reco_type = EC_RECOVER_TYPE_PARITY;
            recover_table->recover_node[i].index = lost_matrix->unitization[i] - parity_start;
            recover_table->recover_node[i].bitmap_len = lost_matrix->matrix_list*BIT_WIDTH;
        }

        for (j = 0; j < BIT_WIDTH; j++)
        {
            recover_table->recover_node[i].recover_bitmap[j] = 0;
            for (k = lost_matrix->matrix_list - 1; k >= 0; k--)
            {
                if (k != lost_matrix->matrix_list - 1) 
                {
                    recover_table->recover_node[i].recover_bitmap[j] <<= BIT_WIDTH;
                }
                
                matrix_vaule = matrix[i*lost_matrix->matrix_list + k];
                if (0 != matrix_vaule)
                {
                    galois_log = ilog_galois_table_2_power_4[matrix_vaule];
                    galois_log = (galois_log + j) % GALOIS_SPACE_2_POWER_4_MAX;
                    matrix_vaule = galois_log_table_2_power_4[galois_log];                         
                }
                recover_table->recover_node[i].recover_bitmap[j] |= matrix_vaule;
            }
        }
    }
    
    return;    
}

void ec_create_recover_matrix_by_uncorrelation_lost_matrix(ec_status_t *ec_status,
                                                                          ec_lost_matrix_t *lost_matrix)
{
    ywb_uint32_t i = 0;
    ywb_uint32_t j = 0;
    ywb_uint32_t src_line = 0;
    ywb_uint32_t dst_line = 0;
    ywb_uint32_t matrix_line = lost_matrix->matrix_line;
    ywb_uint32_t matrix_list = lost_matrix->matrix_list;
    ywb_uint8_t *change_matrix = lost_matrix->matrix;
    ywb_uint8_t matrix_vaule = 0;
    
    EC_ASSERT(NULL == ec_status);
    EC_ASSERT(NULL == lost_matrix);

    if (ec_status->num_pfault > 0)
    {
        for (i = 0; i < ec_status->num_pfault; i++)
        {
            if (ec_status->dfault_num + i < ec_status->array_pfault[i])
            {
                src_line = ec_status->dfault_num + i;
                dst_line = ec_status->array_pfault[i];

                for (j = 0; j < matrix_list; j++)
                {
                    matrix_vaule = change_matrix[src_line*matrix_list + j];
                    change_matrix[src_line*matrix_list + j] = change_matrix[dst_line*matrix_list + j];
                    change_matrix[dst_line*matrix_list + j] = matrix_vaule;
                }
            }
        }
    }

    return;
}

int ec_get_recover_table_by_ec_status(ec_status_t *ec_status, ec_recover_table_t *recover_table)
{
    int ret = EC_OK;
    ec_recover_table_t temp_table;
    char *encode_matrix = NULL;
    ywb_uint8_t *recover_matrix = NULL;
    ywb_uint32_t recover_line = 0;
    ywb_uint32_t recover_list = 0;
    ywb_uint8_t *unitization_list = NULL;
    ywb_uint32_t unitization_size = 0;
    ec_lost_matrix_t lost_matrix;
    struct timeval time_start, time_end;
    struct timezone time_zone;

    gettimeofday(&time_start, &time_zone);
    
    EC_ASSERT(NULL == ec_status);
    EC_ASSERT(NULL == recover_table);
    
    memset(&temp_table, 0x0, sizeof(ec_recover_table_t));
    memset(&lost_matrix, 0x0, sizeof(ec_lost_matrix_t));

    ret = ec_prepare_get_recover_table(ec_status, &lost_matrix);
    if (EC_OK != ret)
    {
        ec_log("Failed to get recover table due to fail to prepare");

        goto abort;
    }

    /*行交换，让参与计算的失效节点矩阵行线性无关*/
    ec_create_recover_matrix_by_uncorrelation_lost_matrix(ec_status, &lost_matrix);

    /*失效矩阵单位化，完成其他行初等变化(计算都是在伽罗瓦域进行)*/
    ec_create_recover_matrix_by_unitization_list(&lost_matrix);

    /*生成失效节点恢复信息*/
    ec_create_recover_table_by_lost_matrix(&lost_matrix, &temp_table);
    memcpy(recover_table, &temp_table, sizeof(ec_recover_table_t));

    gettimeofday(&time_end, &time_zone);
    recover_table->cost_sec = 0.0;
    recover_table->cost_sec += time_end.tv_usec;
    recover_table->cost_sec -= time_start.tv_usec;
    recover_table->cost_sec /= 1000000.0;
    recover_table->cost_sec += time_end.tv_sec;
    recover_table->cost_sec -= time_start.tv_sec;
    
abort:

    ec_lost_matrix_free(&lost_matrix);
    
    return ret;
}

void ec_string_reversion(ywb_uint8_t *bit_string, ywb_uint32_t string_len)
{
    ywb_uint32_t i = 0;
    ywb_uint8_t temp = 0;
    
    for (i  = 0; i < string_len / 2; i++)
    {
        temp = bit_string[i];
        bit_string[i] = bit_string[string_len - i - 1];
        bit_string[string_len - i - 1] = temp;
    }

    return;
}

ywb_uint8_t *ec_to_bit_by_len(ywb_uint64_t num, ywb_uint8_t *bit_string, ywb_uint32_t map_len) 
{
    ywb_int32_t i = 0;
    ywb_uint32_t str_len = 0;
    ywb_uint64_t temp_num = num;
    ywb_uint8_t temp = 0; 

    memset(bit_string, 0x0, map_len + 1);
    
    for (i = 0; i < map_len; i++)
    {
        if (i && (0 == (i % 4))) bit_string[str_len++] = ' ';
        
        temp = (temp_num & 0x01) + 0x30;
        bit_string[str_len++] = temp;
        
        temp_num >>= 1;
    }

    ec_string_reversion(bit_string, str_len);
    
    return bit_string;
}

void ec_printf_recover_table(ec_status_t *ec_status, ec_recover_table_t *recover_table)
{
    ywb_uint32_t i = 0;
    ywb_uint8_t bit_string0[65+64/4];    
    ywb_uint8_t bit_string1[65+64/4];    
    ywb_uint8_t bit_string2[65+64/4];    
    ywb_uint8_t bit_string3[65+64/4];

    printf("ec_status:d_count:%u, p_count_%u, d_fault:%u, p_fault:%u, cost_time:%f(us).\n",
            ec_status->config.du_count, ec_status->config.count_pu,
            ec_status->dfault_num, ec_status->num_pfault, recover_table->cost_sec*1000000);

#if 0
    for (i = 0; i < recover_table->recover_num; i++)
    {
        printf("\t type[%s%s], index:%u,\n",
                EC_RECOVER_TYPE_DATA == recover_table->recover_node[i].reco_type ? "data":"",
                EC_RECOVER_TYPE_PARITY == recover_table->recover_node[i].reco_type ? "parity":"",
                recover_table->recover_node[i].index);
    }
#endif
    
#if 1            
    for (i = 0; i < recover_table->recover_num; i++)
    {
        printf("\t fault_type:%s%s, index:%u, map_len:%u\n"
               "\t\t bit_map1:%s\n"
               "\t\t bit_map2:%s\n"
               "\t\t bit_map3:%s\n"
               "\t\t bit_map4:%s\n",
               EC_RECOVER_TYPE_DATA == recover_table->recover_node[i].reco_type ? "data":"",
               EC_RECOVER_TYPE_PARITY == recover_table->recover_node[i].reco_type ? "parity":"",
               recover_table->recover_node[i].index,
               recover_table->recover_node[i].bitmap_len,
               ec_to_bit_by_len(recover_table->recover_node[i].recover_bitmap[0], bit_string0, recover_table->recover_node[i].bitmap_len),
               ec_to_bit_by_len(recover_table->recover_node[i].recover_bitmap[1], bit_string1, recover_table->recover_node[i].bitmap_len),
               ec_to_bit_by_len(recover_table->recover_node[i].recover_bitmap[2], bit_string2, recover_table->recover_node[i].bitmap_len),
               ec_to_bit_by_len(recover_table->recover_node[i].recover_bitmap[3], bit_string3, recover_table->recover_node[i].bitmap_len));
    }
#endif

    return;    
}

void ec_xor_orig_and_dst_data(ywb_uint8_t *orig, ywb_uint8_t *dst, ywb_uint64_t data_len)
{
    ywb_uint64_t *p_data_src = (ywb_uint64_t *)orig;
    ywb_uint64_t *p_data_dst = ( ywb_uint64_t *)dst;
    ywb_uint64_t change_length = data_len / sizeof(ywb_uint64_t);
    ywb_uint64_t i = 0;
    
    EC_ASSERT(data_len < sizeof(ywb_uint64_t));
    EC_ASSERT(0 != data_len%sizeof(ywb_uint64_t));
    
    for (i = 0; i < change_length ; i++)
    {
        EC_XOR_TEMPLETE(p_data_src[i], p_data_dst[i], p_data_src[i]);    
    }

    return;
}

void ec_get_recover_data_by_recover_node(ec_status_t *ec_status, ec_recover_node_t *recover_node,
                                                       ywb_uint8_t *orig_data, ywb_uint64_t data_len,
                                                       ywb_uint8_t *recover_data, ywb_uint64_t recover_len)
{
    ywb_uint32_t i = 0;
    ec_recover_node_t node;   
    ywb_uint64_t data_skip = 0;
    ywb_uint8_t *dst_data = NULL;

    EC_ASSERT(NULL == ec_status);
    EC_ASSERT(NULL == recover_node);
    EC_ASSERT(NULL == orig_data);
    EC_ASSERT(NULL == recover_data);
    EC_ASSERT(data_len != ((ec_status->config.count_pu + ec_status->config.du_count) * BIT_WIDTH * EC_PRE_SUB_UNIT));    
    EC_ASSERT(recover_node->bitmap_len != (ec_status->config.count_pu + ec_status->config.du_count)*BIT_WIDTH);

    memcpy(&node, recover_node, sizeof(ec_recover_node_t));
    
    for (i = 0 ; i < recover_node->bitmap_len; i++)
    {
        dst_data = orig_data + i*EC_PRE_SUB_UNIT;
        
        if(node.recover_bitmap[0]&0x01) ec_xor_orig_and_dst_data(recover_data, dst_data, EC_PRE_SUB_UNIT);
        if(node.recover_bitmap[1]&0x01) ec_xor_orig_and_dst_data(recover_data+EC_PRE_SUB_UNIT, dst_data, EC_PRE_SUB_UNIT);   
        if(node.recover_bitmap[2]&0x01) ec_xor_orig_and_dst_data(recover_data+2*EC_PRE_SUB_UNIT, dst_data, EC_PRE_SUB_UNIT);   
        if(node.recover_bitmap[3]&0x01) ec_xor_orig_and_dst_data(recover_data+3*EC_PRE_SUB_UNIT, dst_data, EC_PRE_SUB_UNIT);   

        node.recover_bitmap[0] >>= 1;
        node.recover_bitmap[1] >>= 1;
        node.recover_bitmap[2] >>= 1;
        node.recover_bitmap[3] >>= 1;
    }

    return;
}

int ec_get_recover_data_by_recover_table(ec_status_t *ec_status, ec_recover_table_t *recover_table, 
                                                        ywb_uint8_t *orig_data, ywb_uint64_t data_len,
                                                        ywb_uint8_t **recover_data, ywb_uint64_t *recover_len)
{
    int ret = EC_OK;    
    ywb_uint8_t *temp_data = NULL;
    ywb_uint64_t temp_len = 0; 
    ywb_uint32_t i = 0;
    ywb_uint64_t data_skip = 0;

    EC_ASSERT(NULL == ec_status);
    EC_ASSERT(NULL == recover_table);
    EC_ASSERT(NULL == orig_data);
    EC_ASSERT(NULL == recover_data);
    EC_ASSERT(data_len != ((ec_status->config.count_pu + ec_status->config.du_count) * BIT_WIDTH * EC_PRE_SUB_UNIT));
    
    temp_len = recover_table->recover_num * BIT_WIDTH * EC_PRE_SUB_UNIT;
    temp_data = ec_malloc(temp_len);
    if (NULL == temp_data)
    {
        ret = EC_ERROR_CODE_NO_MEMORY;
    
        ec_log("Failed to malloc memory when get recover data, recover_len[%llu]", temp_len);

        goto abort;        
    }
    memset(temp_data, 0x0, temp_len);

    for (i = 0; i < recover_table->recover_num; i++)
    {
        data_skip =  (i * BIT_WIDTH * EC_PRE_SUB_UNIT);
        ec_get_recover_data_by_recover_node(ec_status, &recover_table->recover_node[i],
                                            orig_data, data_len,
                                            temp_data + data_skip, BIT_WIDTH * EC_PRE_SUB_UNIT);  
    }    

    *recover_data = temp_data;
    *recover_len = temp_len;

abort:

    return ret;
}

int ec_create_parity_data_by_recover_table(ywb_uint32_t n, ywb_uint32_t m, 
                                                      ywb_uint8_t *test_data, ywb_uint64_t data_len)
{
    int ret = EC_OK;
    ywb_uint32_t i = 0;
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *recover_data = NULL;
    ywb_uint64_t recover_len = NULL;
    ywb_uint64_t src_data_skip = 0;
    ywb_uint64_t recov_data_skip = 0;

    EC_ASSERT(n <= m);
    EC_ASSERT(data_len != ((m + n)*BIT_WIDTH * EC_PRE_SUB_UNIT));
    EC_ASSERT(NULL == test_data);

    memset(&ec_status, 0x0, sizeof(ec_status_t));
    memset(&recover_table, 0x0, sizeof(ec_recover_table_t));

    ec_status.config.bit_width = BIT_WIDTH;
    ec_status.config.du_count = n;
    ec_status.config.count_pu = m;
    ec_status.dfault_num = 0;
    ec_status.num_pfault = m;
    for (i = 0; i < ec_status.num_pfault; i++)
    {
        ec_status.array_pfault[i] = i;
    }
    
    ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
    EC_ASSERT(EC_OK != ret);

    ret = ec_get_recover_data_by_recover_table(&ec_status, &recover_table,
                                               test_data, data_len, 
                                               &recover_data, &recover_len);
    if (EC_OK != ret)
    {
        ec_log("Failed to get recover data");

        goto abort;
    }
                                               
    for (i = 0; i < recover_table.recover_num; i++)
    {
        src_data_skip = (ec_status.config.du_count + i) * BIT_WIDTH * EC_PRE_SUB_UNIT;
        recov_data_skip = i * BIT_WIDTH * EC_PRE_SUB_UNIT;
        memcpy(test_data + src_data_skip, recover_data + recov_data_skip,
               BIT_WIDTH * EC_PRE_SUB_UNIT);
    }        

abort:

    if (NULL != recover_data)
    {
        ec_free(recover_data);
        recover_data = NULL;
    }

    return ret;
}

int ec_create_m_n_data_and_complute_parity(ywb_uint32_t n, ywb_uint32_t m, 
                                                           ywb_uint8_t **test_data, ywb_uint64_t *data_len)
{
    int ret = EC_OK;
    ywb_uint8_t *temp_data = NULL;
    ywb_uint64_t temp_len = 0;
    FILE *fp = NULL;
    char *file_name = "/dev/urandom";
    ywb_uint64_t read_size = 0;

    EC_ASSERT(n <= m);

    temp_len = (m + n) * BIT_WIDTH * EC_PRE_SUB_UNIT;
    temp_data = ec_malloc(temp_len);
    EC_ASSERT(NULL == test_data);
    memset(temp_data, 0x0, temp_len);

	fp = fopen(file_name, "rb");
    if (NULL == fp) 
    {
        ec_log("Failed open file:%s", file_name);

        ret = EC_ERROR_CODE_FAIL_OPEN_FILE;
    
        goto abort;
    }
       
    read_size = fread(temp_data, sizeof(char), temp_len, fp);
    if (temp_len != read_size)
    {
        ec_log("Failed get file[%s] data due to get_size[%llu] is not equal need_size[%llu].",
                file_name, read_size, temp_len);
    
        ret = EC_ERROR_CODE_FAIL_OPEN_FILE;

        fclose(fp);

        goto abort;
    }

    fclose(fp);

    ret = ec_create_parity_data_by_recover_table(n, m, temp_data, temp_len);
    if (EC_OK != ret)
    {
        ec_log("Failed to get recover table, m[%u], n[%u]", m, n);

        goto abort;
    }
    
    *test_data = temp_data;
    *data_len = temp_len;

    return ret;
    
abort:

    if (NULL != temp_data)
    {
        ec_free(temp_data);
        temp_data = NULL;
    }

    return ret;
}

void ec_exhaustion_n_m_fault_is_equal_1(ywb_uint32_t n , ywb_uint32_t m, 
                                                  ywb_uint8_t *data, ywb_uint64_t data_len)
{
    int ret = EC_OK;
    ywb_uint32_t all_node = n + m;
    ywb_uint32_t i = 0;
    ywb_uint32_t index = 0;
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *recover_data = NULL;
    ywb_uint64_t recover_len = 0;
    ywb_uint64_t data_skip = 0;
    ywb_uint64_t recover_skip = 0;

    EC_ASSERT(n <= m);
    EC_ASSERT(data_len != ((m + n)*BIT_WIDTH * EC_PRE_SUB_UNIT));

    for (i = 0; i < all_node; i++)
    {
        memset(&ec_status, 0x0, sizeof(ec_status_t));
        memset(&recover_table, 0x0, sizeof(ec_recover_table_t));
        ec_status.config.bit_width = BIT_WIDTH;
        ec_status.config.du_count = n;
        ec_status.config.count_pu = m;
        if (i < n)  
        { ec_status.dfault_array[ec_status.dfault_num++] = i;}
        else 
        {ec_status.array_pfault[ec_status.num_pfault++] = i%n;}
        
        ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
        EC_ASSERT(EC_OK != ret);
        
        EC_ASSERT(1 != recover_table.recover_num);        
        ec_printf_recover_table(&ec_status, &recover_table);
        ret = ec_get_recover_data_by_recover_table(&ec_status, &recover_table,
                                                   data, data_len, &recover_data, &recover_len);
        EC_ASSERT(EC_OK != ret);
        
        for (index = 0; index < recover_table.recover_num; index++)
        {
            data_skip = EC_RECOVER_TYPE_DATA == recover_table.recover_node[index].reco_type ? \
                        recover_table.recover_node[index].index :\
                        recover_table.recover_node[index].index + n;
            data_skip = data_skip * BIT_WIDTH * EC_PRE_SUB_UNIT;
            recover_skip = index * BIT_WIDTH * EC_PRE_SUB_UNIT;
            
            ret = memcmp(data + data_skip, recover_data + recover_skip, BIT_WIDTH * EC_PRE_SUB_UNIT);
            EC_ASSERT(0 != ret);
        }
        
        if (NULL != recover_data)
        {
            ec_free(recover_data);
            recover_data = NULL;
        }
    }

    return;
}

void ec_exhaustion_n_m_fault_less_than_equal_2(ywb_uint32_t n , ywb_uint32_t m, 
                                                                ywb_uint8_t *data, ywb_uint64_t data_len)
{
    int ret = EC_OK;
    ywb_uint32_t all_node = n + m;
    ywb_uint32_t i = 0;
    ywb_uint32_t j = 0;
    ywb_uint32_t index = 0;
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *recover_data = NULL;
    ywb_uint64_t recover_len = 0;
    ywb_uint64_t data_skip = 0;
    ywb_uint64_t recover_skip = 0;

    EC_ASSERT(n <= m);
    EC_ASSERT(data_len != ((m + n)*BIT_WIDTH * EC_PRE_SUB_UNIT));

    for (i = 0; i < all_node; i++)
    {
        for (j = i + 1; j < all_node; j++)
        {
            memset(&ec_status, 0x0, sizeof(ec_status_t));
            memset(&recover_table, 0x0, sizeof(ec_recover_table_t));
            ec_status.config.bit_width = BIT_WIDTH;
            ec_status.config.du_count = n;
            ec_status.config.count_pu = m;
            if (i < n)  
            { ec_status.dfault_array[ec_status.dfault_num++] = i;}
            else 
            {ec_status.array_pfault[ec_status.num_pfault++] = i%n;}
            
            if (j < n)  
            { ec_status.dfault_array[ec_status.dfault_num++] = j;}
            else 
            {ec_status.array_pfault[ec_status.num_pfault++] = j%n;}
            
            ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
            EC_ASSERT(EC_OK != ret);
            
            EC_ASSERT(2 != recover_table.recover_num);
            ec_printf_recover_table(&ec_status, &recover_table);
            ret = ec_get_recover_data_by_recover_table(&ec_status, &recover_table,
                                                       data, data_len, &recover_data, &recover_len);
            EC_ASSERT(EC_OK != ret);
            
            for (index = 0; index < recover_table.recover_num; index++)
            {
                data_skip = EC_RECOVER_TYPE_DATA == recover_table.recover_node[index].reco_type ? \
                            recover_table.recover_node[index].index :\
                            recover_table.recover_node[index].index + n;
                data_skip = data_skip * BIT_WIDTH * EC_PRE_SUB_UNIT;
                recover_skip = index * BIT_WIDTH * EC_PRE_SUB_UNIT;
                
                ret = memcmp(data + data_skip, recover_data + recover_skip, BIT_WIDTH * EC_PRE_SUB_UNIT);
                EC_ASSERT(0 != ret);
            }
                            
            if (NULL != recover_data)
            {
                ec_free(recover_data);
                recover_data = NULL;
            }
        }
    }

    ec_exhaustion_n_m_fault_is_equal_1(n, m, data, data_len);

    return;
}

void ec_exhaustion_n_m_fault_less_than_equal_3(ywb_uint32_t n , ywb_uint32_t m, 
                                                            ywb_uint8_t *data, ywb_uint64_t data_len)
{
    int ret = EC_OK;
    ywb_uint32_t all_node = n + m;
    ywb_uint32_t i = 0;
    ywb_uint32_t j = 0;
    ywb_uint32_t k = 0;
    ywb_uint32_t index = 0;
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *recover_data = NULL;
    ywb_uint64_t recover_len = 0;
    ywb_uint64_t data_skip = 0;
    ywb_uint64_t recover_skip = 0;
    
    EC_ASSERT(n <= m);
    EC_ASSERT(data_len != ((m + n) * BIT_WIDTH * EC_PRE_SUB_UNIT));

    for (i = 0; i < all_node; i++)
    {
        for (j = i + 1; j < all_node; j++)
        {
            for (k = j + 1; k < all_node; k ++)
            {          
                memset(&ec_status, 0x0, sizeof(ec_status_t));
                memset(&recover_table, 0x0, sizeof(ec_recover_table_t));
                ec_status.config.bit_width = BIT_WIDTH;
                ec_status.config.du_count = n;
                ec_status.config.count_pu = m;
                if (i < n)  
                { ec_status.dfault_array[ec_status.dfault_num++] = i;}
                else 
                {ec_status.array_pfault[ec_status.num_pfault++] = i%n;}

                if (j < n)  
                { ec_status.dfault_array[ec_status.dfault_num++] = j;}
                else 
                {ec_status.array_pfault[ec_status.num_pfault++] = j%n;}
                
                if (k < n)  
                { ec_status.dfault_array[ec_status.dfault_num++] = k;}
                else 
                {ec_status.array_pfault[ec_status.num_pfault++] = k%n;}
                
                ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
                EC_ASSERT(EC_OK != ret);
                
                EC_ASSERT(3 != recover_table.recover_num);
                ec_printf_recover_table(&ec_status, &recover_table);
                ret = ec_get_recover_data_by_recover_table(&ec_status, &recover_table,
                                                           data, data_len, &recover_data, &recover_len);
                EC_ASSERT(EC_OK != ret);

                for (index = 0; index < recover_table.recover_num; index++)
                {
                    data_skip = EC_RECOVER_TYPE_DATA == recover_table.recover_node[index].reco_type ? \
                                recover_table.recover_node[index].index :\
                                recover_table.recover_node[index].index + n;
                    data_skip = data_skip * BIT_WIDTH * EC_PRE_SUB_UNIT;
                    recover_skip = index * BIT_WIDTH * EC_PRE_SUB_UNIT;
                    
                    ret = memcmp(data + data_skip, recover_data + recover_skip, BIT_WIDTH * EC_PRE_SUB_UNIT);
                    EC_ASSERT(0 != ret);
                }
                
                if (NULL != recover_data)
                {
                    ec_free(recover_data);
                    recover_data = NULL;
                }
            }
        }
    }

    ec_exhaustion_n_m_fault_less_than_equal_2(n, m, data, data_len);

    return;    
}

void ec_exhaustion_n_m_fault_less_than_equal_4(ywb_uint32_t n , ywb_uint32_t m, 
                                                               ywb_uint8_t *data, ywb_uint64_t data_len)
{
    int ret = EC_OK;
    ywb_uint32_t all_node = n + m;
    ywb_uint32_t i = 0;
    ywb_uint32_t j = 0;
    ywb_uint32_t k = 0;
    ywb_uint32_t l = 0;
    ywb_uint32_t index = 0;
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *recover_data = NULL;
    ywb_uint64_t recover_len = 0;
    ywb_uint64_t data_skip = 0;
    ywb_uint64_t recover_skip = 0;

    EC_ASSERT(n <= m);
    EC_ASSERT(data_len != ((m + n)*BIT_WIDTH * EC_PRE_SUB_UNIT));

    for (i = 0; i < all_node; i++)
    {
        for (j = i + 1; j <all_node; j++)
        {
            for (k = j + 1; k < all_node; k++)
            {
                for (l = k + 1; l < all_node; l++)    
                {
                    memset(&ec_status, 0x0, sizeof(ec_status_t));
                    memset(&recover_table, 0x0, sizeof(ec_recover_table_t));
                    ec_status.config.bit_width = BIT_WIDTH;
                    ec_status.config.du_count = n;
                    ec_status.config.count_pu = m;
                    
                    if (i < n)  
                    { ec_status.dfault_array[ec_status.dfault_num++] = i;}
                    else 
                    {ec_status.array_pfault[ec_status.num_pfault++] = i%n;}
                    
                    if (j < n)  
                    { ec_status.dfault_array[ec_status.dfault_num++] = j;}
                    else 
                    {ec_status.array_pfault[ec_status.num_pfault++] = j%n;}
                    
                    if (k < n)  
                    { ec_status.dfault_array[ec_status.dfault_num++] = k;}
                    else 
                    {ec_status.array_pfault[ec_status.num_pfault++] = k%n;}

                    if (l < n)  
                    { ec_status.dfault_array[ec_status.dfault_num++] = l;}
                    else 
                    {ec_status.array_pfault[ec_status.num_pfault++] = l%n;}
                    
                    ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
                    EC_ASSERT(EC_OK != ret);
                    
                    EC_ASSERT(4 != recover_table.recover_num);                    
                    ec_printf_recover_table(&ec_status, &recover_table);
                    ret = ec_get_recover_data_by_recover_table(&ec_status, &recover_table,
                                                               data, data_len, &recover_data, &recover_len);
                    EC_ASSERT(EC_OK != ret);
                    
                    for (index = 0; index < recover_table.recover_num; index++)
                    {
                        data_skip = EC_RECOVER_TYPE_DATA == recover_table.recover_node[index].reco_type ? \
                                    recover_table.recover_node[index].index :\
                                    recover_table.recover_node[index].index + n;
                        data_skip = data_skip * BIT_WIDTH * EC_PRE_SUB_UNIT;
                        recover_skip = index * BIT_WIDTH * EC_PRE_SUB_UNIT;
                        
                        ret = memcmp(data + data_skip, recover_data + recover_skip, BIT_WIDTH * EC_PRE_SUB_UNIT);
                        EC_ASSERT(0 != ret);
                    }

                    if (NULL != recover_data)
                    {
                        ec_free(recover_data);
                        recover_data = NULL;
                    }
                }
            }
        }
    }

    ec_exhaustion_n_m_fault_less_than_equal_3(n, m, data, data_len);

    return;
}

void ec_partiy_is_4_check(ywb_uint32_t n , ywb_uint32_t m)
{
    int ret = EC_OK;
    ywb_uint8_t *data =  NULL;
    ywb_uint64_t data_len = 0;

    EC_ASSERT(4 != m);
    EC_ASSERT(n <= m);

    ret = ec_create_m_n_data_and_complute_parity(n, m, &data, &data_len);
    EC_ASSERT(EC_OK != ret);

    EC_ASSERT(NULL == data);
    EC_ASSERT(data_len != (n + m) * BIT_WIDTH * EC_PRE_SUB_UNIT);

    ec_exhaustion_n_m_fault_less_than_equal_4(n, m, data, data_len);

    if (NULL != data)
    {
        ec_free(data);
        data = NULL;
    }

    return;
}

void ec_partiy_is_3_check(ywb_uint32_t n , ywb_uint32_t m)
{
    int ret = EC_OK;
    ywb_uint8_t *data =  NULL;
    ywb_uint64_t data_len = 0;

    EC_ASSERT(3 != m);
    EC_ASSERT(n <= m);

    ret = ec_create_m_n_data_and_complute_parity(n, m, &data, &data_len);
    EC_ASSERT(EC_OK != ret);

    EC_ASSERT(NULL == data);
    EC_ASSERT(data_len != (n + m) * BIT_WIDTH * EC_PRE_SUB_UNIT);

    ec_exhaustion_n_m_fault_less_than_equal_3(n, m, data, data_len);

    if (NULL != data)
    {
        ec_free(data);
        data = NULL;
    }

    return;
}

void ec_partiy_is_2_check(ywb_uint32_t n , ywb_uint32_t m)
{
    int ret = EC_OK;
    ywb_uint8_t *data =  NULL;
    ywb_uint64_t data_len = 0;

    EC_ASSERT(2 != m);
    EC_ASSERT(n <= m);

    ret = ec_create_m_n_data_and_complute_parity(n, m, &data, &data_len);
    EC_ASSERT(EC_OK != ret);

    EC_ASSERT(NULL == data);
    EC_ASSERT(data_len != (n + m) * BIT_WIDTH * EC_PRE_SUB_UNIT);

    ec_exhaustion_n_m_fault_less_than_equal_2(n, m, data, data_len);

    if (NULL != data)
    {
        ec_free(data);
        data = NULL;
    }

    return;
}

void ec_partiy_is_1_check(ywb_uint32_t n , ywb_uint32_t m)
{
    int ret = EC_OK;
    ywb_uint8_t *data =  NULL;
    ywb_uint64_t data_len = 0;

    EC_ASSERT(1 != m);
    EC_ASSERT(n <= m);

    ret = ec_create_m_n_data_and_complute_parity(n, m, &data, &data_len);
    EC_ASSERT(EC_OK != ret);

    EC_ASSERT(NULL == data);
    EC_ASSERT(data_len != (n + m) * BIT_WIDTH * EC_PRE_SUB_UNIT);

    ec_exhaustion_n_m_fault_is_equal_1(n, m, data, data_len);

    if (NULL != data)
    {
        ec_free(data);
        data = NULL;
    }

    return;
}


int main(int argc, char **argv)
{
    int ret = EC_OK;
    ywb_uint32_t i = 0; 
    
#if 1
    ec_status_t ec_status;
    ec_recover_table_t recover_table;
    ywb_uint8_t *test_data = NULL;
    ywb_uint64_t data_len = 0;
    
    memset(&ec_status, 0x0, sizeof(ec_status_t));
    memset(&recover_table, 0x0, sizeof(ec_recover_table_t));

    ec_status.config.bit_width = 4;
    ec_status.config.du_count = 7;
    ec_status.config.count_pu = 4;

    ec_status.dfault_num = 2;
    ec_status.dfault_array[0] = 2;    
    ec_status.dfault_array[1] = 4;
    
    ec_status.num_pfault = 2;
    ec_status.array_pfault[0] = 0;
    ec_status.array_pfault[1] = 2;
    //ec_status.array_pfault[2] = 3;

    ret = ec_get_recover_table_by_ec_status(&ec_status, &recover_table);
    if (EC_OK != ret)
    {
        ec_log("Faield to get recover table, ret:%d");
    }

    ec_printf_recover_table(&ec_status, &recover_table);
#endif

    for (i = 2; i <= MAX_DATA_UNIT; i++)
    {        
        //ec_partiy_is_1_check(i, 1);
    }

    for (i = 3; i <= MAX_DATA_UNIT; i++)
    {        
        //ec_partiy_is_2_check(i, 2);
    }
    
    for (i = 4; i <= MAX_DATA_UNIT; i++)
    {        
       //ec_partiy_is_3_check(i, 3);
    }

    for (i = 5; i <= MAX_DATA_UNIT; i++)
    {        
        //ec_partiy_is_4_check(i, 4);
    }

    //ec_partiy_is_4_check(10, 4);
    
    return 0;
}
