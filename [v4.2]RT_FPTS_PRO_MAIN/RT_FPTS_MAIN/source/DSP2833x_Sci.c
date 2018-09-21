/**********************************************************************
* @file		DSP2833x_Sci.c
* @brief	DSP28335������������

* @version	1.0
* @date		2016-08-30 20:49:03
* @author	duchentao
*
* @version	2.0
* @date		2017-01-04 13:27:03
* @author	duchentao
* @brief	��������modbus�Ľӿں���
*
* *@version	2.1
* @date		2017-01-04 13:27:03
* @author	tangkefeng
* @brief	����modbusc�Ľӿ�
**********************************************************************/
#include "DSP2833x_Device.h"     
#include "DSP2833x_Examples.h"

uchar RX_BUFA[MAX_BUFA_SIZE] = {0};				
uchar RX_BUFB[MAX_BUFB_SIZE] = {0};				
uchar RX_BUFC[MAX_BUFC_SIZE] = {0};	

/*scia��ʱ���ձ���*/
Uint16  scia_timeout_cnt    = 0;	// 8�ֽڳ�ʱ������
Uint16 	scia_rcv_counter    = 0;	// �յ����ݸ�������ֵ
/*scib��ʱ���ձ���*/
Uint16  scib_timeout_cnt    = 0;	// 8�ֽڳ�ʱ������
Uint16  scib_rcv_counter    = 0;	// �յ����ݸ�������ֵ

/*scic��ʱ���ձ���*/
Uint16  scic_timeout_cnt    = 0;	// 8�ֽڳ�ʱ������
Uint16  scic_rcv_counter    = 0;	// �յ����ݸ�������ֵ

Uint16 	FlagSciBRcvOK 		= 0;	// �ɹ�����8���ֽ����ݱ�־
Uint16 	FlagSciCRcvOK 		= 0;	// �ɹ�����8���ֽ����ݱ�־

/************************************
�ڲ�����������
    -�жϺ�����
    -������ʼ��������
************************************/
__interrupt void SCIA_RXINT_ISR(void);	
__interrupt void SCIB_RXINT_ISR(void);	
__interrupt void SCIC_RXINT_ISR(void);	

void SCIA_DataInit(void);
void SCIB_DataInit(void);
void SCIC_DataInit(void);

/*********************************************************************
 * @brief		SCIA��ʼ��
 * @param[in]	ch_pin ѡ������/����Ĺܽ�
 				-0��GPIO28,GPIO29;
 				-1��GPIO35,GPIO36;
 * @param[in]	baudrate ���ò�����
 * @return 		None
 *********************************************************************/
