#ifndef OPL_H
#define OPL_H


class opl
{
    public:
        /*
        opl(INT16 CN1_EC_l, INT16 CN1_EC_CACMO_l, INT32 CN1_MEP_TP01_l, INT32 CN1_MEP_PR01_l, INT32 CN1_MEP_PR02_l,
            INT32 CN1_MEP_PR03, INT32 CN1_MEP_PR04, INT8 CN1_E_VCE, INT8 CN1_E_VBCE, INT8 CN1_E_VCO, INT8 CN1_E_VBCO,
            INT16 CN2_EC, INT16 CN2_EC_CACOE, INT32 CN2_MEP_TP02, INT32 CN2_MEP_TP03, INT32 CN2_MEP_PR05, INT32 CN2_MEP_PR06,
            INT32 CN2_MEP_PR07, INT8 CN2_E_VPrO, INT8 CN2_E_VMAO, INT16 CN3_EC, INT16 CN3_EC_CACEHP, INT32 CN3_MEP_TP04,
            INT32 CN3_MEP_TP05, INT32 CN3_MEP_PR08, INT32 CN3_MEP_PR09, INT32 CN3_MEP_PR10, INT8 CN3_E_VMAPr, INT8 CN3_E_VPrG,
            INT8 CN3_E_VPrE, INT8 CN3_E_VMAE);
        */
        opl();
        ~opl();

        bool initOPL();
        bool testOPL();


    protected:

    private:
        /*
        INT16 CN1_EC;
        INT16 CN1_EC_CACMO;
        INT32 CN1_MEP_TP01;
        INT32 CN1_MEP_PR01;
        INT32 CN1_MEP_PR02;
        INT32 CN1_MEP_PR03;
        INT32 CN1_MEP_PR04;
        INT8 CN1_E_VCE;
        INT8 CN1_E_VBCE;
        INT8 CN1_E_VCO;
        INT8 CN1_E_VBCO;

        INT16 CN2_EC;
        INT16 CN2_EC_CACOE;
        INT32 CN2_MEP_TP02;
        INT32 CN2_MEP_TP03;
        INT32 CN2_MEP_PR05;
        INT32 CN2_MEP_PR06;
        INT32 CN2_MEP_PR07;
        INT8 CN2_E_VPrO;
        INT8 CN2_E_VMAO;

        INT16 CN3_EC;
        INT16 CN3_EC_CACEHP;
        INT32 CN3_MEP_TP04;
        INT32 CN3_MEP_TP05;
        INT32 CN3_MEP_PR08;
        INT32 CN3_MEP_PR09;
        INT32 CN3_MEP_PR10;
        INT8 CN3_E_VMAPr;
        INT8 CN3_E_VPrG;
        INT8 CN3_E_VPrE;
        INT8 CN3_E_VMAE;
        */
};

#endif // OPL_H