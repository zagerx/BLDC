typedef struct _cmdmap
{
    unsigned short cmd;
    void* (pfunc)(unsigned char* pdata,unsigned short datalen);
}cmdmap_t;

static cmdmap_t commend_map[] = {
    {0x0002,NULL},
};

void _forch_cmdmap(unsigned short cmd,unsigned char *pdata,unsigned short len)
{
    for (unsigned short i = 0; i < sizeof(commend_map)/sizeof(cmdmap_t); i++)
    {
        if (cmd != commend_map[i].cmd)
        {
            continue;
        }
        
        if (commed_map[i].pfunc != 0)
        {
            commed_map[i].pfunc(pdata,len);
        }
    }   
}