void SCIA_Init(uchar ch_pin, Uint32 baudrate)
{
    Uint32 tmp;
    SCI_CFG_Type SCIX_MODE;
    
    SCIX_MODE.Baudrate = baudrate;
    SCIX_MODE.Databits  = SCI_DATABIT_8;
    SCIX_MODE.Parity    = SCI_PARITY_NONE;
    SCIX_MODE.Stopbits  = SCI_STOPBIT_1;
	/*
	���²��ֽ�ֹ�û��޸�;
	*/	

	    // Setup only the GPI/O only for SCI-Afunctionality
   	EALLOW;
		if (0 == ch_pin)
   		{
   		    GpioCtrlRegs.GPAPUD.bit.GPIO28      = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
			GpioCtrlRegs.GPAPUD.bit.GPIO29      = 0;    // Enable pull-up for GPIO29 (SCITXDA)
			GpioCtrlRegs.GPAQSEL2.bit.GPIO28    = 3;    // Asynch input GPIO28 (SCIRXDA)
			GpioCtrlRegs.GPAMUX2.bit.GPIO28     = 1;    // Configure GPIO28 for SCIRXDA operation
			GpioCtrlRegs.GPAMUX2.bit.GPIO29     = 1;    // Configure GPIO29 for SCITXDA operation   		
   		}
   		else if (1 == ch_pin)
   		{
   			GpioCtrlRegs.GPBPUD.bit.GPIO35      = 0;    // Enable pull-up for GPIO35  (SCITXDA)
   			GpioCtrlRegs.GPBPUD.bit.GPIO36      = 0;    // Enable pull-up for GPIO36 (SCIRXDA)
   			GpioCtrlRegs.GPBQSEL1.bit.GPIO36    = 3;    // Asynch input GPIO36 (SCIRXDA)
   			GpioCtrlRegs.GPBMUX1.bit.GPIO35     = 2;    // Configure GPIO35 for SCITXDA operation
   			GpioCtrlRegs.GPBMUX1.bit.GPIO36     = 2;    // Configure GPIO36 for SCIRXDA operation  		
   		}

   	EDIS;
   		
   		
		SciaRegs.SCICTL1.all = 0x0003;				//���븴λ״̬
		
		//ģʽ������������
		tmp = (Uint32)LSPCLK_RATE / (8 * baudrate) - 1;
		SciaRegs.SCIHBAUD	= ((Uint16)tmp & 0xff00)>>8;  	// �����ʣ���LSPCLK = 37.5MHz��; 
		SciaRegs.SCILBAUD	= (Uint16)tmp  & 0x00ff;      
		
		SciaRegs.SCICCR.bit.SCICHAR 	= SCIX_MODE.Databits;
		SciaRegs.SCICCR.bit.STOPBITS 	= SCIX_MODE.Stopbits;
		SciaRegs.SCICCR.bit.LOOPBKENA 	= 0;
		
		if (0 == SCIX_MODE.Parity)
		{
			SciaRegs.SCICCR.bit.PARITYENA 	= 0;
			SciaRegs.SCICCR.bit.PARITY 		= 0;
		}
		else if (1 == SCIX_MODE.Parity)
		{
			SciaRegs.SCICCR.bit.PARITYENA 	= 1;
			SciaRegs.SCICCR.bit.PARITY 		= 0;
		}
		else if (2 == SCIX_MODE.Parity)
		{
			SciaRegs.SCICCR.bit.PARITYENA 	= 1;
			SciaRegs.SCICCR.bit.PARITY 		= 1;
		}
		
		SciaRegs.SCICTL2.bit.TXINTENA   = 0;
		SciaRegs.SCICTL2.bit.RXBKINTENA = 1;		// ʹ��SCIA�����ж� 	  
		
		SciaRegs.SCIFFTX.all=0xC000;
   		SciaRegs.SCIFFRX.all=0x4068;
   		SciaRegs.SCIFFCT.all=0x0000;
		
		SciaRegs.SCIFFTX.bit.TXFIFOXRESET   = 1;		//
		SciaRegs.SCIFFRX.bit.RXFIFORESET    = 1;		//
		
		SciaRegs.SCICTL1.bit.SWRESET =1;    		//�˳���λ״̬
		
		
		//ʹ���жϣ��������輶��PIE����CPU�����������ж�����
	EALLOW;	
		PieVectTable.SCIRXINTA          = &SCIA_RXINT_ISR;	//��SCIA�жϺ�����ڸ���PIE������
	EDIS; 
		
		PieCtrlRegs.PIECTRL.bit.ENPIE   = 1;   		// Enable the PIE block
		PieCtrlRegs.PIEIER9.bit.INTx1   = 1;			// ʹ��PIE�ڵ� SCIA_RX_INT�ж� 
		IER |= M_INT9;								// ʹ�� CPU INT9	 
	EINT;   										// ȫ���ж�ʹ��INTM
	
	SCIA_DataInit();
}

/*********************************************************************
 * @brief		SCIB��ʼ��
 * @param[in]	ch_pin ѡ������/����Ĺܽ�
 				-0��GPIO9,GPIO11;
 				-1��GPIO14,GPIO15;
 				-2��GPIO18,GPIO19;
 				-3��GPIO22,GPIO23;
 * @param[in]	baudrate ���ò�����
 * @return 		None
 *********************************************************************/

