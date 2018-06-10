# my_erasure_coding
+ 该方法采用二进制矩阵进行纠删码的编解码
+ 提供n+m<=16的编解码方式，2<=n<=12，2<=m<=4
+ 使用方法只需要构造数据节点的状态信息，调用ec_get_recover_table_by_ec_status接口可以得到数据块解码的拓扑
+ 二进制矩阵避免RS编码的乘法查表运算，所有编码全部是关联数据块的异或运算，理解简单

----------

# recover_table说明
+ 失效状态信息

```
ec_status.config.du_count = 7;    /*数据节点7个*/
ec_status.config.count_pu = 4;    /*校验节点4个*/
ec_status.dfault_num = 2;         /*失效数据节点2个*/
ec_status.dfault_array[0] = 2;    /*第2个数据节点失效(起始节点号码为0)*/
ec_status.dfault_array[1] = 4;	  /*第4个数据节点失效(起始节点号码为0)*/
ec_status.num_pfault = 2;         /*失效校验节点为2个*/
ec_status.array_pfault[0] = 0;	  /*第0个校验节点失效*/
ec_status.array_pfault[1] = 2;	  /*第2个校验节点失效*/

```	
+ 计算数据源对应bitmap信息

```
ec_status:d_count:7, p_count_4, d_fault:2, p_fault:2, cost_time:57.935715(us).
    fault_type:data, index:2, map_len:44
            bit_map1:1101 0000 0011 0000 1100 0011 0000 0100 0000 0010 1011 
	    bit_map2:1001 0000 0110 0000 1011 0110 0000 1000 0000 0100 0101
	    bit_map3:0001 0000 1100 0000 0101 1100 0000 0011 0000 1000 1010
	    bit_map4:0010 0000 1011 0000 1010 1011 0000 0110 0000 0011 0111
    fault_type:data, index:4, map_len:44
	    bit_map1:0011 0000 0110 0000 0010 1100 0000 0111 0000 1111 1011
	    bit_map2:0110 0000 1100 0000 0100 1011 0000 1110 0000 1101 0101
            bit_map3:1100 0000 1011 0000 1000 0101 0000 1111 0000 1001 1010
	    bit_map4:1011 0000 0101 0000 0011 1010 0000 1101 0000 0001 0111
    fault_type:parity, index:0, map_len:44
            bit_map1:1110 0000 0101 0000 1111 1110 0000 0010 0000 1100 0001
	    bit_map2:1111 0000 1010 0000 1101 1111 0000 0100 0000 1011 0010
	    bit_map3:1101 0000 0111 0000 1001 1101 0000 1000 0000 0101 0100
	    bit_map4:1001 0000 1110 0000 0001 1001 0000 0011 0000 1010 1000
    fault_type:parity, index:2, map_len:44
	    bit_map1:1100 0000 0111 0000 1010 1010 0000 1010 0000 0111 1000
	    bit_map2:1011 0000 1110 0000 0111 0111 0000 0111 0000 1110 0011
	    bit_map3:0101 0000 1111 0000 1110 1110 0000 1110 0000 1111 0110
	    bit_map4:1010 0000 1101 0000 1111 1111 0000 1111 0000 1101 1100
```	
+ bitmap说明
```
	1、fault_type:data, index:2
		bit_map1:1101 0000 0011 0000 1100 0011 0000 0100 0000 0010 1011 
			 (c3) (c2) (c1) (c0) (d6) (d5) (d4) (d3) (d2) (d1) (d0)
	2、第二个数据节点第一个数据子块恢复bitmap，每4位代表一个数据块，每4位中一个位为1表示数据块中第i个子块要参与异或校验
        3、如果有更好的建议和欢迎交流:QQ510539763
```	
<a href="https://info.flagcounter.com/LsRc"><img src="https://s11.flagcounter.com/count2/LsRc/bg_FFFFFF/txt_000000/border_CCCCCC/columns_2/maxflags_10/viewers_0/labels_0/pageviews_0/flags_0/percent_0/" alt="Flag Counter" border="0"></a>
