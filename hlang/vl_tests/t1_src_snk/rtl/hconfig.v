

`ifndef HCONFIG_V_FILE
`define HCONFIG_V_FILE 1
//--------------------------------------------


`define NS_ADDRESS_SIZE 6
`define NS_DATA_SIZE 3
`define NS_REDUN_SIZE 3 // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_PACKET_SIZE 2 // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_1to2_FSZ 1
`define NS_2to1_FSZ 2
`define NS_PACKOUT_FSZ 4  // 1, 2 or 4 ***ONLY***
`define NS_PACKIN_FSZ 4  // 1, 2 or 4 ***ONLY***

`define NS_REQ_CKS 1
`define NS_ACK_CKS 1

`define NS_DECLARE_ERR_CHNL(nam) `NS_DECLARE_ERR_T2_CHNL(nam)
`define NS_DECLARE_ERR_LINK(nam) `NS_DECLARE_ERR_T2_LINK(nam)
`define NS_INSTA_ERR_CHNL(chn0, chn1) `NS_INSTA_ERR_T2_CHNL(chn0, chn1)
`define NS_DECLARE_ERR_REG(nam) `NS_DECLARE_ERR_T2_REG(nam)
`define NS_INIT_ERR_REG(nam) `NS_INIT_ERR_T2_REG(nam)
`define NS_SET_ERR_REG(ou, mg) `NS_SET_ERR_T2_REG(ou, mg)
`define NS_ASSIGN_ERR_CHNL(ou, mg, err_addr) `NS_ASSIGN_ERR_T2_CHNL(ou, mg, err_addr)
`define NS_SELECT_ONE_ERR_LINK(ou, err0, err1) `NS_SELECT_ONE_ERR_T2_LINK(ou, err0, err1)


//--------------------------------------------
`endif // HCONFIG_V_FILE