void SCIBInit(uchar ch_pin, Uint32 baudrate)
{
	Uint32 tmp;
	SCI_CFG_Type SCIX_MODE;

	SCIX_MODE.Baudrate  = baudrate;         
	SCIX_MODE.Databits  = SCI_DATABIT_8;
	SCIX_MODE.Parity    = SCI_PARITY_NONE;
	SCIX_MODE.Stopbits  = SCI_STOPBIT_1;

	//���²��ֽ�ֹ�û��޸�;

	//���Ӳ���У��
	
		// Step 1. Initialize GPIO:
	
		if (0 == ch_pin)
   		{
   			EALLOW;
   			GpioCtrlRegs.GPAPUD.bit.GPIO9       = 0;    // Enable pull-up for GPIO9  (SCITXDB)
   			GpioCtrlRegs.GPAPUD.bit.GPIO11      = 0;    // Enable pull-up for GPIO11 (SCIRXDB)
   			GpioCtrlRegs.GPAQSEL1.bit.GPIO11    = 3;    // Asynch input GPIO11 (SCIRXDB)
   			GpioCtrlRegs.GPAMUX1.bit.GPIO9      = 2;    // Configure GPIO9 for SCITXDB operation
   			GpioCtrlRegs.GPAMUX1.bit.GPIO11     = 2;    // Configure GPIO11 for SCIRXDB operation
   			EDIS;
   		}
   		else if (1 == ch_pin)
   		{
   			EALLOW;
   			GpioCtrlRegs.GPAPUD.bit.GPIO14      = 0;    // Enable pull-up for GPIO14  (SCITXDB)
   			GpioCtrlRegs.GPAPUD.bit.GPIO15      = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
   			GpioCtrlRegs.GPAQSEL1.bit.GPIO15    = 3;    // Asynch input GPIO15 (SCIRXDB)
   			GpioCtrlRegs.GPAMUX1.bit.GPIO14     = 2;    // Configure GPIO14 for SCITXDB operation
   			GpioCtrlRegs.GPAMUX1.bit.GPIO15     = 2;    // Configure GPIO15 for SCIRXDB operation
   			EDIS;
   			
   		}
   		else if (2 == ch_pin)
   		{
   			EALLOW;
   			GpioCtrlRegs.GPAPUD.bit.GPIO18      = 0;    // Enable pull-up for GPIO18  (SCITXDB)
   			GpioCtrlRegs.GPAPUD.bit.GPIO19      = 0;    // Enable pull-up for GPIO19 (SCIRXDB)
   			GpioCtrlRegs.GPAQSEL2.bit.GPIO19    = 3;    // Asynch input GPIO19 (SCIRXDB)
   			GpioCtrlRegs.GPAMUX2.bit.GPIO18     = 2;    // Configure GPIO18 for SCITXDB operation
   			GpioCtrlRegs.GPAMUX2.bit.GPIO19     = 2;    // Configure GPIO19 for SCIRXDB operation
   			EDIS;
   		}
   		else if (3 == ch_pin)
   		{
   			EALLOW;
   			GpioCtrlRegs.GPAPUD.bit.GPIO22      = 0;    // Enable pull-up for GPIO22  (SCITXDB)
   			GpioCtrlRegs.GPAPUD.bit.GPIO23      = 0;    // Enable pull-up for GPIO23 (SCIRXDB)
   			GpioCtrlRegs.GPAQSEL2.bit.GPIO23    = 3;    // Asynch input GPIO23 (SCIRXDB)
   			GpioCtrlRegs.GPAMUX2.bit.GPIO22     = 2;    // Configure GPIO22 for SCITXDB operation
   			GpioCtrlRegs.GPAMUX2.bit.GPIO23     = 2;    // Configure GPIO23 for SCIRXDB operation
   			EDIS;
   		}
	
		
		// Step 2. ��ʼ������Ĵ���:
		ScibRegs.SCICTL1.all = 0x0003;				//���븴λ״̬
		//ģʽ������������
		tmp = (Uint32)LSPCLK_RATE / (8 * baudrate) - 1;
		ScibRegs.SCIHBAUD	= ((Uint16)tmp >>8) & 0x00ff;  	// �����ʣ�LSPCLK = 37.5MHz;
		ScibRegs.SCILBAUD	= (Uint16)tmp & 0x00ff;
		
		//ScibRegs.SCIHBAUD = 0x00;  	// �����ʣ�LSPCLK = 37.5MHz;
		//ScibRegs.SCILBAUD = 0x27;

		ScibRegs.SCICCR.bit.SCICHAR 	= SCIX_MODE.Databits;
		ScibRegs.SCICCR.bit.STOPBITS 	= SCIX_MODE.Stopbits;
		ScibRegs.SCICCR.bit.LOOPBKENA 	= 0;
		
		if (0 == SCIX_MODE.Parity)
		{
			ScibRegs.SCICCR.bit.PARITYENA 	= 0;
			ScibRegs.SCICCR.bit.PARITY 		= 0;
		}
		else if (1 == SCIX_MODE.Parity)
		{
			ScibRegs.SCICCR.bit.PARITYENA 	= 1;
			ScibRegs.SCICCR.bit.PARITY 		= 0;
		}
		else if (2 == SCIX_MODE.Parity)
		{
			ScibRegs.SCICCR.bit.PARITYENA 	= 1;
			ScibRegs.SCICCR.bit.PARITY 		= 1;
		}
		else
		{
			ScibRegs.SCICCR.bit.PARITYENA 	= 0;
			ScibRegs.SCICCR.bit.PARITY 		= 0;
		}

		ScibRegs.SCICTL2.bit.TXINTENA   = 0;
		ScibRegs.SCICTL2.bit.RXBKINTENA = 0;

		ScibRegs.SCIFFTX.all = 0xc000;
		ScibRegs.SCIFFRX.all = 0x0028;
		ScibRegs.SCIFFCT.all = 0x00;
		
   		
   		ScibRegs.SCICTL1.bit.SWRESET    = 1;    	//�˳���λ״̬
   		
		ScibRegs.SCIFFTX.bit.TXFIFOXRESET   = 1;
   		ScibRegs.SCIFFRX.bit.RXFIFORESET    = 1;

   		// Step 3. ��ʼ����ʹ���ж�:
   		//ʹ���жϣ��������輶��PIE����CPU�����������ж�����
   		/*
   	DINT;

		EALLOW;	
		PieCtrlRegs.PIECTRL.bit.ENPIE   = 1;
		PieVectTable.SCIRXINTB          = &SCIB_RXINT_ISR;	//��SCIB�жϺ�����ڸ���PIE������
		EDIS;
	  
		PieCtrlRegs.PIEIER9.bit.INTx3   = 1;			// ʹ��PIE�ڵ� SCIB_RX_INT�ж� 
		IER |= M_INT9;								// ʹ�� CPU INT9	    
		EINT;   									    // ȫ���ж�ʹ��INTM
		*/
		SCIB_DataInit();
}

