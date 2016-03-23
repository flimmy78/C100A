
#include "epdk.h"
#include "rat_partition.h"

/*
**********************************************************************************************************************
*                                               get_part_by_name
*
* author:          terry.zeng 
*
* date:             2010-09-13
*
* Description:       ��ȡ���� 
*
* parameters:       target�����룬 Ŀ����� res_part����������صķ�����
*
* return:           0 	get part success
-1	part not exist
-2	part not formatted
* modify history: 
**********************************************************************************************************************
*/
static int get_part_by_name(const char *equipment, char *des, __s32 search_encrypt_card)
{
    __s32   ret = -1;
    __s32   i = 0;
    __s32   status = 0;
    char    equipment_name[MAX_FS_NAME_LEN];
    char    part_name[RAT_MAX_PARTITION][4];
    char    *partition[RAT_MAX_PARTITION] = {"e:","f:", "g:", "h:", "i:", "j:", "k:", "l:", "m:", "n:", "o:"};	
	
    eLIBs_memset(part_name, 0, RAT_MAX_PARTITION * 4);
    for(i = 0; i < RAT_MAX_PARTITION; i++)
    {
        eLIBs_memset(equipment_name, 0, MAX_FS_NAME_LEN);
        eLIBs_GetPTName(partition[i], equipment_name);
        if(eLIBs_strncmp(equipment_name, equipment, eLIBs_strlen(equipment)) == 0)
        {
            if(0 == search_encrypt_card && eLIBs_strncmp(equipment_name,"SDMMC-DISK:5", eLIBs_strlen("SDMMC-DISK:5")) == 0)
            {//������������ܿ������ҵ�ǰ�Ǽ��ܿ����򲻼�¼
            }
            else
        {
            status = eLIBs_IsPartFormated(partition[i]);
            if(status==ELIBS_PART_FMT_FORMATED)
            {
                eLIBs_strcpy(part_name[i], partition[i]);
                ret = 0;
            }
            else
            {
                ret = -2;
                }
            }
        }
    }
    eLIBs_memcpy(des, part_name, RAT_MAX_PARTITION * 4);
    
    return ret;
}

/*
**********************************************************************************************************************
*                                               
*
* author:          
*
* date:             
*
* Description:       ����̷���Ϣ 
*
* parameters:       disk_type: RAT_SD_CARD RAT_USB_DISK RAT_LOCAL_DISK
					disk_name: ���ص��̷���
*
* return:           partition count
					-1	part not exist					
* modify history: 
**********************************************************************************************************************
*/
__s32 rat_get_partition_name(char* disk_type, char disk_name[RAT_MAX_PARTITION][4], __s32 search_encrypt_card)
{
	char                *equipment[4] = {"UDISK", "SDMMC-DISK", "SCSI_DISK", "USB_CDROM"};    
    char*               usb_cdfs = "USB_CDROM";
    __s32               equipment_index = 0;
	char                name[RAT_MAX_PARTITION][4];
	__s32				i;
	__s32				j;

    __wrn("rat_get_partition_name, disk_type=%s\n", disk_type);

	eLIBs_memset(disk_name, 0, RAT_MAX_PARTITION*4);
	eLIBs_memset(name, 0, sizeof(name));

	if(eLIBs_strcmp(disk_type, RAT_LOCAL_DISK) == 0)
    {
        equipment_index = 0;
    }
    else if(eLIBs_strcmp(disk_type, RAT_SD_CARD) == 0)
    {
        equipment_index = 1;
    }
    else if(eLIBs_strcmp(disk_type, RAT_USB_DISK) == 0)
    {
        equipment_index = 2;
    }
	else if(eLIBs_strcmp(disk_type, RAT_CD_ROM) == 0)
    {
        equipment_index = 3;
    }
    else
    {
        return EPDK_FAIL;
    }

    j = 0;
	if(get_part_by_name(equipment[equipment_index], (char *)name, search_encrypt_card) == 0)
    {		
		for(i=0; i< RAT_MAX_PARTITION; i++)
        {
            if(name[i][0])
            {
				eLIBs_strcpy(disk_name[j], name[i]);
				j++;
			}
		}
	}

    //�����u�̣����cdfs
    if(eLIBs_strcmp(disk_type, RAT_USB_DISK) == 0)
    {        
        if(get_part_by_name(usb_cdfs, (char *)name, 0) == 0)
        {		
    		for(i=0; i< RAT_MAX_PARTITION; i++)
            {
                if(name[i][0])
                {
                    __wrn("cdfs part found, name[i]=%s\n", name[i]);
    				eLIBs_strcpy(disk_name[j], name[i]);
    				j++;
    			}
    		}
    	}
    }

	return EPDK_OK;
}

//�ж��Ƿ���ڼ��ܷ����������̷�����eq_name,��������
//eq_name �̷����buffer  ;  encryption 1����������0���Ǽ�����
__u32 rat_is_encryption_partition_Insert(char *eq_name, __s32 encryption)
{
    __s32   i = 0;
	char    equipment_nm[MAX_FS_NAME_LEN];
    char    *equipment = &equipment_nm[0];
    char    equipment_name[MAX_FS_NAME_LEN]; 
    char    *partition[RAT_MAX_PARTITION] = {"e:","f:", "g:", "h:", "i:", "j:", "k:", "l:", "m:", "n:", "o:"};	

    if(encryption == 1)
    {
        eLIBs_strcpy(equipment,"SDMMC-DISK:50");
	}
	else
	{
        eLIBs_strcpy(equipment,"SDMMC-DISK:00");
	}
		
	for(i = 0; i < RAT_MAX_PARTITION; i++)
    {
        eLIBs_memset(equipment_name, 0, MAX_FS_NAME_LEN);
        eLIBs_GetPTName(partition[i], equipment_name);
		
		__inf("****************************************\n");
		__inf("equipment_name = %s\n",equipment_name);		
		__inf("****************************************\n");		

        if(eLIBs_strncmp(equipment_name, equipment, eLIBs_strlen(equipment)) == 0)
        {
            if(eq_name)
            {
				eLIBs_strcpy((void *) eq_name, partition[i]);
				eLIBs_strcat(eq_name,"\\");
			}

            return 1;
        }
    }
    return 0;
}
