#ifndef RESPONE_H
#define RESPONE_H

#include "m1CardLoginResp.h"
#include "getRanLoginCodeResp.h"
#include "noM1CardLoginResp.h"
#include "getCellIdsByTerminalIdResp.h"
#include "saveDeliverysResp.h"
#include "getRanCodeAgainResp.h"
#include "getDeliveryByCodeResp.h"
#include "getExpressCompanysResp.h"
#include "saveExpressEmpResp.h"
#include "getDeliveryConfirmResp.h"
#include "updatePackageStatusResp.h"
#include "reportCellErrorsResp.h"
#include "repairCellErrorsResp.h"
#include "reportCommonErrorsResp.h"
#include "repairCommonErrorsResp.h"
#include "uploadDeliveryPhoto.h"
#include "updateAccountPaidResp.h"
#include "getVIPCardInfoResp.h"
#include "judgeStoredAccountResp.h"
#include "rechargeToAccountResp.h"
#include "addVipCustomerResp.h"
#include "getTariffInformationResp.h"
#include "checkBalanceResp.h"
#include "openScellResp.h"
#include "getSystemInfoResp.h"
#include "getWeacherInfoResp.h"
#include "isConfirmationResp.h"
#include "judgePwdResp.h"
#include "takeRecordResp.h"
#include "getDelsByVIPCardResp.h"
#include "VipBindingM1cardResp.h"
#include "saveTerminalIpResp.h"
#include "deliveryTimeoutResp.h"
#include "expTakeDeliveryResp.h"
#include "checkOpenScellResp.h"
#include "expGetSaveDeliveryResp.h"



#define  RET_FALSE            "false"
#define  RET_TRUE             "true"
#define  PARAM_ERROR          "400"                //参数错误
#define  CONNECT_ERROR        "500"                //连接错误、需重发
#define  DEVICE_ERROR         "10000"              //设备维护中
#define  CELL_OPEN_ERROR      "10001"              //箱门取件打不开
#define  TRANS_KEY_ERROR      "10002"              //SHA密钥错误
#define  IC_FORBID_ERROR      "10003"              //证件卡号被禁用
#define  NOTE_NUMBER_ERROR    "10004"              //获取短信次数过多
#define  NO_COMPANYID_ERROR   "10005"              //公司编号不存在
#define  EXIST_IC_ERROR       "10006"              //身份证已存在
#define  EXIST_PHONE_ERROR    "10007"              //手机号码已存在

#define  NO_PIC_ERROR         "10008"              //身份证号不存在
#define  NOTEXIST_IC_ERROR    "10009"              //M1卡卡号不存在
#define  recharge_PWD_ERROR   "10010"              //充值卡密码错误

#define  recharge_IC_ERROR    "10011"              //充值账号不存在
#define  EXIST_CUSTOMER       "10012"              //VIP账号已存在

#define  NO_CELLID            "10014"              //格子不存在

#define  EXIST_DELIVERY_ERROR "10015"              //派件存在
#define  FILE_NO_EXIST        "10016"              //文件不存在
#define  EXIST_MI_ERROR       "10019"              //M1卡卡号存在
#define  USE_CELLID           "10021"              //格子有物品
#define  FILE_NO_UPDATA       "10022"              //文件不需要更新
#define  TAKE_MONEY_ERROR     "10023"              //取款箱异常未修复
#define  MONEY_CODE_ERROR     "10024"              //取款密码错误

#define  IC_DATE_ERROR        "10026"              //vip卡有效期失效
#define  IC_PHONE_ERROR       "10027"              //vip卡手机号与包裹手机号不匹配

#define  CARD_NO_PACKAGE      "10029"              //终端没有包裹

#define  VIP_BIND_PHONE       "10030"              //VIP 绑定手机号没开通
#define  VIP_BIND_CODE        "10031"              //VIP 绑定密码错误
#define  OVERDUE_PACKAGE      "10032"              //存在滞留件

#endif // RESPONE_H