/*********************************************************************
 * @brief		SCIC��ʼ��
 * @param[in]	ch_pin ѡ������/����Ĺܽ�
 				-0��GPIO62,GPIO63;

 * @param[in]	baudrate ���ò�����
 * @return 		None
 *********************************************************************/
void SCICInit(uchar ch_pin, Uint32 baudrate)
{
	Uint32 tmp = 0;
	SCI_CFG_Type SCIX_MODE;
	
	SCIX_MODE.Baudrate = baudrate;         
	SCIX_MODE.Databits  = SCI_DATABIT_8;
	SCIX_MODE.Parity    = SCI_PARITY_NONE;
	SCIX_MODE.Stopbits  = SCI_STOPBIT_1;
	/*
	���²��ֽ�ֹ�û��޸�;
	*/	
	
	EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO62      = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63      = 0;    // Enable pull-up for GPIO63 (SCITXDC)
	GpioCtrlRegs.GPBQSEL2.bit.GPIO62    = 3;    // Asynch input GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBMUX2.bit.GPIO62     = 1;    // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63     = 1;    // Configure GPIO63 for SCITXDC operation
    EDIS;

	ScicRegs.SCICTL1.all = 0x0003;				//���븴λ״̬

	//ģʽ������������
	tmp = (Uint32)LSPCLK_RATE / (8 * baudrate) - 1;
	ScicRegs.SCIHBAUD	= ((Uint16)tmp >> 8) & 0x00ff;  	// ������
	ScicRegs.SCILBAUD	= (Uint16)tmp  & 0x00ff;

	ScicRegs.SCICCR.bit.SCICHAR 	= SCIX_MODE.Databits;
	ScicRegs.SCICCR.bit.STOPBITS 	= SCIX_MODE.Stopbits;
	ScicRegs.SCICCR.bit.LOOPBKENA 	= 0;

	if (0 == SCIX_MODE.Parity)
	{
		ScicRegs.SCICCR.bit.PARITYENA 	= 0;
		ScicRegs.SCICCR.bit.PARITY 		= 0;
	}
	else if (1 == SCIX_MODE.Parity)
	{
		ScicRegs.SCICCR.bit.PARITYENA 	= 1;
		ScicRegs.SCICCR.bit.PARITY 		= 0;
	}
	else if (2 == SCIX_MODE.Parity)
	{
		ScicRegs.SCICCR.bit.PARITYENA 	= 1;
		ScicRegs.SCICCR.bit.PARITY 		= 1;
	}
	else
	{
		ScicRegs.SCICCR.bit.PARITYENA 	= 0;
		ScicRegs.SCICCR.bit.PARITY 		= 0;
	}

	ScicRegs.SCICTL2.bit.TXINTENA = 0;
	ScicRegs.SCICTL2.bit.RXBKINTENA = 0;

	ScicRegs.SCIFFTX.all =  0xC000;
	ScicRegs.SCIFFRX.all =  0x0028;		//0x4068
	ScicRegs.SCIFFCT.all =  0x0000;

	ScicRegs.SCICTL1.bit.SWRESET = 1;    		//�˳���λ״̬

	ScicRegs.SCIFFTX.bit.TXFIFOXRESET   = 1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET    = 1;
		
	//ʹ���жϣ��������輶��PIE����CPU�����������ж�����
/*
	EALLOW;	
	PieCtrlRegs.PIECTRL.bit.ENPIE   = 1;
	PieVectTable.SCIRXINTC          = &SCIC_RXINT_ISR;	//��SCIA�жϺ�����ڸ���PIE������
	EDIS;   

	PieCtrlRegs.PIEIER8.bit.INTx5   = 1;			// ʹ��PIE�ڵ� SCIA_RX_INT�ж�
	IER |= M_INT8;								// ʹ�� CPU INT8
	EINT;   									    // ȫ���ж�ʹ��INTM
*/
	SCIC_DataInit();
}

