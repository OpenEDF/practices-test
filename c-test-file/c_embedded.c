/* device abstract operation */
typedef enum
{
    LED_DISPLAY = 0,
    SWITCH_DISPLAY,
    HC595_DEVICE_MAX,
    HC595_Unknow
} device_num;

typedef struct
{
    unsigned char status;
    unsigned char reseverd[3];
    void (*init)(device_num);
    void (*deinit)(device_num);
    int (*dispaly)(device_num, unsigned char);
    void (*output_control)(device_num);
    void (*clear_register)(device_num);
} hc595_t;

typedef enum
{
    ENABLE,
    DISABLE
} STATUS;

#define INIT_74HC595(clock, status1, status2, status3) \
    /* enable the relate clock */

/* setup the GPIO status */

/* set up the default status */

/* update the hc595 status */

/* chip hardware implement */
static void init_74hc595(sys595_device dev)
{
    if (dev >= HC595_DEVICE_MAX)
        return

            switch (dev)
        {
        case LED_DISPLAY:
            INIT_74HC595(clock);
        case LED_DISPLAY:
            .....

                default : return
        }
}

static void deinit_74hc595(sys595_device dev)
{

    /* close the clock */

    /* update the hc595 status */
}

static int dispaly_74hc595(sys595_device dev, unsigned char type_8)
{

    /* judge the status */
    if (hc595->status == DISABLE)
        return;

    /* disbale the output */
    output_disable(hc595, DISABLE);
}

static void output_control(sys595_device dev, STATUS status)
{
    if (status == ENABLE)
        Setup_GPIO_Level(HIGH);
    else
        Setup_GPIO_Level(LOW);
}

static void clear_register(sys595_device dev)
{
    Setup_GPIO_Level(LOW);
    delay();
    Setup_GPIO_Level(HIGH);
}

/* top designed */
struct hc595_t;

#define __INIT__DEVICE_OPS__(device) \
    disable, 0, init_74hc595(device), deinit_74hc595(device), dispaly_74hc595(device), output_control(device), clear_register(device),

/* create the device gor system */
static struct hc595_t system_hc595[HC595_DEVICE_MAX] = {
#ifdef
    {__INIT__DEVICE_OPS__(system_hc595[LED_DISPLAY])},
#endif
    {__INIT__DEVICE_OPS__(system_hc595[SWITCH_DISPLAY])},
};

void write_data(sys595_device device, unsigned char type_8)
{
    if (system_hc595[device]->status != okay)
    {
        system_hc595[device]->init(system_hc595[device]);
        system_hc595[device]->statu = okay;
    }

    dispaly_74hc595(device, type_8);
}
