/* Define the memory block start/length for the F28335  
   PAGE 0 will be used to organize program sections
   PAGE 1 will be used to organize data sections

    Notes: 
          Memory blocks on F28335 are uniform (ie same
          physical memory) in both PAGE 0 and PAGE 1.  
          That is the same memory region should not be
          defined for both PAGE 0 and PAGE 1.
          Doing so will result in corruption of program 
          and/or data. 
          
          L0/L1/L2 and L3 memory blocks are mirrored - that is
          they can be accessed in high memory or low memory.
          For simplicity only one instance is used in this
          linker file. 
          
          Contiguous SARAM memory blocks can be combined 
          if required to create a larger memory block. 
 */


MEMORY
{
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */

   ZONE0       : origin = 0x004000, length = 0x001000     /* XINTF zone 0 */
   RAML0       : origin = 0x008000, length = 0x001000     /* on-chip RAM block L0 */
   RAML1       : origin = 0x009000, length = 0x001000     /* on-chip RAM block L1 */
   RAML23       : origin = 0x00A000, length = 0x002000     /* on-chip RAM block L2 */
//   RAML3       : origin = 0x00B000, length = 0x001000     /* on-chip RAM block L3 */
	/*�ⲿfalshδʹ��
   EX_FLASHA      : origin = 0x100000, length = 0x040000     XINTF zone 6 - program space*/
   /*ZONE7       : origin = 0x200000, length = 0x100000    *//* XINTF zone 7  */
   FLASHH      : origin = 0x300000, length = 0x008000     /* on-chip FLASH */
   FLASHG      : origin = 0x308000, length = 0x008000     /* on-chip FLASH */
   FLASHF      : origin = 0x310000, length = 0x008000     /* on-chip FLASH */
   FLASHE      : origin = 0x318000, length = 0x008000     /* on-chip FLASH */
   FLASHD      : origin = 0x320000, length = 0x008000     /* on-chip FLASH */
   FLASHC      : origin = 0x328000, length = 0x008000     /* on-chip FLASH */
   FLASHA      : origin = 0x338000, length = 0x007F80     /* on-chip FLASH */
   CSM_RSVD    : origin = 0x33FF80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   BEGIN       : origin = 0x33FFF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
   CSM_PWL     : origin = 0x33FFF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
   OTP         : origin = 0x380400, length = 0x000400     /* on-chip OTP */
   ADC_CAL     : origin = 0x380080, length = 0x000009     /* ADC_cal function in Reserved memory */
   
   IQTABLES    : origin = 0x3FE000, length = 0x000b50     /* IQ Math Tables in Boot ROM */
   IQTABLES2   : origin = 0x3FEB50, length = 0x00008c     /* IQ Math Tables in Boot ROM */  
   FPUTABLES   : origin = 0x3FEBDC, length = 0x0006A0     /* FPU Tables in Boot ROM */
   ROM         : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */        
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */

PAGE 1 :   /* Data Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE0 for program allocation */
           /* Registers remain on PAGE1                                                  */
   
   	BOOT_RSVD   : origin = 0x000000, length = 0x000050     	/* Part of M0, BOOT rom will use this for stack */
   	RAMM0       : origin = 0x000050, length = 0x0003B0     	/* on-chip RAM block M0 */
   	RAMM1       : origin = 0x000400, length = 0x000400     	/* on-chip RAM block M1 */
   	RAML4       : origin = 0x00C000, length = 0x001000     	/* on-chip RAM block L1 */
   	RAML5       : origin = 0x00D000, length = 0x001000     	/* on-chip RAM block L1 */
   	RAML6       : origin = 0x00E000, length = 0x001000     	/* on-chip RAM block L1 */
   	RAML7       : origin = 0x00F000, length = 0x001000     	/* on-chip RAM block L1 */

   	EXRAM_REG	: origin = 0x180000, length = 0x004000		/* ��չ�ⲿRAM_16K*/
	EXRAM	   	: origin = 0x184000, length = 0x014000		/* ��չ�ⲿRAM_80K*/
	EMRAM	   	: origin = 0x1A0000, length = 0x020000		/* Ӧ������RAM_128K*/

   	FLASHB      : origin = 0x330000, length = 0x008000     	/* on-chip FLASH */
}

/* Allocate sections to memory blocks.
   Note:
         codestart user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                   execution when booting to flash
         ramfuncs  user defined section to store functions that will be copied from Flash into RAM
*/ 
 
SECTIONS
{
   	/* Allocate program areas: */
   	.cinit              : > FLASHA      PAGE = 0
   	.pinit              : > FLASHA,     PAGE = 0
   	.text               : > FLASHA      PAGE = 0
   	codestart           : > BEGIN       PAGE = 0
   	ramfuncs            : LOAD = FLASHD,
                         RUN = RAML0, 
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         PAGE = 0

	ramcode            : LOAD = FLASHE, PAGE = 0
                         RUN = RAML23,PAGE = 0
                         LOAD_START(_ramLoadStart),
                         LOAD_END(_ramLoadEnd),
                         RUN_START(_ramRunStart)

	csmpasswds          : > CSM_PWL     PAGE = 0
	csm_rsvd            : > CSM_RSVD    PAGE = 0
   
   	/* Allocate uninitalized data sections: */
   	.stack              : > RAMM1       PAGE = 1
   	.esysmem            : > RAMM1       PAGE = 1
   	.DMARAML4			: > RAML4       PAGE = 1
   	.ebss               : > RAML5       PAGE = 1
   	.ParaTab			: > RAML5	   	PAGE = 1
   	.SciData            : > RAML6	   	PAGE = 1
   	.SinTab				: > RAML6		PAGE = 1
   	.URmsReg			: > RAML6		PAGE = 1
   	.SPLLData			: > RAML7		PAGE = 1

   	.ExRamCtrl			: > EXRAM_REG	PAGE = 1
   	.ExRamData			: > EXRAM	   	PAGE = 1
   	.EmRamData			: > EMRAM	   	PAGE = 1

   	/* Initalized sections go in Flash */
   	/* For SDFlash to program these, they must be allocated to page 0 */
   	.econst             : > FLASHA      PAGE = 0
   	.switch             : > FLASHA      PAGE = 0

   	/* Allocate IQ math areas: */
   	IQmath           : > FLASHC     PAGE = 0                  /* Math Code */
   	IQmathTables     : > IQTABLES,  PAGE = 0, TYPE = NOLOAD
   	IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD
   	FPUmathTables    : > FPUTABLES, PAGE = 0, TYPE = NOLOAD
         
   	/* Allocate DMA-accessible RAM sections: */
/*  DMARAML4         : > RAML4,     PAGE = 1
   	DMARAML5         : > RAML5,     PAGE = 1
   	DMARAML6         : > RAML6,     PAGE = 1
   	DMARAML7         : > RAML7,     PAGE = 1
*/
   	/* Allocate 0x400 of XINTF Zone 6 to storing data
   	ZONE6DATA        : > ZONE6B,    PAGE = 1
	*/

   	/* .reset is a standard section used by the compiler.  It contains the */
   	/* the address of the start of _c_int00 for C Code.   /*
   	/* When using the boot ROM this section and the CPU vector */
   	/* table is not needed.  Thus the default type is set here to  */
   	/* DSECT  */
   	.reset              : > RESET,      PAGE = 0, TYPE = DSECT
   	vectors             : > VECTORS     PAGE = 0, TYPE = DSECT
   
   	/* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
   	.adc_cal     : load = ADC_CAL,   PAGE = 0, TYPE = NOLOAD

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