/*********************************************************************//**
 * @brief		�������ݳ�ʼ��
 * @param[in]

 * @return 		None
 **********************************************************************/
void SCIA_DataInit(void)
{
    memset(RX_BUFA,0x00,sizeof(RX_BUFA));
}
void SCIB_DataInit(void)
{
    memset(RX_BUFB,0x00,sizeof(RX_BUFB));
}
void SCIC_DataInit(void)
{
    memset(RX_BUFC,0x00,sizeof(RX_BUFC));
}

/*********************************************************************//**
 * @brief		���ܴ���
 * @param[in]	

 * @return 		None
 **********************************************************************/
void SCIA_DeInit(void)
{
	SciaRegs.SCICTL1.all = 0x0003;				//���븴λ״̬
}

void SCIB_DeInit(void)
{
	ScibRegs.SCICTL1.all = 0x0003;				//���븴λ״̬
}
void SCIC_DeInit(void)
{
	ScicRegs.SCICTL1.all = 0x0003;				//���븴λ״̬
}


/**********************************************************************
 * @brief		����һ�ֽڵ�����
 * @param[in]	Data	Data to transmit (must be 8-bit long)
 * @return 		None
 **********************************************************************/
uchar SCIA_SendByte(uchar Data)
{
	Uint16 timeout = 0;
	
	while(0 == SciaRegs.SCICTL2.bit.TXRDY)
	{
		timeout++;
		if (timeout>=60000)
		{
			return ERR_NO1;
		}
	}
	SciaRegs.SCITXBUF = Data & 0xff; 
	return NO_ERR;
}

uchar SCIB_SendByte(uchar Data)
{
	Uint16 timeout = 0;

	while(0 == ScibRegs.SCICTL2.bit.TXRDY)
	{
		timeout++;
		if (timeout>=60000)
		{
			return ERR_NO1;
		}
	}
	ScibRegs.SCITXBUF = Data & 0xff;
	return NO_ERR;
}

