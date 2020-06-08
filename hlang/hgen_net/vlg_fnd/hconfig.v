
`ifndef HCONFIG_V_FILE
`define HCONFIG_V_FILE 1
//--------------------------------------------


`define NS_ADDRESS_SIZE 6
`define NS_DATA_SIZE 4
`define NS_REDUN_SIZE 4 // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_PACKET_SIZE 3 // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_1to2_FSZ 1
`define NS_2to1_FSZ 2
`define NS_PACKOUT_FSZ 4  // 1, 2 or 4 ***ONLY***
`define NS_PACKIN_FSZ 4  // 1, 2 or 4 ***ONLY***

`define NS_REQ_CKS 1
`define NS_ACK_CKS 1

//--------------------------------------------
`endif // HCONFIG_V_FILE