uchar SCIC_SendByte(uchar Data)
{
	Uint16 timeout = 0;

	while(0 == ScicRegs.SCICTL2.bit.TXRDY)
	{
		timeout++;
		if (timeout>=60000)
		{
			return ERR_NO1;
		}
	}
	ScicRegs.SCITXBUF = Data & 0xff;
	return NO_ERR;
}

/**********************************************************************
 * @brief		����һ���ַ���
 * @param[in]	str[]	Data to transmit (must be 8-bit long)
 				ulNum	�ַ�������
 * @return 		None
 **********************************************************************/
void SCIA_SendStr(uchar str[],Uint16 ulNum)
{
	Uint16 i;

	for(i =0; i < ulNum; i++)
	{ 
		SCIA_SendByte(str[i]);				//�ֽڷ���
	}
}

void SCIB_SendStr(uchar str[],Uint16 ulNum)
{
	Uint16 i;

	for(i =0; i < ulNum; i++)
	{ 
		SCIB_SendByte(str[i]);				//�ֽڷ���
	}
}

void SCIC_SendStr(uchar str[],Uint16 ulNum)
{
	Uint16 i;

	for(i =0; i < ulNum; i++)
	{ 
		SCIC_SendByte(str[i]);				//�ֽڷ���
	}
}


/*---------------------------------------------------------------------
����ԭ�ͣ�interrupt void SCI_RXINT(void)
�������ƣ������жϷ�ʽ���մ�������
�������ܣ�SCI�����жϽ����ַ������������ַ�����
��ڲ�����          
���ڲ�����
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/ 
__interrupt void SCIA_RXINT_ISR(void)
{
	uchar i = 0;
	uchar rcv_n = 0;

 	DINT;

 	rcv_n = SciaRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
	for(i=0; i<rcv_n; i++)
	{
		RX_BUFA[scia_rcv_counter] = (uchar)(SciaRegs.SCIRXBUF.all&0xff);
		scia_rcv_counter++;
		if (scia_rcv_counter >= MAX_BUFA_SIZE)
		{
			scia_rcv_counter = 0;
		} 
	}
	
//	SCIa_HookRxInt();							//���Ӻ����������û����
//	scia_timeout_cnt    = 0;
	
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
	PieCtrlRegs.PIEACK.all |= 0x0100;   		//��Ӧ�ж�
	EINT;   									//�����ж�
}

__interrupt void SCIB_RXINT_ISR(void)
{
	Uint16 i = 0;
	Uint16 rcv_n = 0;

 	rcv_n = ScibRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
 	for(i=0; i<rcv_n; i++)
	{
		RX_BUFB[scib_rcv_counter] = (uchar)(ScibRegs.SCIRXBUF.all&0xff);
		scib_rcv_counter++;
		if (scib_rcv_counter >= MAX_BUFB_SIZE)
		{
			scib_rcv_counter = 0;
		}
	}

	//SCIb_HookRxInt();							//���Ӻ����������û����
	//scib_timeout_cnt    = 0;

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־

	PieCtrlRegs.PIEACK.all |= 0x0100;   		//��Ӧ�ж�
}

__interrupt void SCIC_RXINT_ISR(void)
{
	uchar i = 0;
	uchar rcv_n = 0;

 	DINT;

 	rcv_n = ScicRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
	for(i=0; i<rcv_n; i++)
	{
		RX_BUFC[scic_rcv_counter] = (uchar)(ScicRegs.SCIRXBUF.all&0xff);
		scic_rcv_counter++;
		if (scic_rcv_counter >= MAX_BUFC_SIZE)
		{
			scic_rcv_counter = 0;
		}
	}
	
//	SCIc_HookRxInt();							//���Ӻ����������û����
//	scic_timeout_cnt    = 0;

	ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
	ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
	PieCtrlRegs.PIEACK.all |= 0x0100;   		//��Ӧ�ж�
	EINT;   									//�����ж�
}

/*---------------------------------------------------------------------
����ԭ�ͣ�void SCI_RcvDeal(void)
�������ƣ����ڽ�������
�������ܣ�����ɨ�跽ʽ���մ�������
��ڲ�����
���ڲ�����
��    �ߣ�
��    �ڣ�
----------------------------------------------------------------------*/
void SCIBRcvDeal(void)
{
	uchar i = 0;
	uchar rcv_n = 0;
	uchar scib_rcv_count = 0;

	if(1 == ScibRegs.SCIFFRX.bit.RXFFINT)
	{
		scib_timeout_cnt = 0;
	 	rcv_n = ScibRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
	 	for (i = 0; i < rcv_n; i++)
		{
			RX_BUFB[scib_rcv_count] = (uchar)(ScibRegs.SCIRXBUF.all & 0xff);
			scib_rcv_count++;
			if (scib_rcv_count >= MAX_BUFB_SIZE)
			{
				scib_rcv_count = 0;
			}
		}

	 	if(8 == rcv_n)
	 	{
	 		FlagSciBRcvOK = 1;
	 		rcv_n = 0;
	 	}

		ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
		ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
	}
	else
	{
		if((ScibRegs.SCIFFRX.bit.RXFFST  > 0)&&(ScibRegs.SCIFFRX.bit.RXFFST  < 0x08))
		{
			scib_timeout_cnt++;
			if(scib_timeout_cnt > 10)
			{
				scib_timeout_cnt = 0;
				//ScibRegs.SCIFFRX.bit.RXFIFORESET    = 0;
				//ScibRegs.SCIFFRX.bit.RXFIFORESET    = 1;
				
				rcv_n = ScibRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���				
				for(i = 0; i < rcv_n; i++)
				{
					RX_BUFB[scib_rcv_count] = (uchar)(ScibRegs.SCIRXBUF.all & 0xff);
					scib_rcv_count++;
					if (scib_rcv_count >= MAX_BUFB_SIZE)
					{
						scib_rcv_count = 0;
					}
				}
				ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
				ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
			}
		}
	}
	
	if (1 == ScibRegs.SCIRXST.bit.RXERROR)		// �жϽ����Ƿ���������λSCI
	{
		ScibRegs.SCICTL1.bit.SWRESET = 0;
		DelayUs(10);
		ScibRegs.SCICTL1.bit.SWRESET = 1;
	}
}

void SCICRcvDeal(void)
{
	uchar i = 0;
	uchar rcv_n = 0;
	uchar scic_rcv_count = 0;

	if(1 == ScicRegs.SCIFFRX.bit.RXFFINT)
	{
		scic_timeout_cnt = 0;
	 	rcv_n = ScicRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
	 	for (i = 0; i < rcv_n; i++)
		{
			RX_BUFC[scic_rcv_count] = (uchar)(ScicRegs.SCIRXBUF.all & 0xff);
			scic_rcv_count++;
			if (scic_rcv_count >= MAX_BUFC_SIZE)
			{
				scic_rcv_count = 0;
			}
		}

	 	if(8 == rcv_n)
	 	{
	 		FlagSciCRcvOK = 1;
	 		rcv_n = 0;
	 	}

		ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
		ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
	}
	else
	{
		if((ScicRegs.SCIFFRX.bit.RXFFST  > 0) && (ScicRegs.SCIFFRX.bit.RXFFST  < 0x08))
		{
			scic_timeout_cnt++;
			if(scic_timeout_cnt > 10)
			{
				scic_timeout_cnt = 0;

				rcv_n = ScicRegs.SCIFFRX.bit.RXFFST;		//��ȡFIFO���ݸ���
				for (i = 0; i < rcv_n; i++)
				{
					RX_BUFC[scic_rcv_count] = (uchar)(ScicRegs.SCIRXBUF.all & 0xff);
					scic_rcv_count++;
					if (scic_rcv_count >= MAX_BUFC_SIZE)
					{
						scic_rcv_count = 0;
					}
				}
				ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;		//��������־
				ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;		//����жϱ�־
			}
		}
	}
	
	if (1 == ScicRegs.SCIRXST.bit.RXERROR)		// �жϽ����Ƿ���������λSCI
	{
		ScicRegs.SCICTL1.bit.SWRESET = 0;
		DelayUs(10);
		ScicRegs.SCICTL1.bit.SWRESET = 1;
	}
}

/* --------------------------------- End Of File ------------------------------ */
